/***************************************************************************
*   Copyright (C) 2005 by xihe         *
*   xihels@163.com          *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#ifndef _PIXMAPS_H_
#define _PIXMAPS_H_
#include <gdkmm/pixbuf.h>
#include <gtkmm/image.h>

Gtk::Image* getSmil(const char* filename);
Gtk::Image* getImage(const char* filename);
Glib::RefPtr<Gdk::Pixbuf> getPix(const char* filename);
Glib::RefPtr<Gdk::Pixbuf> getPix16(const char* filename);
Glib::RefPtr<Gdk::Pixbuf> getPix30(const char* filename);


#endif // _PIXMAPS_H_
