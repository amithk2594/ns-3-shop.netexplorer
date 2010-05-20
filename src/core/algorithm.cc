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

#include "algorithm.h"
#include <iostream>

namespace algorithm {

double Zoom (const Rectangle &rect, const std::vector<Node> &nodes)
{
  double minX = 0.0, minY = 0.0, maxX = 0.0, maxY = 0.0;

  for (std::vector<Node>::const_iterator i = nodes.begin (); i != nodes.end (); ++i)
    {
      if ((*i).x < minX)
        {
          minX = (*i).x;
        }
      else if ((*i).x > maxX)
        {
          maxX = (*i).x;
        }

      if ((*i).y < minY)
        {
          minY = (*i).y;
        }
      else if ((*i).y > maxY)
        {
          maxY = (*i).y;
        }
    }

  if ((maxX - minX < 1E-9) || (maxY - minY < 1E-9))
    {
      return 1.0;
    }

  double zX = (maxX - minX) / rect.width ();
  double zY = (maxY - minY) / rect.height ();

  if (zX < zY)
    {
      return zX;
    }
  else
    {
      return zY;
    }
}

//template<class T>
//bool
//VectorContains (const std::vector<T> &vec, const T &value)
//{
//  for (std::vector<T>::const_iterator i = vec.begin (); i != vec.end (); ++i)
//    {
//      if (*i == value)
//        {
//          return true;
//        }
//    }

//  return false;
//}

//std::vector<Edge>
//GetSpanningTree (const std::vector<Edge> &edges)
//{
//  std::vector<Node*> nodes;
//  std::vector<Edge> result;

//  for (std::vector<Edge>::iterator i = edges.begin (); i != edges.end (); ++i)
//    {
//      bool c1 = VectorContains<Node*> (nodes, (*i).n1);
//      bool c2 = VectorContains<Node*> (nodes, (*i).n2);

//      if (!c1 || !c2)
//        {
//          if (!c1)
//            {
//              nodes.push_back ((*i).n1);
//            }

//          if (!c2)
//            {
//              nodes.push_back ((*i).n2);
//            }

//          result.push_back (*i);
//        }
//    }

//  return result;
//}

//std::vector<Node*>
//GetAdjacentNodes (const Node* node, const std::vector<Edge> &edges)
//{
//  std::vector<Node*> result;

//  for (std::vector<Edge>::const_iterator i = edges.begin (); i != edges.end (); ++i)
//    {
//      if (node == (*i).n1)
//        {
//          result.push_pack ((*i).n2));
//        }
//      else if (node == (*i).n2)
//        {
//          result.push_pack ((*i).n1));
//        }
//    }

//  return result;
//}

//uint32_t
//GetTreeDepth (const Node* node, const std::vector<Edge> &edges)
//{
//  return GetTreeDepth (node, edges, 0);
//}

//uint32_t
//GetTreeDepth (const Node* node, const std::vector<Edge> &edges, const Node* prev)
//{
//  uint32_t depth = 0;
//  std::vector<Node*> adjacent = GetAdjacentEdges (node, edges);
//  for (std::vector<Node*>::const_iterator i = adjacent.begin (); i != adjacent.end (); ++i)
//    {
//      Node* n = *i;
//      if (n == prev)
//        {
//          continue;
//        }

//      uint32_t d = GetTreeDepth (n, edges) + 1;
//      if (d > depth)
//        {
//          depth = d;
//        }
//    }

//  return depth;
//}

//std::vector<Node>
//Reorder (const std::vector<Node*> &nodes, const std::vector<Edge> &edges)
//{
//  Node* node = 0;
//  uint32_t depth = 0;
//  std::vector<Node> result;

//  std::vector<Edge> spanning = GetSpanningTree (edges);
//  for (std::vector<Node*>::const_iterator i = nodes.begin (); i != nodes.end (); ++i)
//    {
//      uint32_t d = GetTreeDepth (*i, edges);
//      if (d > depth)
//        {
//          depth = d;
//          node = *i;
//        }
//    }

//  if (node == 0)
//    {
//      return result;
//    }

//  Node n(0.0, 0.0);
//  result.push_back (n);

//  return result;
//}

}; // namespace algorithm
