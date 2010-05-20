/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrey Churin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Andrey Churin <aachurin@gmail.com>
 */

#include "gtk-util.h"

namespace GtkUtil {

Gtk::Alignment*
Alignment (Gtk::Widget &widget, Gtk::AlignmentEnum xalign, Gtk::AlignmentEnum yalign, float xscale, float yscale)
{
  Gtk::Alignment *alignment = Gtk::manage (new Gtk::Alignment (xalign, yalign, xscale, yscale));
  alignment->add (widget);
  return alignment;
}

Gtk::Alignment*
Alignment (Gtk::Widget &widget, uint32_t t, uint32_t r, uint32_t b, uint32_t l)
{
  Gtk::Alignment *alignment = Gtk::manage (new Gtk::Alignment ());
  alignment->add (widget);
  alignment->set_padding (t, b, l, r);
  return alignment;
}

Gtk::Alignment*
Alignment (Gtk::Widget &widget, uint32_t trbl)
{
  return GtkUtil::Alignment (widget, trbl, trbl, trbl, trbl);
}

Gtk::Alignment*
Alignment (Gtk::Widget &widget, uint32_t tb, uint32_t rl)
{
  return GtkUtil::Alignment (widget, tb, rl, tb, rl);
}

Gtk::Alignment*
Alignment (Gtk::Widget &widget, uint32_t t, uint32_t rl, uint32_t b)
{
  return GtkUtil::Alignment (widget, t, rl, b, rl);
}

} //namespace GtkUtil
