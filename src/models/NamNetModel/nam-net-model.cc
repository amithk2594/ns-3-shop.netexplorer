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

#include <fstream>
#include <iostream>
#include <iomanip>

#include "gtk-util.h"
#include "algorithm.h"
#include "nam-images.h"
#include "nam-net-motion.h"
#include "nam-net-model.h"
#include "nam-net-model.ui"


ENSURE_REGISTER_MODEL (NamNetModel);

ModelTypeId
NamNetModel::GetModelTypeId (void)
{
  static ModelTypeId tid = ModelTypeId (
    "NamNetModel",
    sigc::ptr_fun (&NamNetModel::Factory),
    "NetAnim model",
    "text/plain",
    "*.nam",
    true,
    false
  );
  return tid;
}

NetModel*
NamNetModel::Factory (void)
{
  return new NamNetModel ();
}

NamNetModel::NamNetModel ()
  : NetModel (ui::NamNetModel)
{
  m_motion = NamNetMotion::Create ();
  m_moveMotion = ImageMotion::Create (Gdk::Pixbuf::create_from_inline (48*48*4 + 24, images::move_image));
  m_rotateMotion = ImageMotion::Create (Gdk::Pixbuf::create_from_inline (48*48*4 + 24, images::rotate_image));
}

NamNetModel::~NamNetModel ()
{
}

void
NamNetModel::Initialize (void)
{
  Glib::RefPtr<Gtk::ActionGroup> group = GetActionGroup ();
  group->add (Gtk::Action::create ("Play", Gtk::Stock::MEDIA_PLAY), sigc::mem_fun (*this, &NamNetModel::HandlePlay));
  group->add (Gtk::Action::create ("Pause", Gtk::Stock::MEDIA_PAUSE), sigc::mem_fun (*this, &NamNetModel::HandlePause));
  group->add (Gtk::Action::create ("Stop", Gtk::Stock::MEDIA_STOP), sigc::mem_fun (*this, &NamNetModel::HandleStop));
  group->add (Gtk::Action::create ("Rewind", Gtk::Stock::MEDIA_REWIND), sigc::mem_fun (*this, &NamNetModel::HandleRewind));
  group->add (Gtk::Action::create ("Forward", Gtk::Stock::MEDIA_FORWARD), sigc::mem_fun (*this, &NamNetModel::HandleForward));

  m_scene.signal_zoom_change ().connect (sigc::mem_fun (*this, &NamNetModel::HandleZoomChange));
  m_scale.signal_change_value ().connect (sigc::mem_fun (*this, &NamNetModel::HandleScaleChange));
  m_scale.signal_button_press_event ().connect (sigc::mem_fun (*this, &NamNetModel::HandleSliderMovingStart), false);
  m_scale.signal_button_release_event ().connect (sigc::mem_fun (*this, &NamNetModel::HandleSliderMovingEnd), false);
  m_zoomCombo.signal_changed ().connect (sigc::mem_fun (*this, &NamNetModel::HandleZoomChanged));
  m_speedScale.signal_value_changed ().connect (sigc::mem_fun (*this, &NamNetModel::HandleSpeedChanged));

  m_speedLabel.set_width_chars (6);

  Gtk::Alignment* alignment;
  Gtk::HBox* topBox = Gtk::manage (new Gtk::HBox ());
  Gtk::HBox* bottomBox = Gtk::manage (new Gtk::HBox ());
  Gtk::Toolbar* toolbar = GetUI<Gtk::Toolbar> ("/Tool");
  Gtk::MenuBar* menubar = GetUI<Gtk::MenuBar> ("/Menu");

  // top box
  CreateSpeedVector ();

  m_zoomCombo.set_model (CreateZoomList ());
  m_zoomCombo.set_text_column (0);
  m_zoomCombo.remove ();
  m_zoomCombo.add (m_zoomEntry);
  m_zoomCombo.show_all_children ();
  m_zoomEntry.set_editable (false);
  m_zoomEntry.set_alignment (1.0);
  m_zoomEntry.set_width_chars (5);
  m_zoomCombo.set_active (3);

  topBox->pack_start (*Gtk::manage (new Gtk::Label ("Zoom:")), Gtk::PACK_SHRINK, 2);
  topBox->pack_start (m_zoomCombo, Gtk::PACK_SHRINK, 2);
  topBox->pack_start (*Gtk::manage (new Gtk::VSeparator ()), Gtk::PACK_SHRINK, 4);

  topBox->pack_start (*Gtk::manage (new Gtk::Label ("Interval:")), Gtk::PACK_SHRINK, 2);
  topBox->pack_start (m_speedLabel, Gtk::PACK_SHRINK, 2);
  topBox->pack_start (m_speedScale, Gtk::PACK_EXPAND_WIDGET, 6);
  topBox->pack_start (*Gtk::manage (new Gtk::VSeparator ()), Gtk::PACK_SHRINK, 4);

  topBox->pack_start (*Gtk::manage (new Gtk::Label ("Time:")), Gtk::PACK_SHRINK, 2);
  topBox->pack_start (m_timeLabel, Gtk::PACK_SHRINK, 2);

  // bottom box
  toolbar->set_show_arrow (false);
  m_scale.set_draw_value (false);
  m_speedScale.set_draw_value (false);
  m_speedScale.set_range (0, 20);
  m_speedScale.set_increments (1, 2);
  m_speedScale.set_value (10);
  bottomBox->pack_start (*toolbar, Gtk::PACK_SHRINK);
  bottomBox->pack_start (m_scale, Gtk::PACK_EXPAND_WIDGET, 6);

  // base layout
  pack_start (*menubar, Gtk::PACK_SHRINK);
  pack_start (*GtkUtil::Alignment (*topBox, 6), Gtk::PACK_SHRINK);
  pack_start (*GtkUtil::Alignment (m_scene, 0, 6));
  pack_start (*bottomBox, Gtk::PACK_SHRINK);

  show_all_children ();

  m_scene.AddMotion (m_motion);

  m_motion->signal_enter_frame ().connect (sigc::mem_fun (*this, &NamNetModel::HandleMotion));
  m_motionStateConnection = m_motion->signal_state ().connect (sigc::mem_fun (*this, &NamNetModel::HandleMotionState));

  m_scene.signal_move_motion ().connect (sigc::mem_fun (*this, &NamNetModel::HandleSceneMove));
  m_scene.signal_rotate_motion ().connect (sigc::mem_fun (*this, &NamNetModel::HandleSceneRotate));

  HandleSpeedChanged ();
  HandleStop ();
}

