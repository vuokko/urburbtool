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
#include "hexbrowser.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <FL/Fl_PNG_Image.H>

#define ADDR_LEN 4
#define BYTE_COUNT 16

Fl_PNG_Image * sent = 0, * received = 0, * timeout = 0, * error = 0;
extern unsigned char sent_png[];
extern unsigned int sent_png_len;
extern unsigned char received_png[];
extern unsigned int received_png_len;
extern unsigned char error_png[];
extern unsigned int error_png_len;
extern unsigned char timeout_png[];
extern unsigned int timeout_png_len;

Hex_Browser::Hex_Browser(int X, int Y, int W, int H, const char * L):
  Fl_Browser(X,Y,W,H,L)
{
  if (!sent){
    sent = new Fl_PNG_Image(NULL, sent_png, sent_png_len);
  }
  if (!received){
    received = new Fl_PNG_Image(NULL, received_png, received_png_len);
  }
  if (!error){
    error = new Fl_PNG_Image(NULL, error_png, error_png_len);
  }
  if (!timeout){
    timeout = new Fl_PNG_Image(NULL, timeout_png, timeout_png_len);
  }
}
void Hex_Browser::add(const char * buffer, int len)
{
  static char output[ADDR_LEN + 1 +
                     (BYTE_COUNT *3) +
                     18 +
                     BYTE_COUNT]; //Text presentation.
  static char address[ADDR_LEN + 2];
  static char hex[5];
  static char text[BYTE_COUNT + 1];
  int i, n;

  n = 0;
  i = 0;
  output[0] = 0;
  text[0] = 0;
  while (n < len){
    i = n % BYTE_COUNT;
    if (i == 0){
      snprintf(address, ADDR_LEN + 2, "%04x ",n);
      output[0] = 0;
      strncat(output, receiving?"@t@C0@B7@.":"@t@C7@B0@.", sizeof(output) -
              strlen(output) - 1);
      strncat(output,address, sizeof(output) - strlen(output) - 1);
      strncat(output,"  ", sizeof(output) - strlen(output) - 1);
    }
    snprintf(hex, 4,"%02hhx ", buffer[n]);
    strncat(output,hex,sizeof(output) - strlen(output) - 1);
    if (i == 8){
      strncat(output," ",sizeof(output) - strlen(output) - 1);
      text[i] = ' ';
    }
    if (isprint(buffer[n]))
      text[i < 8 ? i : i + 1] = buffer[n];
    else
      text[i < 8 ? i : i + 1] = '.';
    text[i < 8 ? i + 1 : i + 2] = 0;
    if (i == BYTE_COUNT -1){
      strncat(output,"  ",sizeof(output) - strlen(output) - 1);
      strncat(output,text,sizeof(output) - strlen(output) -1);
      Fl_Browser::add(output);
      icon(lineno(item_last()), receiving?received:sent);
    }
    n++;
  }
  if (i != BYTE_COUNT - 1){
    while (i < BYTE_COUNT -1){
      strncat(output,"   ",sizeof(output) - strlen(output) - 1);
      i++;
    }
    strncat(output,"  ",sizeof(output) - strlen(output) - 1);
    strncat(output,text,sizeof(output) - strlen(output) -1);
    Fl_Browser::add(output);
    icon(lineno(item_last()), receiving?received:sent);
  }
  Fl_Browser::add("");
}
void Hex_Browser::recv_timeout()
{
  int line = lineno(item_last());
  while(line > 0){
    if (icon(line) == received){
      icon(++line, timeout);
      return;
    }
    line--;
  }
}
void Hex_Browser::recv_error(const char * err)
{
  int line = lineno(item_last());
  while(line > 0){
    if (icon(line) == received){
      icon(++line, error);
      if(err)
        text(line, err);
      return;
    }
    line--;
  }
}
void Hex_Browser::send_timeout()
{
  int line = lineno(item_last());
  while(line > 0){
    if (icon(line) == sent){
      icon(++line, timeout);
      return;
    }
    line--;
  }
}
void Hex_Browser::send_error(const char * err)
{
  int line = lineno(item_last());
  while(line > 0){
    if (icon(line) == sent){
      icon(++line, error);
      if(err)
        text(line, err);
      return;
    }
    line--;
  }
}
