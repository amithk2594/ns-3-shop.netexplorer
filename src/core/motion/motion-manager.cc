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

#include "motion-manager.h"
#include "error.h"

MotionManager::MotionManager ()
  : m_target (0),
    m_currFrame (1),
    m_prevFrame (0),
    m_motionCount (0),
    m_rate (25)
{
}

MotionManager::~MotionManager ()
{
  for (MotionList::iterator i = m_motions.begin (); i != m_motions.end (); ++i)
    {
      (*i)->Release ();
    }

  m_motions.clear ();
}

void
MotionManager::SetTarget (Gtk::DrawingArea* target)
{
  m_target = target;
}

void
MotionManager::SetRate (uint32_t fps)
{
  m_rate = fps;
}

uint32_t
MotionManager::GetRate (void) const
{
  return m_rate;
}

void
MotionManager::Force (bool force)
{
  m_forced = force;

  if (force)
    {
      if (!m_motionTimer)
        {
          m_motionTimer = Glib::signal_timeout().connect (sigc::mem_fun (*this, &MotionManager::HandleMotionTimer), 1000 / m_rate);
        }
    }
  else if (m_motionCount == 0)
    {
      m_motionTimer.disconnect ();
    }
}

void
MotionManager::Add (const Glib::RefPtr<Motion> &motion)
{
  if (motion->m_finished)
    {
      throw Error ("Could not add finished motion");
    }

  motion->Capture (sigc::mem_fun(*this, &MotionManager::HandleMotionState));
  m_motions.push_back (motion);

  if (motion->m_started)
    {
      HandleMotionStart ();
    }
  Invalidate ();
}

void
MotionManager::Remove (const Glib::RefPtr<Motion> &motion)
{
  for (MotionList::iterator i = m_motions.begin (); i != m_motions.end (); ++i)
    {
      if ((*i) == motion)
        {
          motion->Release ();
          if (motion->m_started)
            {
              HandleMotionStop ();
            }
          m_motions.erase (i);
          break;
        }
    }

  Invalidate ();
}

void
MotionManager::HandleMotionStart ()
{
  m_motionCount++;
  if (!m_motionTimer)
    {
      m_motionTimer = Glib::signal_timeout().connect (sigc::mem_fun (*this, &MotionManager::HandleMotionTimer), 1000 / m_rate);
    }
}

void
MotionManager::HandleMotionStop (bool invalidate)
{
  m_motionCount--;
  if (m_motionCount == 0 && !m_forced)
    {
      m_motionTimer.disconnect ();
      if (invalidate)
        {
          Invalidate ();
        }
    }
}

void
MotionManager::HandleMotionState (const Motion* motion)
{
  if (motion->m_started)
    {
      HandleMotionStart ();
    }
  else
    {
      HandleMotionStop ();
    }
}

bool
MotionManager::HandleMotionTimer (void)
{
  if (m_motionCount > 0)
    {
      m_currFrame++;
    }

  HandleInvalidate ();
  return true;
}

void
MotionManager::HandleInvalidate ()
{
  if (m_target == 0)
    {
      return;
    }

  Glib::RefPtr<Gdk::Window> win = m_target->get_window ();

  if (win != 0)
    {
      Gtk::Allocation allocation = m_target->get_allocation ();
      Gdk::Rectangle rect (0, 0, allocation.get_width(), allocation.get_height());
      win->invalidate_rect (rect, false);
    }
}

void
MotionManager::Invalidate (void)
{
  if (!m_motionTimer)
    {
      HandleInvalidate ();
    }
}

void
MotionManager::Process (const Cairo::RefPtr<Cairo::Context> &context)
{
  MotionList::iterator i = m_motions.begin ();

  while (i != m_motions.end ())
    {
      const Glib::RefPtr<Motion> motion = *i;

      if (motion->m_started && m_currFrame != m_prevFrame)
        {
          motion->EnterFrame (m_rate);
        }

      if (motion->m_visual)
        {
          context->save ();
          motion->DrawFrame (context);
          context->restore ();
        }

      if (motion->m_finished)
        {
          motion->Release ();
          if (motion->m_started)
            {
              HandleMotionStop (false);
            }

          i = m_motions.erase (i);
        }
      else
        {
          ++i;
        }
    }

  m_prevFrame = m_currFrame;
}
