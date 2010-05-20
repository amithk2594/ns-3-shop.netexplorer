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

#ifndef NULL_NET_MODEL_H
#define NULL_NET_MODEL_H

#include <gtkmm.h>
#include "net-model.h"

class NullNetModel: public NetModel
{
public:
  NullNetModel ();
  virtual ~NullNetModel ();

  static ModelTypeId GetModelTypeId (void);
  static NetModel* Factory (void);

  // Functions defined in base class NetModel
  virtual void Initialize (void);
  virtual bool ReadFromStream (Glib::RefPtr<Gio::DataInputStream> stream);
  virtual bool WriteToStream (Glib::RefPtr<Gio::DataOutputStream> stream);
};

#endif /* NAM_NET_MODEL_H */
