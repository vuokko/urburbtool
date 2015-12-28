Urb Urb Tool
============

Send and receive USB block messages

The itch
--------

While developing driver for oscilloscope, I wanted to send my commands
interactively and just test and beat the features out of the device.

The scratch
-----------

This program uses FLTK to make its UI, libusb to communicate. All civilized
distributions should have them. For compilation XXD might be required to create
images.cxx file.

Compile with (GNU)make and run with ./urburbtool No install targets provided.

Etymology
---------

Wireshark report these packet as URB_Bulk_in and out. URB URB tool was
written to get some contol over URBbing devices.

Known bugs
----------

* The horizontal scrollbar sucks with FLTK 1.3.3. SVN version works.

License
-------

GPL v3+