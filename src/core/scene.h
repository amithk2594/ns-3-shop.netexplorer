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

#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>
#include <gtkmm.h>

#include "common.h"
#include "motion.h"
#include "motion-manager.h"

class Scene: public Gtk::DrawingArea
{
public:
  enum MotionState
  {
    MOTION_START,
    MOTION_PROGRESS,
    MOTION_END
  };

  Scene ();
  virtual ~Scene ();

  /**
   * \param value zoom value
   */
  void SetZoom (double value);
  /**
   * \returns current zoom
   */
  double GetZoom (void) const;
  /**
   * \param value scene rotation
   */
  void SetRotation (double value);
  /**
   * \returns current rotation
   */
  double GetRotation (void) const;
  /**
   * \param x
   * \param y
   * Scene center
   */
  void SetCenter (double x, double y);
  /**
   * \param &x
   * \param &y
   * Returns scene center
   */
  void GetCenter (double &x, double &y) const;
  /**
   * \param value zoom value
   */
  void SetZoomStep (double value);
  /**
   * \returns current zoom
   */
  double GetZoomStep (void) const;
  /**
   * \param value scene scale
   */
  void SetScale (double value);
  /**
   * \returns current scene scale
   */
  double GetScale (void) const;
  /**
   * \param value border width
   */
  void SetBorderWidth (uint32_t value);
  /**
   * \returns current border width
   */
  uint32_t GetBorderWidth (void) const;
  /**
   * \param color border color
   */
  void SetBorderColor (const RgbaColor& color);
  /**
   * \returns border color
   */
  RgbaColor GetBorderColor (void) const;
  /**
   * \param x
   * \param y
   * \param &sx
   * \param &sy
   * Converts viewport coordinates to scene coordinates
   */
  void GetSceneCoords (uint32_t x, uint32_t y, double &sx, double &sy) const;
  void GetSceneCoords (double sceneX, double sceneY, uint32_t x, uint32_t y, double &sx, double &sy) const;
  /**
   * \returns scene viewport rectange
   */
  Rectangle GetSceneViewport (void) const;
  /**
   * \param context cairo context
   * Apply scene zoom and scale to context
   */
  void ApplyTransformation (const Cairo::RefPtr<Cairo::Context> &context);
  /**
   * \param fps motion rate
   */
  void SetRate (uint32_t fps);
  /**
   * \returns current motion rate
   */
  uint32_t GetRate (void) const;
  /**
   * \brief enable or disable "full" motion
   */
  void ForceMotion (bool force = true);
  /**
   * \brief invalidate scene
   */
  void Invalidate (void);
  /**
   * \param motion
   */
  void AddMotion (const Glib::RefPtr<Motion> &motion);
  /**
   * \param motion
   */
  void RemoveMotion (const Glib::RefPtr<Motion> &motion);

  typedef sigc::signal<bool, double> SignalZoomChangeType;
  typedef sigc::signal<bool, MotionState, const Rectangle&> SignalMoveMotionType;
  typedef sigc::signal<bool, MotionState, double, double> SignalRotateMotionType;

  SignalZoomChangeType signal_zoom_change (void) const;
  SignalMoveMotionType signal_move_motion (void) const;
  SignalRotateMotionType signal_rotate_motion (void) const;

private:
  SignalZoomChangeType m_signalZoomChange;
  SignalMoveMotionType m_signalMoveMotion;
  SignalRotateMotionType m_signalRotateMotion;

private:
  enum MotionType
  {
    MOTION_NONE = 0,
    MOTION_MOVING,
    MOTION_ROTATION
  };

  bool HandleExpose (GdkEventExpose* event);
  bool HandleScroll (GdkEventScroll* event);
  bool HandleButtonPress (GdkEventButton* event);
  bool HandleButtonRelease (GdkEventButton* event);
  bool HandleMoving (GdkEventMotion* event);
  bool HandleRotation (GdkEventMotion* event);

  double        m_zoom;
  double        m_zoomStep;
  double        m_rotation;
  double        m_scale;
  double        m_centerX;
  double        m_centerY;
  uint32_t      m_borderWidth;
  RgbaColor     m_borderColor;
  MotionType    m_motionType;
  double        m_motionStartX;
  double        m_motionStartY;
  double        m_motionCenterX;
  double        m_motionCenterY;
  double        m_motionAngle;
  sigc::connection m_motionConnection;
  MotionManager m_manager;
};

#endif /* SCENE_H */
