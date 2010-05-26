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

#ifndef TREE_MODELS_H
#define TREE_MODELS_H

#include <gtkmm.h>
#include "gtk-util.h"

using namespace GtkUtil;

template <typename T1, typename T2 = empty, typename T3 = empty>
class ColumnModel;

template <typename T1>
class ColumnModel<T1, empty, empty> : public Gtk::TreeModel::ColumnRecord
{
public:
  ColumnModel () { add (col1); }
  const Gtk::TreeModel::iterator Append (Glib::RefPtr<Gtk::ListStore> list, const T1 &value1)
  {
    Gtk::TreeModel::iterator i = list->append();
    (*i)[col1] = value1;
    return i;
  }

public:
  Gtk::TreeModelColumn<T1> col1;
};

template <typename T1, typename T2>
class ColumnModel<T1, T2, empty> : public Gtk::TreeModel::ColumnRecord
{
public:
  ColumnModel () { add (col1); add (col2);}
  const Gtk::TreeModel::iterator Append (Glib::RefPtr<Gtk::ListStore> list, const T1 &value1, const T2 &value2)
  {
    Gtk::TreeModel::iterator i = list->append();
    (*i)[col1] = value1;
    (*i)[col2] = value2;
    return i;
  }

public:
  Gtk::TreeModelColumn<T1> col1;
  Gtk::TreeModelColumn<T2> col2;
};

template <typename T1, typename T2, typename T3>
class ColumnModel : public Gtk::TreeModel::ColumnRecord
{
public:
  ColumnModel () { add (col1); add (col2); add (col3);}
  const Gtk::TreeModel::iterator Append (Glib::RefPtr<Gtk::ListStore> list, const T1 &value1, const T2 &value2, const T3 &value3)
  {
    Gtk::TreeModel::iterator i = list->append();
    (*i)[col1] = value1;
    (*i)[col2] = value2;
    (*i)[col3] = value3;
    return i;
  }

public:
  Gtk::TreeModelColumn<T1> col1;
  Gtk::TreeModelColumn<T2> col2;
  Gtk::TreeModelColumn<T3> col3;
};

#endif /* TREE_MODELS_H */
