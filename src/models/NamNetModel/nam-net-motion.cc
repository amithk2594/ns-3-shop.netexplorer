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

#include "nam-net-motion.h"

NamNetMotion::NamNetMotion ()
  : m_currentTime (0),
    m_lastTime (0),
    m_speed (0),
    m_edgeWidth (0.005),
    m_nodeWidth (0.04),
    m_packetWidth (0.02),
    m_edgeColor (0.5, 0.5, 0.5, 1),
    m_nodeColor (0.1, 0.1, 0.1, 1),
    m_packetColor (0.0, 0.0, 1.0, 0.7)
{
  SetVisual (true);
}

NamNetMotion::~NamNetMotion ()
{
}

Glib::RefPtr<NamNetMotion>
NamNetMotion::Create (void)
{
  return Glib::RefPtr<NamNetMotion> (new NamNetMotion());
}

void
NamNetMotion::EnterFrame (uint32_t rate)
{
  double time = m_currentTime + m_speed / rate;

  if (time > m_lastTime)
    {
      Stop ();
      return;
    }

  m_currentTime = time;

  Motion::EnterFrame (rate);
}

void
NamNetMotion::DrawFrame (const Cairo::RefPtr<Cairo::Context> &context)
{
  // draw links
  context->save ();
  context->set_line_cap (Cairo::LINE_CAP_ROUND);
  context->set_line_width (m_edgeWidth);
  context->set_source_rgba (m_edgeColor.r, m_edgeColor.g, m_edgeColor.b, m_edgeColor.a);
  for (EdgeVector::const_iterator i = m_edges.begin (); i != m_edges.end (); ++i)
    {
      context->move_to ((*i).n1->x, (*i).n1->y);
      context->line_to ((*i).n2->x, (*i).n2->y);
    }
  context->stroke ();

  context->set_source_rgba (m_packetColor.r, m_packetColor.g, m_packetColor.b, m_packetColor.a);
  context->set_line_cap (Cairo::LINE_CAP_BUTT);
  context->set_line_width (m_packetWidth);

  for (PacketVector::iterator i = m_packets.begin(); i != m_packets.end(); ++i)
    {
      if ((*i).lbRx <= m_currentTime) continue; // In the past
      if ((*i).fbTx > m_currentTime) continue;  // In the future

      Packet pkt = *i;

      context->save ();
      const Edge *edge = pkt.edge;
      if (pkt.direction == 0)
        {
          context->translate (edge->n1->x, edge->n1->y);
          context->rotate (edge->angle);
        }
      else
        {
          context->translate (edge->n2->x, edge->n2->y);
          context->rotate (edge->angle + M_PI);
        }

      // Compute packet transmission time
      double txTime = pkt.lbTx - pkt.fbTx;

      // Adujst if last bit not yet transmitted
      if (pkt.lbTx > m_currentTime) txTime = m_currentTime - pkt.fbTx;
      // Adjust if first bit already received
      if (pkt.fbRx < m_currentTime) txTime -= m_currentTime - pkt.fbRx;

      // Propagation delay
      double delay = pkt.fbRx - pkt.fbTx;

      // Compute last bit distance from transmitter
      double lbTime = m_currentTime - pkt.lbTx; // Relative to current time

      if (lbTime < 0) lbTime = 0;

      double pktWidth = edge->distance * txTime / delay;

      //if (pktWidth > 4) pktWidth -= 4; // Some spacing between pkts

      double pktDist = edge->distance * lbTime / delay;

      context->move_to (pktDist, 0);
      context->line_to (pktDist + pktWidth, 0);
      context->stroke ();

      context->restore();
    }

  // draw nodes
  double delta = m_nodeWidth / 2;
  context->set_source_rgba (m_nodeColor.r, m_nodeColor.g, m_nodeColor.b, m_nodeColor.a);
  for (NodeMap::const_iterator i = m_nodes.begin (); i != m_nodes.end (); ++i)
    {
      double x = (*i).second.x;
      double y = (*i).second.y;
      context->rectangle ((*i).second.x - delta, y - delta, m_nodeWidth, m_nodeWidth);
    }

  context->fill ();

  context->restore ();
}

