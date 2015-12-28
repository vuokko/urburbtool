/*
 * This file is part of the Urb Urb tool.
 *
 * Copyright (C) 2015 Hannu Vuolasaho <vuokkosetae@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "urburbtool.h"
#include "logic.h"
#include <FL/Fl_Menu_.H>
#include <ctype.h>
#include <libusb-1.0/libusb.h>
#include <FL/fl_ask.H>
#include <FL/Fl.H>


#define END_ESCAPE -5
#define RECV_BUF_SIZE 0xffff
UserInterface * ui = 0;
enum ItemType {
  ERROR,DEVICE, DESCRIPTION, CONF_PARENT, CONFIGURATION, INTERFACE, ENDPOINT
};
struct UsbData {
  enum ItemType type;
  union {
    uint8_t config;
    uint8_t interface;
    uint8_t endpoint;
  };
  uint8_t bus;
  uint8_t addr;
  uint16_t vid;
  uint16_t pid;
  bool useThis;
  libusb_device_handle *dev;
  struct libusb_device_descriptor *desc;

  UsbData(){
    bus = 0;
    addr = 0;
    dev = 0;
    vid = 0;
    pid = 0;
    desc = 0;
    useThis = false;
  }
};
static struct libusb_transfer * sendTrans = 0;
static int sendBufSize = 0;
static struct libusb_transfer * recvTrans = 0;
struct ConnectionSettings {
  uint8_t bus;
  uint8_t addr;
  uint16_t vid;
  uint16_t pid;
  int conf;
  int iFace;
  uint8_t rxEP;
  uint8_t txEP;
  libusb_device_handle *handle;
};
static struct ConnectionSettings conn;
static unsigned recvCount = 0;
void cb_button(Fl_Widget* w, void *)
{
  int ret;
  w->deactivate();
  ui->DeviceTree->activate();
  ui->DeviceTree->color(FL_WHITE);
  ui->SendInput->deactivate();

  if((ret = libusb_release_interface(conn.handle, conn.iFace)) != 0){
    fl_alert("Libusb interface release error: %s",
             libusb_error_name(ret));
  }
  if ((ret = libusb_attach_kernel_driver(conn.handle, conn.iFace)) != 0 &&
      ret != LIBUSB_ERROR_NOT_FOUND && ret != LIBUSB_ERROR_NOT_SUPPORTED){
    fl_alert("Libusb kernel driver attach error: %s",
             libusb_error_name(ret));
  }
  libusb_close(conn.handle);
  conn.rxEP = 0xff;
  conn.txEP = 0xff;
  for ( Fl_Tree_Item *item = ui->DeviceTree->first(); item; item = ui->DeviceTree->next(item) ) {
    struct UsbData * data = (struct UsbData *)item->user_data();
    if (data)
      delete data;
  }
  ui->DeviceTree->clear();
  initTree(ui->DeviceTree);
}
static void LIBUSB_CALL cb_recv(struct libusb_transfer * xfer){

  if (recvCount && xfer->status == LIBUSB_TRANSFER_TIMED_OUT)return;
  ui->HexBrowser->receiving = true;
  ui->HexBrowser->add((char *)xfer->buffer, xfer->actual_length);

  switch (xfer->status){
  case LIBUSB_TRANSFER_COMPLETED:
    recvCount++;
    libusb_submit_transfer(recvTrans);
    break;
  case LIBUSB_TRANSFER_ERROR:
    ui->HexBrowser->recv_error("Transfer failed.");
    break;
  case LIBUSB_TRANSFER_TIMED_OUT:
    if(!recvCount)
      ui->HexBrowser->recv_timeout();
    else
      return;
    break;
  case LIBUSB_TRANSFER_CANCELLED:
    ui->HexBrowser->recv_error("Transfer was cancelled.");
    break;
  case LIBUSB_TRANSFER_STALL:
    ui->HexBrowser->recv_error("Halt condition detected (endpoint stalled).");
    break;
  case LIBUSB_TRANSFER_NO_DEVICE:
    ui->HexBrowser->recv_error("Device was disconnected.");
    break;
  case LIBUSB_TRANSFER_OVERFLOW:
  default:
    ui->HexBrowser->recv_error("Unexpected status in send transfer.");
  }
}
static void LIBUSB_CALL cb_send(struct libusb_transfer * xfer)
{
  ui->HexBrowser->receiving = false;
  ui->HexBrowser->add((char *)xfer->buffer, xfer->actual_length);
  switch (xfer->status){
  case LIBUSB_TRANSFER_COMPLETED:
    if (xfer->actual_length != xfer->length)
      ui->HexBrowser->send_error("Not all data was sent.");
    recvCount = 0;
    libusb_submit_transfer(recvTrans);
    break;
  case LIBUSB_TRANSFER_ERROR:
    ui->HexBrowser->send_error("Transfer failed.");
    break;
  case LIBUSB_TRANSFER_TIMED_OUT:
    ui->HexBrowser->send_timeout();
    break;
  case LIBUSB_TRANSFER_CANCELLED:
    ui->HexBrowser->send_error("Transfer was cancelled.");
    break;
  case LIBUSB_TRANSFER_STALL:
    ui->HexBrowser->send_error("Halt condition detected (endpoint stalled).");
    break;
  case LIBUSB_TRANSFER_NO_DEVICE:
    ui->HexBrowser->send_error("Device was disconnected.");
    break;
  case LIBUSB_TRANSFER_OVERFLOW:
  default:
    ui->HexBrowser->send_error("Unexpected status in send transfer.");
  }
}
void cb_SendInput(Fl_Widget* w, void *)
{
  const char * txt;
  char badEsc[4], val;
  Fl_Input * input = (Fl_Input *)w;
  int i, n, len, escaping;
  int hex = 0;
  txt = input->value();
  if (!txt)return;
  len = strlen(txt); //Always bigger.
  if (len > sendBufSize){
    sendBufSize = len;
    sendTrans->buffer = (unsigned char*)realloc(sendTrans->buffer, sendBufSize);
    if (!sendTrans->buffer){
      fl_alert("Libusb transfer buffer reallocation failed. Closing.");
      ui->hide();
      Fl::delete_widget(ui);
      return;
    }
  }
  escaping = -5;
  n = 0;
  for (i = 0; i < len; ++i){
    if (escaping + 2 >= i){
      if (isxdigit(txt[i])){
        val = txt[i];
        val -= 0x30;
        if (val > 0x30)
          val -= 0x27;
        if (val > 0x10)
          val -= 0x7;
        hex |= val << (4*(1 - (i - 1 - escaping)));
        if (escaping + 2 == i){
          ((char *)(sendTrans->buffer))[n++] = (char)(hex & 0xff);
          escaping = END_ESCAPE;
        }
        continue;
      }
      if ((escaping + 1 == i) && txt[i] == '\\'){
        printf("ESC \\\n");
        ((char*)(sendTrans->buffer))[n++] = '\\';
        escaping = END_ESCAPE;
        continue;
      }
      if ((escaping + 1 == i) && txt[i] == 'n'){
        ((char*)(sendTrans->buffer))[n++] = '\n';
        escaping = END_ESCAPE;
        continue;
      }
      if ((escaping + 1 == i) && txt[i] == 'r'){
        ((char*)(sendTrans->buffer))[n++] = '\r';
        escaping = END_ESCAPE;
        continue;
      }
      input->position( escaping, i + 1);
      // We will return after writing error message It is OK to overwrite len.
      len = i - escaping;
      while (escaping <= i){
        badEsc[len - (i - escaping)] = txt[escaping];
        escaping++;
      }
      badEsc[3] = '\0';
      fl_alert("Non understandable escape: %s", badEsc);
      return;
    } else {
      if (txt[i] == '\\'){
        escaping = i;
        hex = 0;
        continue;
      } else {
        ((char*)(sendTrans->buffer))[n++] = txt[i];
      }
    }
  }
  if(escaping >= 0){
    input->position(escaping, len);
    fl_alert("Unended escape.");
    return;
  }
  sendTrans->length = n;
  libusb_submit_transfer(sendTrans);
}
static void openConnection()
{
  libusb_device **list;
  ssize_t cnt, i;
  struct libusb_device_descriptor devdes;
  int ret;

  cnt = libusb_get_device_list(NULL, &list);
  if (cnt < 0){
    fl_alert("Libusb device listing error: %s",libusb_error_name(cnt));
    return;
  }
  for (i = 0; i < cnt; ++i){
    libusb_device *device = list[i];
    if ((ret = libusb_get_device_descriptor (device, &devdes)) != 0){
      fl_alert("Libusb device descriptor error: %s",libusb_error_name(ret));
      goto ending;
    }
    if (libusb_get_bus_number(device) == conn.bus &&
        libusb_get_device_address(device) == conn.addr&&
        devdes.idVendor == conn.vid && devdes.idProduct == conn.pid){
      //Device found
      if ((ret = libusb_open(device, &(conn.handle))) != 0){
        fl_alert("Libusb device open error: %s",libusb_error_name(ret));
        goto ending;
      }
      if ((ret =libusb_set_configuration(conn.handle, conn.conf)) != 0){
        fl_alert("Libusb device configuration error: %s",
                 libusb_error_name(ret));
        goto ending;
      }
      if ((ret = libusb_kernel_driver_active(conn.handle, conn.iFace)) == 1){
        if ((ret = libusb_detach_kernel_driver(conn.handle, conn.iFace)) != 0){
          fl_alert("Libusb kernel driver detach error: %s",
                   libusb_error_name(ret));
          goto ending;
        }
      } else {
        if (ret != 0 && ret != LIBUSB_ERROR_NOT_SUPPORTED){
          fl_alert("Libusb kernel driver query error: %s",
                   libusb_error_name(ret));
          goto ending;
        }
      }
      if ((ret = libusb_claim_interface(conn.handle, conn.iFace)) != 0){
        fl_alert("Libusb interface claim error: %s",
                 libusb_error_name(ret));
        goto ending;
      }
      libusb_fill_bulk_transfer(sendTrans, conn.handle, conn.txEP,
                                sendTrans->buffer, 0, cb_send, 0, 1000);
      libusb_fill_bulk_transfer(recvTrans, conn.handle, conn.rxEP,
                                recvTrans->buffer, RECV_BUF_SIZE, cb_recv, 0, 1000);
      ui->DisconnectButton->activate();
      ui->DisconnectButton->set();
      ui->DeviceTree->deactivate();
      ui->DeviceTree->color(FL_GRAY);
      ui->SendInput->activate();
      return;
    }
  }

 ending:
  libusb_free_device_list(list, 1);
  return;
}

static void cb_tree(Fl_Widget* w, void *)
{
  Fl_Tree_Item * item;
  Fl_Tree * tree = (Fl_Tree *)w;
  struct UsbData *data;
  int iFace, conf;
  char buf[50];

  if (!tree)return;
  item = (Fl_Tree_Item*)tree->callback_item();
  if (!item)return;
  data = (struct UsbData *)item->user_data();
  if (!data||data->type != ENDPOINT)
    return;

  switch (tree->callback_reason()){
  case FL_TREE_REASON_SELECTED:
    iFace = ((struct UsbData *)(item->parent()->user_data()))->interface;
    conf = ((struct UsbData *)(item->parent()->parent()->user_data()))->config;

    if (data->bus != conn.bus || data->addr != conn.addr || conf != conn.conf ||
        iFace != conn.iFace || data->vid != conn.vid || data->pid != conn.pid){
      ui->RxStatus->label("RX: Not set");
      ui->TxStatus->label("TX: Not set");
      ui->Device->label("Device, conf, interface not set");
      memset(&conn, 0, sizeof(conn));
      conn.rxEP = 0xff;
      conn.txEP = 0xff;
    }
    conn.vid = data->vid;
    conn.pid = data->pid;
    conn.conf = conf;
    conn.iFace = iFace;
    conn.bus = data->bus;
    conn.addr = data->addr;
    if (data->endpoint & 0x80){
      conn.rxEP = data->endpoint;
      snprintf(buf, 49, "RX EP: 0x%02x", data->endpoint);
      ui->RxStatus->copy_label(buf);
    } else {
      conn.txEP = data->endpoint;
      snprintf(buf, 49, "TX EP: 0x%02x", data->endpoint);
      ui->TxStatus->copy_label(buf);
    }
    snprintf(buf, 49, "Bus: 0x%02hhx Addr: 0x%02hhx Conf: %d IF: %d",
             data->bus, data->addr, conf, iFace);
    ui->Device->copy_label(buf);
    if(conn.rxEP != 0xff && conn.txEP != 0xff)
      openConnection();
    return;
  case FL_TREE_REASON_DESELECTED:
  case FL_TREE_REASON_OPENED:
  case FL_TREE_REASON_CLOSED:
  case FL_TREE_REASON_DRAGGED:
  case FL_TREE_REASON_NONE:
  default:
    return;
  }
}

static const char *  if_class(uint8_t classof)
{
  switch(classof){
  case LIBUSB_CLASS_PER_INTERFACE:
  default:
    return "None.";
  case LIBUSB_CLASS_AUDIO:
    return "Audio";
  case LIBUSB_CLASS_COMM:
    return "Communications";
  case LIBUSB_CLASS_HID:
    return "Human Interface Device";
  case LIBUSB_CLASS_PHYSICAL:
    return "Physical.";
  case LIBUSB_CLASS_PRINTER:
    return "Printer";
  case LIBUSB_CLASS_PTP:
    return "Image class.";
  case LIBUSB_CLASS_MASS_STORAGE:
    return "Mass storage";
  case LIBUSB_CLASS_HUB:
    return "Hub";
  case LIBUSB_CLASS_DATA:
    return "Data";
  case LIBUSB_CLASS_SMART_CARD:
    return "Smart Card";
  case LIBUSB_CLASS_CONTENT_SECURITY:
    return "Content Security";
  case LIBUSB_CLASS_VIDEO:
    return "Video";
  case LIBUSB_CLASS_PERSONAL_HEALTHCARE:
    return "Personal Healthcare";
  case LIBUSB_CLASS_DIAGNOSTIC_DEVICE:
    return "Diagnostic Device.";
  case LIBUSB_CLASS_WIRELESS:
    return "Wireless";
  case LIBUSB_CLASS_APPLICATION:
    return "Application";
  case LIBUSB_CLASS_VENDOR_SPEC:
    return "Vendor-specific";
  }
}

static void fillDeviceDescription(Fl_Tree * t, Fl_Tree_Item * parent)
{
  char buf[60];
  int ret;
  struct UsbData * data = (struct UsbData *)parent->user_data();
  snprintf(buf, 59, "Specification %hhx.%hhx", (data->desc->bcdUSB) >> 8,
           (data->desc->bcdUSB) &0xff);
  t->add(parent, buf);
  snprintf(buf, 59, "Device class %s subclass 0x%02x",
           if_class(data->desc->bDeviceClass),
           data->desc->bDeviceSubClass);
  t->add(parent, buf);
  snprintf(buf, 59, "Protocol %hhx", data->desc->bDeviceProtocol);
  t->add(parent, buf);
  snprintf(buf, 59, "Maximum packet size for endpoint 0 is %hhd bytes",
           data->desc->bMaxPacketSize0);
  t->add(parent, buf);
  snprintf(buf, 59, "Device release: %x.%x", (data->desc->bcdDevice) >> 8,
           (data->desc->bcdDevice) &0xff);
  t->add(parent, buf);
  if (data->desc->iManufacturer) {
    snprintf(buf, 59, "Manufacturer: ");
    if ((ret = libusb_get_string_descriptor_ascii(data->dev,
                                                  data->desc->iManufacturer,
                                                  (unsigned char *)buf +
                                                  strlen(buf),
                                                  59 - strlen(buf))) < 0){
      fl_alert("Failed to get manufacturer string: %s", libusb_error_name(ret));
    } else {
      t->add(parent, buf);
    }
  }
  if (data->desc->iProduct){
    snprintf(buf, 59, "Product: ");
    if ((ret = libusb_get_string_descriptor_ascii(data->dev,
                                                  data->desc->iProduct,
                                                  (unsigned char *)buf +
                                                  strlen(buf),
                                                  59 - strlen(buf))) < 0){
      fl_alert("Failed to get product string: %s", libusb_error_name(ret));
    } else {
      t->add(parent, buf);
    }
  }

  if (data->desc->iSerialNumber){
    snprintf(buf, 59, "Serial number: ");
    if ((ret = libusb_get_string_descriptor_ascii(data->dev,
                                                  data->desc->iSerialNumber,
                                                  (unsigned char *)buf +
                                                  strlen(buf),
                                                  59 - strlen(buf))) < 0){
      fl_alert("Failed to get serial number string: %s", libusb_error_name(ret));
    } else {
      t->add(parent, buf);
    }
  }
}
static void fillConfigDescription(Fl_Tree * t, Fl_Tree_Item * parent, libusb_device *device, int configCount)
{
  //Fill configuration.
  struct libusb_config_descriptor * config;
  int ret, i,j,k,l;
  char buf[100];
  struct UsbData * data = (struct UsbData *)parent->user_data();
  Fl_Tree_Item * item, *interface, *endpoint;
  libusb_device_handle *handle;
  uint16_t vid, pid;
  uint8_t bus, addr;
  if (!parent||!(data = (struct UsbData *)parent->user_data())){
    fl_alert("fillConfigDescriptor hadn't parent or parent hadn't data. Closing.");
    ui->hide();
    Fl::delete_widget(ui);
    return;
  }
  handle = data->dev;
  vid = data->vid;
  pid = data->pid;
  bus = data->bus;
  addr = data->addr;
  for (i = 0; i < configCount; ++i){
    config = 0;
    if ((ret = libusb_get_config_descriptor(device, i, &config)) != 0){
      fl_alert("Libusb config descriptor error: %s",libusb_error_name(ret));
      libusb_free_config_descriptor(config);
      return;
    }
    snprintf(buf, 99, "%hhu%s", config->bConfigurationValue,
             config->iConfiguration?" ":" Unnamed");
    if (config->iConfiguration &&
        (ret = libusb_get_string_descriptor_ascii(handle,
                                                  config->iConfiguration,
                                                  (unsigned char *)buf +
                                                  strlen(buf),
                                                  99 - strlen(buf))) < 0){
      fl_alert("Failed to get configuration name string: %s", libusb_error_name(ret));
    }
    item = t->add(parent, buf);
    data = new UsbData;
    data->type = CONFIGURATION;
    data->config = config->bConfigurationValue;
    data->vid = vid;
    data->pid = pid;
    data->bus = bus;
    data->addr = addr;
    item->user_data(data);
    snprintf(buf, 99, "current: %humA", ((uint16_t)(config->MaxPower))*2);
    t->add(item, buf);
    snprintf(buf, 99, "%s and %s", (config->bmAttributes & (1 <<6))?"Self powered":"Bus powered",
             (config->bmAttributes & (1 <<5))?"can wake up computer":"doesn't wake up computer");
    t->add(item, buf);
    for (j = 0; j < config->bNumInterfaces; ++j){
      for (k = 0; k < config->interface[j].num_altsetting; ++k){
        snprintf(buf,99, "Interface %hhu",
                 config->interface[j].altsetting[k].bInterfaceNumber);
        interface = t->add(item, buf);
        data = new UsbData;
        data->type = INTERFACE;
        data->interface = config->interface[j].altsetting[k].bInterfaceNumber;
        data->vid = vid;
        data->pid = pid;
        data->bus = bus;
        data->addr = addr;
        interface->user_data(data);
        for (l = 0; l < config->interface[j].altsetting[k].bNumEndpoints; ++l){
          snprintf(buf, 99, "Endpoint 0x%02hhx %s",
                   config->interface[j].altsetting[k].endpoint[l].bEndpointAddress,
                   config->interface[j].altsetting[k].
                   endpoint[l].bEndpointAddress & 0x80?
                   "device to host":"host to device");
          endpoint = t->add(interface, buf);
          data = new UsbData;
          data->type = ENDPOINT;
          data->endpoint = config->interface[j].altsetting[k].endpoint[l].bEndpointAddress;
          data->vid = vid;
          data->pid = pid;
          data->bus = bus;
          data->addr = addr;
          endpoint->user_data(data);
          uint8_t attrs = config->interface[j].altsetting[k].endpoint[l].bmAttributes;
          switch (attrs & 0x03){
          case 0:
            snprintf(buf, 99, "Control transfer. %s %s",
                     (attrs & 0x04?(attrs & 0x08?"Synchronous.":"Asynchronous."):
                      (attrs & 0x08?"Adaptive sync.":"No sync.")),
                     (attrs & 0x10?(attrs & 0x20?"WTF???":"For feedback"):
                      (attrs & 0x20?"For implicit Feedback":"For Data")));
            break;
          case 1:
            snprintf(buf, 99, "Isochronous transfer. %s %s",
                     (attrs & 0x04?(attrs & 0x08?"Synchronous.":"Asynchronous."):
                      (attrs & 0x08?"Adaptive sync.":"No sync.")),
                     (attrs & 0x10?(attrs & 0x20?"WTF???":"For feedback"):
                      (attrs & 0x20?"For implicit Feedback":"For Data")));
            break;
          case 2:
            endpoint->labelbgcolor(FL_GREEN);
            data->useThis = true;
            snprintf(buf, 99, "Bulk transfer. %s %s",
                     (attrs & 0x04?(attrs & 0x08?"Synchronous.":"Asynchronous."):
                      (attrs & 0x08?"Adaptive sync.":"No sync.")),
                     (attrs & 0x10?(attrs & 0x20?"WTF???":"For feedback"):
                      (attrs & 0x20?"For implicit Feedback":"For Data")));
            break;
          case 3:
            snprintf(buf, 99, "Interrupt transfer. %s %s",
                     (attrs & 0x04?(attrs & 0x08?"Synchronous.":"Asynchronous."):
                      (attrs & 0x08?"Adaptive sync.":"No sync.")),
                     (attrs & 0x10?(attrs & 0x20?"WTF???":"For feedback"):
                      (attrs & 0x20?"For implicit Feedback":"For Data")));
            break;
          }
          t->add(endpoint, buf);
          snprintf(buf, 99, "Max packet size %hu",
                   config->interface[j].altsetting[k].endpoint[l].wMaxPacketSize);
          t->add(endpoint, buf);
          snprintf(buf, 99, "Interval %hhu frames",
                   config->interface[j].altsetting[k].endpoint[l].bInterval);
          t->add(endpoint, buf);
          snprintf(buf, 99, "Audio refresh rate %hhu",
                   config->interface[j].altsetting[k].endpoint[l].bRefresh);
          t->add(endpoint, buf);
        }

        if (config->interface[j].altsetting[k].iInterface){
          if ((ret = libusb_get_string_descriptor_ascii(handle,
                                                        config->interface[j].
                                                        altsetting[k].iInterface,
                                                        (unsigned char *)buf,
                                                        99)) < 0){
            fl_alert("Failed to get interface name string: %s",
                     libusb_error_name(ret));
          }
          t->add(interface, buf);
        }
        snprintf(buf,99, "Alternate setting 0x%02hhx",
                 config->interface[j].altsetting[k].bAlternateSetting);
        t->add(interface, buf);
        snprintf(buf,99, "Class %s 0x%02hhx",
                 if_class(config->interface[j].altsetting[k].bInterfaceClass),
                 config->interface[j].altsetting[k].bInterfaceClass);
        t->add(interface, buf);
        snprintf(buf,99, "Subclass 0x%02hhx",
                 config->interface[j].altsetting[k].bInterfaceSubClass);
        t->add(interface, buf);
        snprintf(buf,99, "Protocol 0x%02hhx",
                 config->interface[j].altsetting[k].bInterfaceProtocol);
        t->add(interface, buf);

      }
    }
  }
  libusb_free_config_descriptor(config);
}

void initTree(Fl_Tree *t)
{
  int ret;
  libusb_device **list;
  ssize_t cnt, i;
  struct libusb_device_descriptor devdes;
  char top_level_device[60];
  Fl_Tree_Item * item;
  struct UsbData * itemData;
  libusb_device_handle *handle;
  cnt = libusb_get_device_list(NULL, &list);
  if (cnt < 0){
    fl_alert("Libusb device listing error: %s",libusb_error_name(cnt));
    return;
  }

  for (i = 0; i < cnt; ++i){
    libusb_device *device = list[i];
    if ((ret = libusb_get_device_descriptor (device, &devdes)) != 0){
      fl_alert("Libusb device descriptor error: %s",libusb_error_name(ret));
      goto ending;
    }
    snprintf(top_level_device, 59, "%03hhd:%03hhd/Device: %04hx:%04hx",
             libusb_get_bus_number(device),
             libusb_get_device_address(device),
             devdes.idVendor, devdes.idProduct);

    if ((ret = libusb_open(device, &handle)) != 0){
      strncat(top_level_device, " ",
              59 - strlen(top_level_device));
      strncat(top_level_device, libusb_error_name(ret),
              59 - strlen(top_level_device));
      item = t->add(top_level_device);
      item->parent()->close();
      item->parent()->labelbgcolor(FL_RED);
      continue;
    }
    item = t->add(top_level_device);
    if (!item)continue;

    itemData = new UsbData;
    itemData->type = DEVICE;
    itemData->bus = libusb_get_bus_number(device);
    itemData->addr = libusb_get_device_address(device);
    itemData->vid = devdes.idVendor;
    itemData->pid = devdes.idProduct;
    item->user_data(itemData);
    item->close();
    ret = strlen(top_level_device);
    strncat(top_level_device, "/description", 59 - ret);
    item = t->add(top_level_device);
    itemData = new UsbData;
    itemData->type = DESCRIPTION;
    itemData->vid = devdes.idVendor;
    itemData->pid = devdes.idProduct;
    itemData->bus = libusb_get_bus_number(device);
    itemData->addr = libusb_get_device_address(device);
    //Set the descriptor here so that there is no need to ask it any more.
    itemData->desc = &devdes;
    itemData->dev = handle;
    item->user_data(itemData);
    fillDeviceDescription(t, item);
    item->close();
    itemData->desc = 0;//This is going out of scope.
    itemData->dev = 0;//And we close the device later.

    top_level_device[ret] = 0;
    strncat(top_level_device, "/configuration", 59 - ret);
    item = t->add(top_level_device);
    itemData = new UsbData;
    itemData->type = CONF_PARENT;
    itemData->vid = devdes.idVendor;
    itemData->pid = devdes.idProduct;
    itemData->dev = handle;
    itemData->bus = libusb_get_bus_number(device);
    itemData->addr = libusb_get_device_address(device);
    item->user_data(itemData);
    int configCount = devdes.bNumConfigurations;
    fillConfigDescription(t, item, device, configCount);
    libusb_close(handle);//And close the device.
    itemData->dev = 0;
  }
  t->root_label("Universal Systematic Binding - USB");
  t->callback(cb_tree);
  //Greetings to Timo Vuorensola
 ending:
  libusb_free_device_list(list, 1);
  return;

}
static void usb_handler(void *)
{
  int ret;
  struct timeval tv;
  //1 ms timeout
  tv.tv_sec = 0;
  tv.tv_usec = 1000;
  if ((ret = libusb_handle_events_timeout_completed(0, &tv, 0)) != 0){
    fl_alert("usb handler error %s", libusb_error_name(ret));
    Fl::remove_idle(usb_handler);
  }
}
void initSystem()
{
  int ret;
  memset(&conn, 0, sizeof(conn));
  conn.rxEP = 0xff;
  conn.txEP = 0xff;
  if (!ui)ui = new UserInterface;
  if ((ret = libusb_init(0)) != 0){
    fl_alert("Libusb init error: %s",libusb_error_name(ret));
    ui->hide();
    Fl::delete_widget(ui);
    return;
  }
  // Init transfers
  if (!sendTrans){
    sendTrans = libusb_alloc_transfer(0);
    if (!sendTrans){
      fl_alert("Libusb transfer allocation failed. Closing.");
      ui->hide();
      Fl::delete_widget(ui);
      return;
    }
    sendBufSize = 1;
    sendTrans->buffer = (unsigned char *)malloc(sendBufSize);
    if (!sendTrans->buffer){
      fl_alert("Libusb transfer buffer allocation failed. Closing.");
      ui->hide();
      Fl::delete_widget(ui);
      return;
    }
    sendTrans->callback = cb_send;
  }
  if (!recvTrans){
    recvTrans = libusb_alloc_transfer(0);
    if (!recvTrans){
      fl_alert("Libusb transfer allocation failed. Closing.");
      ui->hide();
      Fl::delete_widget(ui);
      return;
    }
    recvTrans->buffer = (unsigned char *)malloc(RECV_BUF_SIZE);
    if (!recvTrans->buffer){
      fl_alert("Libusb transfer buffer allocation failed. Closing.");
      ui->hide();
      Fl::delete_widget(ui);
      return;
    }
    recvTrans->callback = cb_recv;
  }
  initTree(ui->DeviceTree);
  Fl::add_idle(usb_handler);
}
