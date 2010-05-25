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

#include <gtkmm.h>
#include <iostream>

#include "net-view.h"

int
main(int argc, char *argv[])
{
  bool version = false;
  std::string filename;

  Glib::ustring name = "netexplorer";

  Glib::OptionGroup options (name, "NetExplorer Options");

  Glib::OptionEntry entry;
  entry.set_long_name ("version");
  entry.set_description ("Show version and exit.");
  options.add_entry (entry, version) ;

  entry.set_long_name ("filename");
  entry.set_short_name ('f');
  entry.set_description ("Load model from file.");
  options.add_entry_filename (entry, filename) ;

  Glib::OptionContext context ("-- A Network Animator for Gnome/GTK+") ;
  context.add_group (options);

  Gtk::Main kit(argc, argv, context);

  if (version)
    {
      std::cout << name << " - Version 1.1b" << std::endl;
      return 0;
    }

  NetView window;
  if (filename.size () > 0)
    {
      if (!window.LoadModel (filename))
        {
          return 0;
        }
    }

  Gtk::Main::run (window);
  return 0;
}
