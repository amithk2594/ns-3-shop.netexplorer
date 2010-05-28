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

#ifndef NODE_H
#define NODE_H

#include "marker.h"
#include "numeration.h"

namespace graph {

/**
 * \brief Node representation class.
 */
class Node: public Marked, public Numbered
{
public:
  virtual ~Node();

  inline uint32_t GetId (void) const;
  inline uint32_t GetUserId (void) const;
  inline void SetUserId (uint32_t id);
  /**
   * \brief get node's corresponding graph
   */
  inline Graph* GetGraph (void) const
  /**
   * Add edge to node in specified direction
   */
  virtual void AddEdgeInDir (Edge *edge, GraphDir dir);
  /**
   * Add predecessor edge
   */
  virtual inline void AddPred (Edge *edge)
  {
    addEdgeInDir (edge, GRAPH_DIR_UP);
  }

  /**
   * Add successor edge
   */
  virtual inline void addSucc( Edge *edge)
  {
    addEdgeInDir (edge, GRAPH_DIR_DOWN);
  }
  /**
   *  Iteration through edges routines.
   *
   *  set iterator to beginning of edge list and return first edge
   */
  virtual inline Edge* firstEdgeInDir( GraphDir dir)
  {
      e_it[ dir ] = edges[ dir ];

      if ( e_it[ dir] == NULL)
      {
          return NULL;
      }
      return e_it[ dir ]->data();
  }
  /**
   * Advance iterator and return next edge in specified direction
   * NOTE: If end of list is reached we return NULL for first time and fail if called once again
   */
  virtual inline Edge* nextEdgeInDir( GraphDir dir)
  {
      e_it[ dir] = e_it[ dir]->next();
      return (e_it[ dir] != NULL )? e_it[ dir]->data() : NULL;
  }
  /**
   * Return true if iterator of list points to one-after-last element
   */
  inline bool endOfEdgesInDir( GraphDir dir)
  {
      return e_it [ dir] == NULL;
  }

  /**
   * Corresponding iterators for successors/predeccessors.
   * NOTE: See firstEdgeInDir and other ...InDir routines for details
   */
  virtual inline Edge* firstSucc()
  {
      return firstEdgeInDir( GRAPH_DIR_DOWN);
  }
  virtual inline Edge* nextSucc()
  {
      return nextEdgeInDir( GRAPH_DIR_DOWN);
  }
  inline bool endOfSuccs()
  {
      return endOfEdgesInDir( GRAPH_DIR_DOWN);
  }
  virtual inline Edge* firstPred()
  {
      return firstEdgeInDir( GRAPH_DIR_UP);
  }
  virtual inline Edge* nextPred()
  {
      return nextEdgeInDir( GRAPH_DIR_UP);
  }
  inline bool endOfPreds()
  {
      return endOfEdgesInDir( GRAPH_DIR_UP);
  }
  /**
   * Deletion of edge in specified direction
   */
  void DeleteEdgeInDir (GraphDir dir, EdgeListItem* it);
  /**
   * delete predecessor edge
   */
  inline void DeletePredecessor (EdgeListItem* it);
  /**
   * \delete successor edge
   */
  inline void DeleteSuccesor (EdgeListItem* it);
  /**
   * \returns next node of a graph
   */
  inline Node* nextNode (void);
  /**
   * \returns previous node of a graph
   */
  inline Node* prevNode (void);





    int user_id;
    /** Graph is closely connected class by implementation */
    friend class Graph;

    /** Connection with inclusive graph */
    int unique_id; // Unique id is given by graph and cannot be modified
        protected:
                Graph * graph;// Pointer to graph
        private:
    NodeListItem my_it;//Item of graph's list

    //Lists of edges and iterators for them
    EdgeListItem *edges[ GRAPH_DIRS_NUM];
    EdgeListItem *e_it[ GRAPH_DIRS_NUM];

    inline NodeListItem* getGraphIt()
    {
        return &my_it;
    }
        inline void detachFromGraph()
    {
        my_it.detach();
    }
protected:
        virtual void writeByXmlWriter( xmlTextWriterPtr writer);
        virtual void readByXml (xmlNode * cur_node);
protected:

    /** We can't create nodes separately, do it through NewNode method of graph */
    Node( Graph *graph_p, int _id):
         unique_id( _id), graph( graph_p), my_it()
    {
        edges[ GRAPH_DIR_UP] = NULL;
        edges[ GRAPH_DIR_DOWN] = NULL;
        e_it[ GRAPH_DIR_UP] = NULL;
        e_it[ GRAPH_DIR_DOWN] = NULL;
        my_it.setData( ( Node*)this);
        setUserId( _id);
    }


};

} // namespace graph

#endif /* NODE_H */
