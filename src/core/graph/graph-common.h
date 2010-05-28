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

#ifndef GRAPH_COMMON_H
#define GRAPH_COMMON_H

#include <list>
#include "num.h"
#include "marker.h"
#include "graph_error.h"

namespace graph {

class Graph;
class Node;
class Edge;

/**
 * \brief Number type used for numbering nodes and edges in graph
 */
typedef uint32_t GraphNum;
/**
 * \enum Directions type in graph
 */
enum GraphDir
{
    GRAPH_DIR_UP,
    GRAPH_DIR_DOWN,
    GRAPH_DIRS_NUM
};
/**
 * Return direction that is reverse to given one
 */
/*inline GraphDir
revDir (GraphDir dir)
{
    graphassert( GRAPH_DIRS_NUM == 2);
    return ( dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN: GRAPH_DIR_UP;
}
*/

const GraphNum GRAPH_MAX_NODE_NUM  = (GraphNum)-1;
const GraphNum GRAPH_MAX_EDGE_NUM  = (GraphNum)-1;

//typedef ListItem<Node> NodeListItem;
//typedef ListItem<Edge> EdgeListItem;

}; // namespace graph

#endif /* MARKER_H */
