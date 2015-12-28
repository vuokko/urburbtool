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
#ifndef HEXBROWSER_H
#define HEXBROWSER_H

#include <FL/Fl_Browser.H>

class Hex_Browser : public Fl_Browser
{
public:
  Hex_Browser(int X, int Y, int W, int H, const char * L = 0);
  void add(const char * buffer, int len);
  void recv_timeout();
  void recv_error(const char * err);
  void send_timeout();
  void send_error(const char * err);
  bool receiving;
};

#endif
