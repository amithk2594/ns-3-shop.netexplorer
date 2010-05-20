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

#ifndef NAM_NET_MOTION_H
#define NAM_NET_MOTION_H

#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <map>

#include <gtkmm.h>
#include "common.h"
#include "motion.h"

class NamNetMotion : public Motion
{
public:
  virtual ~NamNetMotion ();
  /**
   * \param width link width
   */
  void SetEdgeWidth (double width);
  /**
   * \returns width link width
   */
  double GetEdgeWidth (void) const;
  /**
   * \param color link color
   */
  void SetEdgeColor (const RgbaColor &color);
  /**
   * \returns link color
   */
  RgbaColor GetEdgeColor (void) const;
  /**
   * \returns width link width
   */
  void SetNodeWidth (double width);
  /**
   * \returns node width
   */
  double GetNodeWidth (void) const;
  /**
   * \param color node color
   */
  void SetNodeColor (const RgbaColor &color);
  /**
   * \returns node color
   */
  RgbaColor GetNodeColor (void) const;
  /**
   * \returns width packet width
   */
  void SetPacketWidth (double width);
  /**
   * \returns packet width
   */
  double GetPacketWidth (void) const;
  /**
   * \param color packet color
   */
  void SetPacketColor (const RgbaColor &color);
  /**
   * \returns packet color
   */
  RgbaColor GetPacketColor (void) const;
  /**
   * \param speed motion speed
   */
  void SetMotionSpeed (double speed);
  /**
   * \returns motion speed
   */
  double GetMotionSpeed (void) const;
  /**
   * \param time motion time
   */
  void SetCurrentTime (double time);
  /**
   * \returns current time
   */
  double GetCurrentTime (void) const;
  /**
   * \returns last motion time
   */
  double GetLastTime (void) const;
  /**
   * \brief load motion data
   */
  void LoadMotion (Glib::RefPtr<Gio::DataInputStream> stream);

  std::vector<Node> GetNodes (void) const;

  virtual void EnterFrame (uint32_t rate);
  virtual void DrawFrame (const Cairo::RefPtr<Cairo::Context> &context);

  static Glib::RefPtr<NamNetMotion> Create (void);
protected:
  NamNetMotion ();

private:
  typedef std::map<uint32_t, Node> NodeMap;
  typedef std::vector<Packet> PacketVector;
  typedef std::vector<Edge> EdgeVector;

  double          m_currentTime;
  double          m_lastTime;
  double          m_speed;
  double          m_edgeWidth;
  double          m_nodeWidth;
  double          m_packetWidth;
  RgbaColor       m_edgeColor;
  RgbaColor       m_nodeColor;
  RgbaColor       m_packetColor;
  NodeMap         m_nodes;
  EdgeVector      m_edges;
  PacketVector    m_packets;
  PacketVector    m_currentPackets;
  SignalEnterFrame m_signalEnterFrame;
};

#endif /* NAM_NET_MOTION_H */
