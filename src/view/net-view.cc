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

#include <gtkmm/stock.h>
#include <iostream>
#include <string>

#include "net-view.h"
#include "null-net-model.h"

NetView::NetView ()
  : m_model (0)
{
  set_default_size (700, 700);
  set_title ("Network Explorer");

  InitializeModel (ModelFactory::Create ("NullNetModel"));
}

NetView::~NetView ()
{
}

void
NetView::InitializeModel (NetModel *model)
{
//  if (m_model->GetModelTypeId () == model->GetModelTypeId ())
//    {

//    }

  if (m_model != 0)
    {
      remove_accel_group (m_model->GetAccelGroup ());
      remove ();
      delete m_model;
    }

  m_model = model;

  Glib::RefPtr<Gtk::ActionGroup> group = m_model->GetActionGroup ();
  group->add (Gtk::Action::create ("FileMenu", "_File"));
  group->add (Gtk::Action::create ("FileNew", Gtk::Stock::NEW));
  group->add (Gtk::Action::create ("FileOpen", Gtk::Stock::OPEN), sigc::mem_fun (*this, &NetView::HandleFileOpen));
  group->add (Gtk::Action::create ("FileClose", Gtk::Stock::CLOSE), sigc::mem_fun (*this, &NetView::HandleFileClose));
  group->add (Gtk::Action::create ("FileQuit", Gtk::Stock::QUIT), sigc::mem_fun (*this, &NetView::HandleFileQuit));
  group->add (Gtk::Action::create ("HelpMenu", "_Help"));
  group->add (Gtk::Action::create ("HelpAbout", Gtk::Stock::ABOUT));

  m_model->Initialize ();
  m_model->Reset ();
  add (*m_model);
  add_accel_group (m_model->GetAccelGroup ());

  show_all_children (false);
}

void
NetView::HandleFileOpen ()
{
  Gtk::FileChooserDialog dialog ("Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for (*this);

  dialog.add_button (Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button (Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  for (ModelFactory::Iterator i = ModelFactory::Begin (); i != ModelFactory::End (); ++i)
    {
      if ((*i).second.IsReadable ())
        {
          Gtk::FileFilter filter;
          filter.set_name ((*i).second.GetDescription ());
          filter.add_pattern ((*i).second.GetPattern ());
          dialog.add_filter (filter);
        }
    }

  int result = dialog.run();

  if (result != Gtk::RESPONSE_OK)
    {
      return;
    }

  std::string filename = dialog.get_filename ();

  for (ModelFactory::Iterator i = ModelFactory::Begin (); i != ModelFactory::End (); ++i)
    {
      if ((*i).second.IsReadable ())
        {
          Glib::PatternSpec pattern ((*i).second.GetPattern ());
          if (pattern.match (filename))
            {
              Glib::RefPtr<Gio::FileInputStream> fileStream = dialog.get_file ()->read ();
              Glib::RefPtr<Gio::DataInputStream> dataStream = Gio::DataInputStream::create (fileStream);
              NetModel* model = (*i).second.Create ();
              if (!model->ReadFromStream (dataStream))
                {
                  // XXX: some error message
                  delete model;
                  return;
                }

              InitializeModel (model);
              return;
            }
        }
    }
}

void
NetView::HandleFileClose ()
{
  InitializeModel (ModelFactory::Create ("NullNetModel"));
}

void
NetView::HandleFileQuit ()
{
  hide ();
}

void
NetView::HandleHelpAbout ()
{
}
