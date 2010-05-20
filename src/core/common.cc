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

#include <cmath>

#include "common.h"

RgbaColor::RgbaColor (double rr, double gg, double bb, double aa)
  : r (rr),
    g (gg),
    b (bb),
    a (aa)
{
}

RgbaColor::~RgbaColor ()
{
}

RgbColor::RgbColor (double rr, double gg, double bb)
  : r (rr),
    g (gg),
    b (bb)
{
}

RgbColor::~RgbColor ()
{
}

RgbColor::operator RgbaColor () const
{
  return RgbaColor (r, g, b, 1.0);
}

Rectangle::Rectangle ()
  : left (0),
    top (0),
    right (0),
    bottom (0)
{
}

Rectangle::Rectangle (double x1, double y1, double x2, double y2)
  : left (x1),
    top (y1),
    right (x2),
    bottom (y2)
{
}

Rectangle::~Rectangle ()
{
}

double
Rectangle::width (void) const
{
  return std::fabs (right - left);
}

double
Rectangle::height (void) const
{
  return std::fabs (bottom - top);
}

Node::Node ()
  : x (0),
    y (0)
{
}

Node::Node (double xx, double yy)
  : x (xx),
    y (yy)
{
}

Node::~Node ()
{
}

Edge::Edge ()
  : n1 (0),
    n2 (0)
{
}

Edge::Edge (const Node &node1, const Node &node2)
{
  SetNodes (node1, node2);
}

void
Edge::SetNodes (const Node &node1, const Node &node2)
{
  n1 = &node1;
  n2 = &node2;
  double dy = node2.y - node1.y;
  double dx = node2.x - node1.x;
  distance = std::sqrt (dy * dy + dx * dx);
  angle = std::atan2 (dy, dx);
}

Edge::~Edge ()
{
}

Packet::Packet ()
  : edge (0)
{
}

Packet::Packet (const Edge &e, uint32_t dir, double firstBitTx, double lastBitTx, double firstBitRx, double lastBitRx)
  : edge (&e),
    direction (dir),
    fbTx (firstBitTx),
    lbTx (lastBitTx),
    fbRx (firstBitRx),
    lbRx (lastBitRx)
{
}

Packet::~Packet ()
{
}
