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

#ifndef MOTION_H
#define MOTION_H

#include <stdint.h>
#include <gtkmm.h>

/**
 * \brief Motion base class
 */
class Motion : public Glib::Object
{
public:
  virtual ~Motion ();
  /**
   * \brief start animation
   */
  virtual void Start (void);
  /**
   * \brief stop animation
   */
  virtual void Stop (void);
  /**
   * \returns true if motion started
   */
  bool IsStarted (void) const;
  /**
   * \returns true if motion is visual
   */
  bool IsVisual (void) const;
  /**
   * \returns true if motion is visual
   */
  bool IsFinished (void) const;
  /**
   * \brief new frame
   */
  virtual void EnterFrame (uint32_t rate);
  /**
   * \brief redraw frame
   */
  virtual void DrawFrame (const Cairo::RefPtr<Cairo::Context> &context) = 0;

  typedef sigc::signal<void, const Motion* > SignalMotionStateType;
  typedef sigc::signal<void> SignalEnterFrame;

  SignalMotionStateType signal_state (void) const;
  SignalEnterFrame signal_enter_frame (void) const;

protected:
  Motion ();
  /**
   * \param visual
   */
  void SetVisual (bool visual = true);
  /**
   * \brief finish animataion
   */
  void Finish (void);

private:
  void Capture (const sigc::slot<void, const Motion* > &slot);
  void Release (void);

private:
  bool m_started;
  bool m_finished;
  bool m_visual;

  sigc::connection m_connection;
  SignalMotionStateType m_signalState;
  SignalEnterFrame m_signalEnterFrame;

  friend class MotionManager;
};

/**
 * \brief Static motion
 */
class StaticMotion : public Motion
{
public:
  virtual ~StaticMotion ();
  //functions defined in base class Motion
  virtual void Start (void);
  virtual void Stop (void);
  virtual void EnterFrame (uint32_t rate);

protected:
  StaticMotion ();
};

/**
 * \brief Static image motion
 */
class ImageMotion : public StaticMotion
{
public:
  static Glib::RefPtr<ImageMotion> Create ();
  static Glib::RefPtr<ImageMotion> Create (const Glib::RefPtr<Gdk::Pixbuf> &pixbuf);

  virtual ~ImageMotion ();
  /**
   * \brief set image
   * \param pixbuf image Pixbuf
   */
  void SetPixbuf (const Glib::RefPtr<Gdk::Pixbuf> &pixbuf);
  /**
   * \brief set image position
   * \param x
   * \param y   
   */
  void SetPosition (double x, double y);
  // functions defined in base class Motion
  virtual void DrawFrame (const Cairo::RefPtr<Cairo::Context> &context);

protected:
  ImageMotion ();
  ImageMotion (const Glib::RefPtr<Gdk::Pixbuf> &pixbuf);

private:
  double        m_x;
  double        m_y;
  uint32_t      m_width;
  uint32_t      m_height;
  Cairo::RefPtr<Cairo::ImageSurface> m_surface;
};

/**
 * \brief Simple animation 
 */
class Animation : public Motion
{
public:
  typedef double (*Transition)(double);
  
  template<Transition T>
  static Glib::RefPtr<Animation> Create (double duration)
  {
    return Glib::RefPtr<Animation> (new Animation (sigc::ptr_fun (T), duration));
  }

  template<Transition T>
  static Glib::RefPtr<Animation> Create (double duration, const sigc::slot<void, double> &slot)
  {
    return Glib::RefPtr<Animation> (new Animation (sigc::ptr_fun (T), duration, slot));
  }

  virtual ~Animation ();

  /**
   * Set animation callback function
   */
  void SetSlot (const sigc::slot<void, double> &slot);

  // functions defined in base class Motion
  virtual void EnterFrame (uint32_t rate);
  virtual void DrawFrame (const Cairo::RefPtr<Cairo::Context> &context);
  
  // transition functions
  static double Linear (double value) { return value; }
  static double Sinoidal (double value) { return -std::cos (value * M_PI) / 2.0 + .5; }
  static double Wobble (double value) { return -std::cos (value * value * M_PI * 9) / 2.0 + .5; }
  static double Reverse (double value) { return 1.0 - value; }
  static double None (double value) { return 0.0; }
  static double Full (double value) { return 1.0; }
  static double Spring (double value) { return 1.0 - std::cos (value * 4.5 * M_PI) * std::exp (-value * 6.0); }

protected:
  Animation (sigc::slot<double, double> t, double duration);
  Animation (sigc::slot<double, double> t, double duration, const sigc::slot<void, double> &slot);

private:
  void SetTransition (Transition t);
  double m_duration;
  double m_current;
  sigc::slot<void, double> m_slot;
  sigc::slot<double, double> m_transition;
};

/**
 * \brief Animation queue
 */
class AnimationQueue : public Motion
{
public:
  static Glib::RefPtr<AnimationQueue> Create (void);
  static Glib::RefPtr<AnimationQueue> Create (const Glib::RefPtr<Animation> &a1);
  static Glib::RefPtr<AnimationQueue> Create (const Glib::RefPtr<Animation> &a1, const Glib::RefPtr<Animation> &a2);
  static Glib::RefPtr<AnimationQueue> Create (const Glib::RefPtr<Animation> &a1, const Glib::RefPtr<Animation> &a2,
    const Glib::RefPtr<Animation> &a3);
  static Glib::RefPtr<AnimationQueue> Create (const Glib::RefPtr<Animation> &a1, const Glib::RefPtr<Animation> &a2,
    const Glib::RefPtr<Animation> &a3, const Glib::RefPtr<Animation> &a4);
  static Glib::RefPtr<AnimationQueue> Create (const Glib::RefPtr<Animation> &a1, const Glib::RefPtr<Animation> &a2,
    const Glib::RefPtr<Animation> &a3, const Glib::RefPtr<Animation> &a4, const Glib::RefPtr<Animation> &a5);
  virtual ~AnimationQueue ();

  void Add (const Glib::RefPtr<Animation> &anim);
  virtual void Stop (void);

  // functions defined in the base class Motion
  virtual void EnterFrame (uint32_t rate);
  virtual void DrawFrame (const Cairo::RefPtr<Cairo::Context> &context);

protected:
  AnimationQueue ();

private:
  typedef std::deque<Glib::RefPtr<Animation> > AnimationDeque;
  AnimationDeque m_animations;
};

#endif /* MOTION_H */