Glib::RefPtr<Gtk::ListStore>
NamNetModel::CreateZoomList (void)
{
  Glib::RefPtr<Gtk::ListStore> list = Gtk::ListStore::create (m_stringDoubleModel);

  m_stringDoubleModel.Append (list, "800%", 8.0);
  m_stringDoubleModel.Append (list, "400%", 4.0);
  m_stringDoubleModel.Append (list, "200%", 2.0);
  m_stringDoubleModel.Append (list, "100%", 1.0);
  m_stringDoubleModel.Append (list, "50%",  0.5);
  m_stringDoubleModel.Append (list, "25%",  0.25);
  m_stringDoubleModel.Append (list, "12%",  0.12);

  return list;
}

void
NamNetModel::Reset (void)
{
  if (!m_scene.is_mapped ())
    {
      m_allocConnection = m_scene.signal_size_allocate ().connect (sigc::mem_fun (*this, &NamNetModel::HandleSceneAlloc));
    }
  else
    {
      HandleSetZoom ();
    }
}

void
NamNetModel::HandleSceneAlloc (Gtk::Allocation& allocation)
{
  HandleSetZoom ();
}

void
NamNetModel::HandleSetZoom (void)
{
  m_allocConnection.disconnect ();

  Rectangle viewport = m_scene.GetSceneViewport ();
  std::vector<Node> nodes = m_motion->GetNodes ();
  double zoom = algorithm::Zoom (viewport, nodes);

  m_scene.AddMotion (Animation::Create<Animation::Spring> (0.70,
      sigc::bind (sigc::mem_fun (*this, &NamNetModel::ZoomMotion), zoom)));
}

void
NamNetModel::ZoomMotion (double value, double zoom)
{
  double z = 1.0 + value * (zoom - 1.0);
  m_scene.SetZoom (z);
  SetZoom (z);
}

void
NamNetModel::CreateSpeedVector (void)
{
  m_speedVector.push_back (std::make_pair ("1ns",   0.000001));
  m_speedVector.push_back (std::make_pair ("2ns",   0.000002));
  m_speedVector.push_back (std::make_pair ("4ns",   0.000004));
  m_speedVector.push_back (std::make_pair ("8ns",   0.000008));
  m_speedVector.push_back (std::make_pair ("16ns",  0.000016));
  m_speedVector.push_back (std::make_pair ("32ns",  0.000032));
  m_speedVector.push_back (std::make_pair ("64ns",  0.000064));
  m_speedVector.push_back (std::make_pair ("125ns", 0.000125));
  m_speedVector.push_back (std::make_pair ("250ns", 0.000250));
  m_speedVector.push_back (std::make_pair ("500ns", 0.000500));
  m_speedVector.push_back (std::make_pair ("1ms",   0.001));
  m_speedVector.push_back (std::make_pair ("2ms",   0.002));
  m_speedVector.push_back (std::make_pair ("4ms",   0.004));
  m_speedVector.push_back (std::make_pair ("8ms",   0.008));
  m_speedVector.push_back (std::make_pair ("16ms",  0.016));
  m_speedVector.push_back (std::make_pair ("32ms",  0.032));
  m_speedVector.push_back (std::make_pair ("64ms",  0.064));
  m_speedVector.push_back (std::make_pair ("125ms", 0.125));
  m_speedVector.push_back (std::make_pair ("250ms", 0.250));
  m_speedVector.push_back (std::make_pair ("500ms", 0.5));
  m_speedVector.push_back (std::make_pair ("1s",    1.0));
}

