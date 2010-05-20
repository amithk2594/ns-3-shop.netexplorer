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

#include <map>
#include "net-model.h"

ModelTypeId::ModelTypeId (const Glib::ustring &name, const sigc::slot<NetModel*> &factory, const Glib::ustring &descr,
  const Glib::ustring &mimeType, const Glib::ustring &pattern, bool readable, bool writable)
  : m_name (name),
    m_factory (factory),
    m_description (descr),
    m_mimeType (mimeType),
    m_pattern (pattern),
    m_readable (readable),
    m_writable (writable)
{
}

Glib::ustring
ModelTypeId::GetName (void) const
{
  return m_name;
}

NetModel*
ModelTypeId::Create (void) const
{
  return Gtk::manage (m_factory ());
}

Glib::ustring
ModelTypeId::GetDescription (void) const
{
  return m_description;
}

Glib::ustring
ModelTypeId::GetMimeType (void) const
{
  return m_mimeType;
}

Glib::ustring
ModelTypeId::GetPattern (void) const
{
  return m_pattern;
}

bool
ModelTypeId::IsReadable (void) const
{
  return m_readable;
}

bool
ModelTypeId::IsWritable (void) const
{
  return m_writable;
}

ModelTypeId::operator Glib::ustring (void) const
{
  return m_name;
}

bool
ModelTypeId::operator== (const ModelTypeId& model) const
{
  return m_name == model.m_name;
}

bool
ModelTypeId::operator!= (const ModelTypeId& model) const
{
  return m_name != model.m_name;
}

void
ModelFactory::RegisterModel (const ModelTypeId &id)
{
  GetModelMap ()->insert (std::make_pair (id, id));
}

ModelTypeId
ModelFactory::GetModel (const Glib::ustring &name)
{
  ModelFactory::Iterator i = GetModelMap ()->find (name);
  if (i == GetModelMap ()->end ())
    {
      throw ModelError ("Invalid model name");
    }

  return (*i).second;
}

NetModel*
ModelFactory::Create (const Glib::ustring &name)
{
  return GetModel (name).Create ();
}

ModelFactory::Iterator
ModelFactory::Begin (void)
{
  return GetModelMap ()->begin ();
}

ModelFactory::Iterator
ModelFactory::End (void)
{
  return GetModelMap ()->end ();
}

std::map<Glib::ustring, ModelTypeId>*
ModelFactory::GetModelMap (void)
{
  static std::map<Glib::ustring, ModelTypeId> mmap;
  return &mmap;
}

ModelError::ModelError (const Glib::ustring& message)
  : m_message (message)
{}

ModelError::~ModelError () throw ()
{}

Glib::ustring
ModelError::what () const
{
  return m_message;
}

ModelTypeId
NetModel::GetModelTypeId (void)
{
  static ModelTypeId tid = ModelTypeId (
    "NetModel"
  );
  return tid;
}

NetModel::NetModel ()
{
  InitializeModel ();
}

NetModel::NetModel (const Glib::ustring &ui)
{
  InitializeModel ();
  LoadUI (ui);
}

NetModel::~NetModel ()
{
}

void
NetModel::InitializeModel ()
{
  m_uiManager = Gtk::UIManager::create ();
  m_uiManager->insert_action_group (Gtk::ActionGroup::create ());
}

void
NetModel::Reset ()
{
}

Glib::RefPtr<Gtk::Action>
NetModel::GetAction (const Glib::ustring& path) const
{
  return m_uiManager->get_action (path);
}

Glib::RefPtr<Gtk::AccelGroup>
NetModel::GetAccelGroup (void) const
{
  return m_uiManager->get_accel_group ();
}

Glib::RefPtr<Gtk::ActionGroup>
NetModel::GetActionGroup (void) const
{
  return *(m_uiManager->get_action_groups ().begin ());
}

void
NetModel::LoadUI (const Glib::ustring &ui)
{
  m_uiManager->add_ui_from_string (ui);
}
