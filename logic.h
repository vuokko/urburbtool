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
#ifndef LOGIC_H
#define LOGIC_H

#include <FL/Fl_Tree.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Browser.H>

#include "hexbrowser.h"

extern UserInterface * ui;

void initSystem();
void initTree(Fl_Tree *t);
void cb_SendInput(Fl_Widget* w, void * d);
void cb_button(Fl_Widget* w, void * d);
#endif