void
NamNetModel::HandleSpeedChanged (void)
{
  int value = (int)m_speedScale.get_value();
  m_motion->SetMotionSpeed (m_speedVector[value].second);
  m_speedLabel.set_label (m_speedVector[value].first);
}

void
NamNetModel::SetMotionTime (double time)
{
  Glib::ustring text = Glib::ustring::format (std::fixed, std::setprecision(6), time);
  m_timeLabel.set_text (text);
}

bool
NamNetModel::HandleSceneRotate (enum Scene::MotionState state, double a1, double a2)
{
  if (state == Scene::MOTION_START)
    {
      m_scene.AddMotion (m_rotateMotion);
    }
  else if (state == Scene::MOTION_END)
    {
      m_scene.RemoveMotion (m_rotateMotion);
    }
  return false;
}

bool
NamNetModel::HandleSceneMove (enum Scene::MotionState state, const Rectangle &rect)
{
  if (state == Scene::MOTION_START)
    {
      m_scene.AddMotion (m_moveMotion);
    }
  else if (state == Scene::MOTION_END)
    {
      m_scene.RemoveMotion (m_moveMotion);
    }

  return false;
}

void
NamNetModel::HandleMotion (void)
{
  double time = m_motion->GetCurrentTime ();
  SetMotionTime (time);
  m_scale.set_value (time);
}

void
NamNetModel::HandleMotionState (const Motion *motion)
{
  if (!motion->IsStarted ())
    {
      HandleStop ();
    }
}

void
NamNetModel::HandlePlay (void)
{
  GetAction ("/Tool/Play")->set_visible (false);
  GetAction ("/Tool/Pause")->set_visible (true);
  GetAction ("/Tool/Stop")->set_sensitive (true);

  m_motionStateConnection.block ();
  m_motion->Start ();
  m_motionStateConnection.unblock ();
}

void
NamNetModel::HandlePause (void)
{
  GetAction ("/Tool/Play")->set_visible (true);
  GetAction ("/Tool/Pause")->set_visible (false);

  m_motionStateConnection.block ();
  m_motion->Stop ();
  m_motionStateConnection.unblock ();
}

void
NamNetModel::HandleStop (void)
{
  m_motionStateConnection.block ();
  m_motion->Stop ();
  m_motionStateConnection.unblock ();
  m_scale.set_value (0.0);
  m_motion->SetCurrentTime (0.0);
  SetMotionTime (0.0);

  GetAction ("/Tool/Play")->set_visible (true);
  GetAction ("/Tool/Pause")->set_visible (false);
  GetAction ("/Tool/Stop")->set_sensitive (false);

  m_scene.Invalidate ();
}

void
NamNetModel::HandleRewind (void)
{
  int value = (int)m_speedScale.get_value ();
  if (value > 0)
    {
      m_speedScale.set_value (value - 1);
    }
}

void
NamNetModel::HandleForward (void)
{
  int value = (int)m_speedScale.get_value ();
  if (value < 20)
    {
      m_speedScale.set_value (value + 1);
    }
}

bool
NamNetModel::HandleScaleChange (Gtk::ScrollType scroll, double value)
{
  m_motion->SetCurrentTime (value);
  SetMotionTime (value);
  m_scene.Invalidate ();
  return true;
}

bool
NamNetModel::HandleSliderMovingStart (GdkEventButton* event)
{
  m_scene.ForceMotion ();
  m_motionStateConnection.block ();
  m_motion->Stop ();
  m_motionStateConnection.unblock ();
  return false;
}

bool
NamNetModel::HandleSliderMovingEnd (GdkEventButton* event)
{
  m_scene.ForceMotion (false);
  if (GetAction ("/Tool/Pause")->get_visible ())
    {
      HandlePlay ();
    }
  return false;
}

void
NamNetModel::HandleZoomChanged (void)
{
  Gtk::TreeModel::iterator i = m_zoomCombo.get_active ();
  if (i)
    {
      m_scene.SetZoom ((*i)[m_stringDoubleModel.col2]);
    }
}

bool
NamNetModel::HandleZoomChange (double zoom)
{
  if (zoom < 0.05 || zoom > 16.0)
    {
      return true;
    }

  SetZoom (zoom);
  return false;
}

void
NamNetModel::SetZoom (double zoom)
{
  Glib::ustring title = Glib::ustring::format (std::fixed, std::setprecision(0), zoom * 100);
  title.append ("%");
  m_zoomEntry.set_text (title);
}

bool
NamNetModel::ReadFromStream (Glib::RefPtr<Gio::DataInputStream> stream)
{
  m_motion->LoadMotion (stream);
  m_scale.set_range (0, m_motion->GetLastTime ());
  return true;
}

bool
NamNetModel::WriteToStream (Glib::RefPtr<Gio::DataOutputStream> stream)
{
  return false;
}
