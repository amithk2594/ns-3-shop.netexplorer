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

#include "motion.h"
#include "error.h"
#include <iostream>

Motion::Motion ()
  : m_started (false),
    m_visual (false),
    m_finished (false)
{
}

Motion::~Motion ()
{
}

void
Motion::SetVisual (bool visual)
{
  m_visual = visual;
}

void
Motion::Finish (void)
{
  m_finished = true;
}

void
Motion::Start (void)
{
  if (!m_started)
    {
      if (m_finished)
        {
          throw Error ("Could not start finished motion");
        }

      m_started = true;
      m_signalState.emit (this);
    }
}

void
Motion::Stop (void)
{
  if (m_started)
    {
      if (m_finished)
        {
          throw Error ("Could not stop finished motion");
        }

      m_started = false;
      m_signalState.emit (this);
    }
}

void
Motion::EnterFrame (uint32_t rate)
{
  m_signalEnterFrame.emit ();
}

bool
Motion::IsStarted (void) const
{
  return m_started;
}

bool
Motion::IsVisual (void) const
{
  return m_visual;
}

void
Motion::Capture (const sigc::slot<void, const Motion* > &slot)
{
  if (m_connection)
    {
      throw Error ("Could not capture motion");
    }

  m_connection = m_signalState.connect (slot);
}

void
Motion::Release (void)
{
  m_connection.disconnect ();
}

Motion::SignalMotionStateType
Motion::signal_state (void) const
{
  return m_signalState;
}

Motion::SignalEnterFrame
Motion::signal_enter_frame (void) const
{
  return m_signalEnterFrame;
}

///// StaticMotion /////
StaticMotion::StaticMotion ()
{
  SetVisual (true);
}

StaticMotion::~StaticMotion ()
{
}

void
StaticMotion::Start (void)
{
}

void
StaticMotion::Stop (void)
{
}

void
StaticMotion::EnterFrame (uint32_t rate)
{
}

///// Image motion /////

Glib::RefPtr<ImageMotion>
ImageMotion::Create ()
{
  return Glib::RefPtr<ImageMotion> (new ImageMotion ());
}

Glib::RefPtr<ImageMotion>
ImageMotion::Create (const Glib::RefPtr<Gdk::Pixbuf> &pixbuf)
{
  return Glib::RefPtr<ImageMotion> (new ImageMotion (pixbuf));
}

ImageMotion::ImageMotion ()
  : m_x (0),
    m_y (0)
{
}

ImageMotion::ImageMotion (const Glib::RefPtr<Gdk::Pixbuf> &pixbuf)
  : StaticMotion (),
    m_x (0),
    m_y (0)
{
  SetPixbuf (pixbuf);
}

ImageMotion::~ImageMotion ()
{
}

void
ImageMotion::SetPixbuf (const Glib::RefPtr<Gdk::Pixbuf> &pixbuf)
{
  Cairo::Format format = Cairo::FORMAT_RGB24;
  if (pixbuf->get_has_alpha())
    {
      format = Cairo::FORMAT_ARGB32;
    }

  m_width = pixbuf->get_width();
  m_height = pixbuf->get_height();

  m_surface = Cairo::ImageSurface::create (format, m_width, m_height);
  Cairo::RefPtr<Cairo::Context> context = Cairo::Context::create (m_surface);

  Gdk::Cairo::set_source_pixbuf (context, pixbuf, 0.0, 0.0);
  context->paint();
}

void
ImageMotion::SetPosition (double x, double y)
{
  m_x = 0;
  m_y = 0;
}

void
ImageMotion::DrawFrame (const Cairo::RefPtr<Cairo::Context> &context)
{
  context->save ();
  context->set_identity_matrix ();
  context->set_source (m_surface, m_x, m_y);
  context->rectangle (m_x, m_y, m_width, m_height);
  context->clip ();
  context->paint ();
  context->restore ();
}

//// Animation ////

Animation::Animation (sigc::slot<double, double> t, double duration)
  : m_duration (duration),
    m_current (0),
    m_transition (t)
{
  SetVisual (false);
  Start ();
}

Animation::Animation (sigc::slot<double, double> t, double duration, const sigc::slot<void, double> &slot)
  : m_duration (duration),
    m_current (0),
    m_slot (slot),
    m_transition (t)
{
  SetVisual (false);
  Start ();
}

Animation::~Animation ()
{
}

void
Animation::EnterFrame (uint32_t rate)
{
  m_current += 1.0 / rate;

  if (m_current >= m_duration)
    {
      m_slot (m_transition (1.0));
      Finish ();
    }
  else
    {
      m_slot (m_transition (m_current / m_duration));
    }
}

void
Animation::SetSlot (const sigc::slot<void, double> &slot)
{
  m_slot = slot;
}

void
Animation::DrawFrame (const Cairo::RefPtr<Cairo::Context> &context)
{
}
