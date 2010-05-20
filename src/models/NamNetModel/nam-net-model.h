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

#ifndef NAM_NET_MODEL_H
#define NAM_NET_MODEL_H

#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <deque>
#include <string>

#include <gtkmm.h>

#include "common.h"
#include "net-model.h"
#include "tree-models.h"
#include "scene.h"
#include "nam-net-motion.h"

class NamNetModel : public NetModel
{
public:
  NamNetModel ();
  virtual ~NamNetModel ();

  static ModelTypeId GetModelTypeId (void);
  static NetModel* Factory (void);

  virtual bool ReadFromStream (Glib::RefPtr<Gio::DataInputStream> stream);
  virtual bool WriteToStream (Glib::RefPtr<Gio::DataOutputStream> stream);
  virtual void Reset (void);
  virtual void Initialize (void);

private:
  Glib::RefPtr<Gtk::ListStore> CreateZoomList (void);
  void CreateSpeedVector (void);
  void SetMotionTime (double time);
  void SetZoom (double zoom);
  void ZoomMotion (double time, double zoom);
  void HandleSceneAlloc (Gtk::Allocation& allocation);
  void HandleSetZoom (void);
  bool HandleSceneRotate (enum Scene::MotionState state, double a1, double a2);
  bool HandleSceneMove (enum Scene::MotionState state, const Rectangle &rect);
  void HandleMotion (void);
  void HandleMotionState (const Motion *motion);
  void HandlePlay (void);
  void HandlePause (void);
  void HandleStop (void);
  void HandleRewind (void);
  void HandleForward (void);
  bool HandleScaleChange (Gtk::ScrollType scroll, double value);
  bool HandleSliderMovingStart (GdkEventButton* event);
  bool HandleSliderMovingEnd (GdkEventButton* event);
  void HandleZoomChanged (void);
  bool HandleZoomChange (double zoom);
  void HandleSpeedChanged (void);

private:
  typedef ColumnModel<Glib::ustring, double> StringDoubleModel;

  Gtk::HScale     m_scale;
  Scene           m_scene;
  Gtk::Label      m_timeLabel;
  Gtk::Label      m_speedLabel;
  Gtk::Entry      m_zoomEntry;
  Gtk::HScale     m_speedScale;
  Glib::RefPtr<ImageMotion>  m_moveMotion;
  Glib::RefPtr<ImageMotion>  m_rotateMotion;
  Glib::RefPtr<NamNetMotion> m_motion;
  Glib::RefPtr<Animation> m_test;
  Gtk::ComboBoxEntry m_zoomCombo;
  StringDoubleModel m_stringDoubleModel;
  std::vector<std::pair<Glib::ustring, double> > m_speedVector;
  sigc::connection m_motionStateConnection;
  sigc::connection m_allocConnection;
};

#endif /* NAM_NET_MODEL_H */
