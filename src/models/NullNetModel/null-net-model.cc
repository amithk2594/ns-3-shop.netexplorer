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

#include <iostream>

#include "null-net-model.h"
#include "null-net-model.ui"

ENSURE_REGISTER_MODEL (NullNetModel);

ModelTypeId
NullNetModel::GetModelTypeId (void)
{
  static ModelTypeId tid = ModelTypeId (
    "NullNetModel",
    sigc::ptr_fun (&NullNetModel::Factory)
  );
  return tid;
}

NullNetModel::NullNetModel ()
  : NetModel (ui::NullNetModel)
{
}

NullNetModel::~NullNetModel ()
{
}

NetModel*
NullNetModel::Factory (void)
{
  return new NullNetModel ();
}

void
NullNetModel::Initialize (void)
{
  pack_start (*GetUI<Gtk::MenuBar> ("/Menu"), Gtk::PACK_SHRINK);
}

bool
NullNetModel::ReadFromStream (Glib::RefPtr<Gio::DataInputStream> stream)
{
  return false;
}

bool
NullNetModel::WriteToStream (Glib::RefPtr<Gio::DataOutputStream> stream)
{
  return false;
}
