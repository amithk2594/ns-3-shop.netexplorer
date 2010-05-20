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

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdint.h>
#include <vector>
#include "common.h"

namespace algorithm {

double Zoom (const Rectangle &rect, const std::vector<Node> &nodes);
//std::vector<Edge> GetAdjacentEdges (const Node* node, const std::vector<Edge> &edges);
//std::vector<Edge> GetSpanningTree (const std::vector<Edge> &edges);

}; // namespace algorithm

#endif /* ALGORITHMS_H */
