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

#ifndef MOTION_MANAGER_H
#define MOTION_MANAGER_H

#include <stdint.h>
#include <gtkmm.h>

#include "motion.h"
#include "common.h"

class Scene;

class MotionManager
{
public:
  MotionManager ();
  virtual ~MotionManager ();
  /**
   * \param scene
   */
  void SetTarget (Gtk::DrawingArea* target);
  /**
   * \param fps
   */
  void SetRate (uint32_t fps);
  /**
   * \returns fps
   */
  uint32_t GetRate (void) const;
  /**
   * \param force
   */
  void Force (bool force = true);
  /**
   * \param motion
   */
  void Add (const Glib::RefPtr<Motion> &motion);
  /**
   * \param motion
   */
  void Remove (const Glib::RefPtr<Motion> &motion);
  /**
   * \brief invalidate scene
   */
  void Invalidate (void);
  /**
   * \brief process all motions
   */
  void Process (const Cairo::RefPtr<Cairo::Context> &context);

private:
  void HandleInvalidate (void);
  void HandleMotionState (const Motion *motion);
  void HandleMotionStart ();
  void HandleMotionStop (bool invalidate = true);
  bool HandleMotionTimer (void);

  typedef std::list<Glib::RefPtr<Motion> > MotionList;

  Gtk::DrawingArea* m_target;
  uint32_t      m_currFrame;
  uint32_t      m_prevFrame;
  uint32_t      m_rate;
  bool          m_forced;
  MotionList    m_motions;
  uint32_t      m_motionCount;
  sigc::connection m_motionTimer;
};

#endif /* MOTION_MANAGER_H */
