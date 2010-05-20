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

#include <math.h>

#include "scene.h"
#include "error.h"

Scene::Scene ()
  : m_zoom (1),
    m_rotation (0),
    m_zoomStep (0.05),
    m_scale (300.0),
    m_centerX (0),
    m_centerY (0),
    m_borderWidth (1),
    m_borderColor (0.75, 0.75, 0.75, 1),
    m_motionType (MOTION_NONE)
{
  m_manager.SetTarget (this);

  set_events (Gdk::SCROLL_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::BUTTON_MOTION_MASK);
  signal_scroll_event ().connect (sigc::mem_fun (*this, &Scene::HandleScroll));
  signal_button_press_event ().connect (sigc::mem_fun (*this, &Scene::HandleButtonPress));
  signal_button_release_event ().connect (sigc::mem_fun (*this, &Scene::HandleButtonRelease));

  signal_expose_event ().connect (sigc::mem_fun (*this, &Scene::HandleExpose));

}

Scene::~Scene ()
{
  m_manager.SetTarget (0);
}

void
Scene::SetZoom (double value)
{
  m_zoom = value;
  Invalidate ();
}

double
Scene::GetZoom (void) const
{
  return m_zoom;
}

void
Scene::SetRotation (double value)
{
  m_rotation = value;
  Invalidate ();
}

double
Scene::GetRotation (void) const
{
  return m_rotation;
}

void
Scene::SetZoomStep (double value)
{
  m_zoomStep = value;
}

double
Scene::GetZoomStep (void) const
{
  return m_zoomStep;
}

void
Scene::SetScale (double value)
{
  m_scale = value;
  Invalidate ();
}

double
Scene::GetScale (void) const
{
  return m_scale;
}

void
Scene::SetCenter (double x, double y)
{
  m_centerX = x;
  m_centerY = y;
  Invalidate ();
}

void
Scene::GetCenter (double &x, double &y) const
{
  x = m_centerX;
  y = m_centerY;
}

void
Scene::SetBorderWidth (uint32_t value)
{
  m_borderWidth = value;
  Invalidate ();
}

uint32_t
Scene::GetBorderWidth (void) const
{
  return m_borderWidth;
}

void
Scene::SetBorderColor (const RgbaColor& color)
{
  m_borderColor = color;
  Invalidate ();
}

RgbaColor
Scene::GetBorderColor (void) const
{
  return m_borderColor;
}

void
Scene::SetRate (uint32_t fps)
{
  m_manager.SetRate (fps);
}

uint32_t
Scene::GetRate (void) const
{
  return m_manager.GetRate ();
}

void
Scene::ForceMotion (bool force)
{
  m_manager.Force (force);
}

void
Scene::AddMotion (const Glib::RefPtr<Motion> &motion)
{
  m_manager.Add (motion);
}

void
Scene::RemoveMotion (const Glib::RefPtr<Motion> &motion)
{
  m_manager.Remove (motion);
}

void
Scene::Invalidate (void)
{
  m_manager.Invalidate ();
}

void
Scene::GetSceneCoords (uint32_t x, uint32_t y, double &sx, double &sy) const
{
  GetSceneCoords (m_centerX, m_centerY, x, y, sx, sy);
}

void
Scene::GetSceneCoords (double sceneX, double sceneY, uint32_t x, uint32_t y, double &sx, double &sy) const
{
  Gtk::Allocation allocation = get_allocation ();
  double scale = m_scale * m_zoom;
  sx = (x - allocation.get_width () / 2.0) / scale + sceneX;
  sy = (y - allocation.get_height () / 2.0) / scale + sceneY;
}

Rectangle
Scene::GetSceneViewport (void) const
{
  Rectangle rect;
  Gtk::Allocation allocation = get_allocation ();
  GetSceneCoords (m_centerX, m_centerY, 0, 0, rect.left, rect.top);
  GetSceneCoords (m_centerX, m_centerY, allocation.get_width (), allocation.get_height (), rect.right, rect.bottom);
  return rect;
}

void
Scene::ApplyTransformation (const Cairo::RefPtr<Cairo::Context> &context)
{
  double sx, sy;
  double scale = m_scale * m_zoom;
  Gtk::Allocation allocation = get_allocation ();

  context->scale (scale, scale);
  GetSceneCoords (m_centerX, m_centerY, allocation.get_width (), allocation.get_height (), sx, sy);
  context->translate (sx, sy);
  context->rotate (m_rotation);
}

Scene::SignalZoomChangeType
Scene::signal_zoom_change (void) const
{
  return m_signalZoomChange;
}

Scene::SignalMoveMotionType
Scene::signal_move_motion (void) const
{
  return m_signalMoveMotion;
}

