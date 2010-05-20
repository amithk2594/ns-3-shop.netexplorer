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

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

class RgbaColor
{
public:
  RgbaColor (double rr, double gg, double bb, double aa);
  virtual ~RgbaColor ();
public:
  double r;
  double g;
  double b;
  double a;
};

class RgbColor
{
public:
  RgbColor (double rr, double gg, double bb);
  virtual ~RgbColor ();
  operator RgbaColor () const;

public:
  double r;
  double g;
  double b;
};

class Rectangle
{
public:
  Rectangle ();
  Rectangle (double x1, double y1, double x2, double y2);
  virtual ~Rectangle ();

  double width (void) const;
  double height (void) const;

public:
  double left;
  double top;
  double right;
  double bottom;

};

class Node
{
public:
  Node ();
  Node (double xx, double yy);
  virtual ~Node ();

public:
  double x;
  double y;
};

class Edge
{
public:
  Edge ();
  Edge (const Node &node1, const Node &node2);
  virtual ~Edge ();

  void SetNodes (const Node &node1, const Node &node2);

public:
  const Node *n1;
  const Node *n2;
  double distance;
  double angle;
};

class Packet
{
public:
  Packet ();
  Packet (const Edge &e, uint32_t dir, double firstBitTx, double lastBitTx, double firstBitRx, double lastBitRx);
  virtual ~Packet ();

public:
  const Edge *edge;
  uint32_t direction; // 0 - from n1 to n2
  double fbTx;
  double lbTx;
  double fbRx;
  double lbRx;
};

#endif /* COMMON_H */
