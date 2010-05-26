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

#ifndef GTK_UTIL_H
#define GTK_UTIL_H

#include <gtkmm.h>
#include <stdint.h>

namespace GtkUtil {

class empty {};

Gtk::Alignment* Alignment (Gtk::Widget &widget, Gtk::AlignmentEnum xalign = Gtk::ALIGN_CENTER,
                              Gtk::AlignmentEnum yalign = Gtk::ALIGN_CENTER, float xscale=1.0, float yscale=1.0);
Gtk::Alignment* Alignment (Gtk::Widget &widget, uint32_t t, uint32_t r, uint32_t b, uint32_t l);
Gtk::Alignment* Alignment (Gtk::Widget &widget, uint32_t trbl);
Gtk::Alignment* Alignment (Gtk::Widget &widget, uint32_t tb, uint32_t rl);
Gtk::Alignment* Alignment (Gtk::Widget &widget, uint32_t t, uint32_t rl, uint32_t b);

template<class T1 = empty, class T2 = empty, class T3 = empty, class T4 = empty, class T5 = empty>
class PreventFunctor;

template<>
class PreventFunctor<empty, empty, empty, empty, empty> : public sigc::functor_base
{
public:
  PreventFunctor () {}
  bool operator() () const { return true; }
};

template<class T1>
class PreventFunctor<T1, empty, empty, empty, empty> : public sigc::functor_base
{
public:
  PreventFunctor () {}
  bool operator() (T1) const { return true; }
};

template<class T1, class T2>
class PreventFunctor<T1, T2, empty, empty, empty> : public sigc::functor_base
{
public:
  PreventFunctor () {}
  bool operator() (T1, T2) const { return true; }
};

template<class T1, class T2, class T3>
class PreventFunctor<T1, T2, T3, empty, empty> : public sigc::functor_base
{
public:
  PreventFunctor () {}
  bool operator() (T1, T2, T3) const { return true; }
};

template<class T1, class T2, class T3, class T4>
class PreventFunctor<T1, T2, T3, T4, empty> : public sigc::functor_base
{
public:
  PreventFunctor () {}
  bool operator() (T1, T2, T3, T4) const { return true; }
};

template<class T1, class T2, class T3, class T4, class T5>
class PreventFunctor : public sigc::functor_base
{
public:
  PreventFunctor () {}
  bool operator() (T1, T2, T3, T4, T5) const { return true; }
};

} // namespace GtkUtil;

#endif /* GTK_UTIL_H */