Scene::SignalRotateMotionType
Scene::signal_rotate_motion (void) const
{
  return m_signalRotateMotion;
}

bool
Scene::HandleExpose (GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::Window> win = get_window ();

  if (win == 0)
    {
      return false;
    }

  Cairo::RefPtr<Cairo::Context> context = win->create_cairo_context ();

  if (event)
    {
      context->rectangle (event->area.x, event->area.y, event->area.width, event->area.height);
      context->clip();
    }

  context->save ();
  context->set_source_rgba (1.0, 1.0, 1.0, 1);
  context->paint ();

  context->save ();
  ApplyTransformation (context);
  m_manager.Process (context);
  context->restore ();

  Gtk::Allocation allocation = get_allocation ();

  context->set_line_cap (Cairo::LINE_CAP_SQUARE);
  context->set_line_width (m_borderWidth);
  context->set_source_rgba (m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a);
  context->rectangle (0, 0, allocation.get_width (), allocation.get_height ());
  context->stroke ();

  context->restore ();

  return true;
}

bool
Scene::HandleScroll (GdkEventScroll* event)
{
  double zoom;
  if (event->direction == 0)
    {
      zoom = (std::floor (m_zoom / m_zoomStep + 1E-9) + 1) * m_zoomStep;
    }
  else if (event->direction == 1)
    {
      zoom = (std::ceil (m_zoom / m_zoomStep - 1E-9) - 1) * m_zoomStep;
    }
  else
    {
      return false;
    }

  if (!m_signalZoomChange.emit (zoom))
    {
      SetZoom (zoom);
    }

  return true;
}

bool
Scene::HandleButtonPress (GdkEventButton* event)
{
  if (event->button == 1 || event->button == 3)
    {
      double x, y;
      GetSceneCoords (m_centerX, m_centerY, event->x, event->y, x, y);
      m_motionCenterX = m_centerX;
      m_motionCenterY = m_centerY;
      if (event->button == 1)
        {
          m_motionStartX = x;
          m_motionStartY = y;
          if (m_signalMoveMotion.emit (MOTION_START, Rectangle (m_motionStartX, m_motionStartY, x, y)))
            {
              return false;
            }
          m_motionType = MOTION_MOVING;
          m_motionConnection = signal_motion_notify_event ().connect (sigc::mem_fun (*this, &Scene::HandleMoving));
         }
      else
        {
          m_motionAngle = std::atan2 (y - m_centerY, x - m_centerX) - m_rotation;
          if (m_signalRotateMotion.emit (MOTION_START, m_motionAngle, m_motionAngle))
            {
              return false;
            }
          m_motionType = MOTION_ROTATION;
          m_motionConnection = signal_motion_notify_event ().connect (sigc::mem_fun (*this, &Scene::HandleRotation));
        }
      return true;
    }
  return false;
}

bool
Scene::HandleButtonRelease (GdkEventButton* event)
{
  if (event->button == 1 && m_motionType == MOTION_MOVING ||
      event->button == 3 && m_motionType == MOTION_ROTATION)
    {
      double x, y;
      GetSceneCoords (m_motionCenterX, m_motionCenterY, event->x, event->y, x, y);

      if (m_motionType == MOTION_MOVING)
        {
          m_signalMoveMotion.emit (MOTION_END, Rectangle (m_motionStartX, m_motionStartY, x, y));
        }
      else
        {
          double angle = std::atan2 (y - m_motionCenterY, x - m_motionCenterX);
          m_signalRotateMotion.emit (MOTION_END, m_motionAngle, angle);
        }

      m_motionConnection.disconnect ();
      m_motionType = MOTION_NONE;
      Invalidate ();
      return true;
    }

  return false;
}

bool
Scene::HandleMoving (GdkEventMotion* event)
{
  double x, y;
  GetSceneCoords (m_motionCenterX, m_motionCenterY, event->x, event->y, x, y);

  Rectangle rect (m_motionStartX, m_motionStartY, x, y);
  if (!m_signalMoveMotion.emit (MOTION_PROGRESS, rect))
    {
      SetCenter (m_motionCenterX + x - m_motionStartX, m_motionCenterY + y - m_motionStartY);
    }

  return true;
}

bool
Scene::HandleRotation (GdkEventMotion* event)
{
  double x, y;
  GetSceneCoords (m_motionCenterX, m_motionCenterY, event->x, event->y, x, y);
  double angle = std::atan2 (y - m_motionCenterY, x - m_motionCenterX);

  if (!m_signalRotateMotion.emit (MOTION_PROGRESS, m_motionAngle, angle))
    {
      SetRotation (angle - m_motionAngle);
    }

  return true;
}
