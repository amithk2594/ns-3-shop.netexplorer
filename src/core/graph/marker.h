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

#ifndef MARKER_H
#define MARKER_H

namespace graph {

typedef unsigned short int MarkerIndex;
typedef unsigned int MarkerValue;

/**
 * Marker-related constants
 */
const short int MAX_GRAPH_MARKERS = 10; /** How many markers are allowed simultaneously */
const MarkerValue GRAPH_MARKER_CLEAN = 0;
const MarkerValue GRAPH_MARKER_FIRST = 1;
const MarkerValue GRAPH_MARKER_LAST = ( MarkerValue)( (int)-1);

/**
 * \enum Possible marker errors
 */
typedef enum MarkerErrorType_e
{
  /** Some error occured */
  M_ERROR_GENERIC,
  /** We've ran out of indexes. Someone forgot to free markers? */
  M_ERROR_OUT_OF_INDEXES,
  /** We're out of values. Seems to be interanl error of marker class.*/
  M_ERROR_OUT_OF_VALUES,
  /** Number of error types */
  M_ERROR_NUM
} MarkerErrorType;

/**
 * \brief Marker class
 */
class Marker
{
  MarkerIndex index;
  MarkerValue value;
  friend class Marked;
  friend class MarkerManager;
};

/**
 * \brief Marked object
 */
class Marked
{
public:
  Marked();
  /**
   * \param marker Marker
   * \returns false if node is already marked. True otherwise.
   * Mark node with marker.
   */
  bool Mark (const Marker &marker);
  /**
   * \param marker
   * \returns true if node is marked with this marker
  */
  bool IsMarked (const Marker &marker);
  /**
   * \param marker
   * \returns true if node has been marked with this marker and unmarks it
   */
  bool Unmark (const Marker &marker);
  /**
   * Clears value for given index
   */
  void Clear (MarkerIndex i);

private:
  MarkerValue markers[MAX_GRAPH_MARKERS];
};


/**
 * \brief Marker manager implementation
 */
class MarkerManager
{
public:
  MarkerManager();
  /**
   * \brief Acquire new marker. Markers MUST be freed after use,otherwise you run to markers number limit.
   */
  Marker NewMarker();
  /**
   * Free marker
   */
  inline void FreeMarker (const Marker &marker);

private:

  /** Implementatinal routines */
  /**
   * Find free index
   */
  inline MarkerIndex findFreeIndex()
  {
      MarkerIndex i = 0;
      /** Search for free marker index */
      for ( i = 0; i < MAX_GRAPH_MARKERS; i++)
      {
          if ( !is_used [ i])
          {
              return i;
          }
      }
      throw M_ERROR_OUT_OF_INDEXES;
      return i;
  }

  /**
   * Increment marker value
   */
  inline MarkerValue nextValue()
  {
      if ( last == GRAPH_MARKER_LAST)
      {
          last = GRAPH_MARKER_FIRST;
      } else
      {
          last++;
      }
      return last;
  }

  /**
   * MUST BE implemented in inhereted class
   */
  virtual void clearMarkersInObjects() = 0;

  /**
   * Check if this value is busy
   */
  inline bool isValueBusy( MarkerValue val)
  {
      /** Check all markers */
      for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
      {
          if ( is_used [ i] && markers[ i] == val)
              return true;
      }
      return false;
  }

  /**
   * Return next free value
   */
  inline MarkerValue findNextFreeValue()
  {
      MarkerIndex i = 0;
      bool reached_limit = false;
      MarkerValue res = last;

      while( isValueBusy( res))
      {
          /**
           * If we reached checked GRAPH_MARKER_LAST twice,
           * then we are in infinite loop and for some reason we don't free our markers
           */
          if ( res == GRAPH_MARKER_LAST)
          {
              assert< MarkerErrorType> ( !reached_limit,
                                         M_ERROR_OUT_OF_VALUES);
              clearMarkersInObjects();
              reached_limit = true;
          }
          res = nextValue();
      }
      return res;
  }
protected:
  /**
   * Clears unused markers in given object
   */
  inline void clearUnusedMarkers( Marked *m_obj)
  {
      for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
      {
          if ( !is_used [ i])
              m_obj->clear( i);
      }
  }

private:
    /**
     * Markers
     */
    MarkerValue markers[ MAX_GRAPH_MARKERS];
    bool is_used[ MAX_GRAPH_MARKERS];
    MarkerValue last;
};

}; // namespace graph

#endif /* MARKER_H */