void
NamNetMotion::SetEdgeWidth (double width)
{
  m_edgeWidth = width;
}

double
NamNetMotion::GetEdgeWidth (void) const
{
  return m_edgeWidth;
}

void
NamNetMotion::SetEdgeColor (const RgbaColor &color)
{
  m_edgeColor = color;
}

RgbaColor
NamNetMotion::GetEdgeColor (void) const
{
  return m_edgeColor;
}

void
NamNetMotion::SetNodeWidth (double width)
{
  m_nodeWidth = width;
}

double
NamNetMotion::GetNodeWidth (void) const
{
  return m_nodeWidth;
}

void
NamNetMotion::SetNodeColor (const RgbaColor &color)
{
  m_nodeColor = color;
}

RgbaColor
NamNetMotion::GetNodeColor (void) const
{
  return m_nodeColor;
}

void
NamNetMotion::SetPacketWidth (double width)
{
  m_packetWidth = width;
}

double
NamNetMotion::GetPacketWidth (void) const
{
  return m_packetWidth;
}

void
NamNetMotion::SetPacketColor (const RgbaColor &color)
{
  m_packetColor = color;
}

RgbaColor
NamNetMotion::GetPacketColor (void) const
{
  return m_packetColor;
}

void
NamNetMotion::SetMotionSpeed (double speed)
{
  m_speed = speed;
}

double
NamNetMotion::GetMotionSpeed (void) const
{
  return m_speed;
}

void
NamNetMotion::SetCurrentTime (double time)
{
  m_currentTime = time;
}

double
NamNetMotion::GetCurrentTime (void) const
{
  return m_currentTime;
}

double
NamNetMotion::GetLastTime (void) const
{
  return m_lastTime;
}

std::vector<Node>
NamNetMotion::GetNodes (void) const
{
  std::vector<Node> result;
  for (NodeMap::const_iterator i = m_nodes.begin (); i != m_nodes.end (); ++i)
    {
      result.push_back ((*i).second);
    }
  return result;
}

void
NamNetMotion::LoadMotion (Glib::RefPtr<Gio::DataInputStream> stream)
{
  std::string line;
  uint32_t i1, i2;
  NodeMap::iterator n1, n2;
  EdgeVector::iterator e;
  double time = 0;
  char action;

  Stop ();
  m_currentTime = 0;
  m_nodes.clear ();
  m_packets.clear ();
  m_edges.clear ();

  while (stream->read_line (line))
    {
      std::istringstream iss (line);
      iss >> time >> action;

      switch (action)
      {
        case 'N' :
          {
            Node node;
            uint32_t id;
            iss >> id >> node.x >> node.y;
            m_nodes.insert (std::make_pair (id, node));
            break;
          }

        case 'L' : // Edge
          {
            uint32_t i1, i2;
            iss >> i1 >> i2;
            n1 = m_nodes.find (i1);
            n2 = m_nodes.find (i2);

            if (n1 == m_nodes.end () || n2 == m_nodes.end ())
              {
                continue;
              }

            m_edges.push_back (Edge ((*n1).second, (*n2).second));
            break;
          }

        case 'P' : // Packet
          {
            Packet packet;
            packet.fbTx = time;
            iss >> i1 >> i2 >> packet.lbTx >> packet.fbRx >> packet.lbRx;
            n1 = m_nodes.find (i1);
            n2 = m_nodes.find (i2);
            if (n1 == m_nodes.end () || n2 == m_nodes.end ())
              {
                continue;
              }

            for (e = m_edges.begin (); e != m_edges.end (); ++e)
              {
                if ((*e).n1 == &(*n1).second && (*e).n2 == &(*n2).second)
                  {
                    packet.direction = 0;
                    break;
                  }
                else if ((*e).n2 == &(*n1).second && (*e).n1 == &(*n2).second)
                  {
                    packet.direction = 1;
                    break;
                  }
              }

            if (e != m_edges.end ())
              {
                packet.edge = &(*e);
                m_packets.push_back (packet);
              }
            break;
          }

        default:
          break;
      }
    }

  if (m_packets.size ())
    {
      m_lastTime = m_packets.back ().lbRx;
    }
  else
    {
      m_lastTime = 0;
    }
}
