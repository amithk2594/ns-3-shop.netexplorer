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

typedef uint16_t MarkerIndex;
typedef uint32_t MarkerValue;

/**
 * Marker-related constants
 */
const uint32_t MAX_GRAPH_MARKERS = 10; // How many markers are allowed simultaneously
const MarkerValue GRAPH_MARKER_CLEAN = 0;
const MarkerValue GRAPH_MARKER_FIRST = 1;
const MarkerValue GRAPH_MARKER_LAST = (MarkerValue) (-1);

/**
 * \enum Possible marker errors
 */
enum MarkerErrorType
{
  /** Some error occured */
  M_ERROR_GENERIC,
  /** We've ran out of indexes. Someone forgot to free markers? */
  M_ERROR_OUT_OF_INDEXES,
  /** We're out of values. Seems to be interanl error of marker class.*/
  M_ERROR_OUT_OF_VALUES,
  /** Number of error types */
  M_ERROR_NUM
};

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
  /**
   * Create marked object
   */
  Marked ();
  /**
   * \param marker Marker
   * \returns false if node is already marked. True otherwise.
   * Mark node with marker.
   */
  inline bool Mark (const Marker &marker);
  /**
   * \param marker
   * \returns true if node is marked with this marker
  */
  inline bool IsMarked (const Marker &marker);
  /**
   * \param marker
   * \returns true if node has been marked with this marker and unmarks it
   */
  inline bool Unmark (const Marker &marker);
  /**
   * Clears value for given index
   */
  inline void Clear (MarkerIndex i);

private:
  MarkerValue m_markers[MAX_GRAPH_MARKERS];
};


/**
 * \brief Marker manager implementation
 */
class MarkerManager
{
public:
  MarkerManager ();
  /**
   * \brief Acquire new marker. Markers MUST be freed after use,otherwise you run to markers number limit.
   */
  Marker NewMarker (void);
  /**
   * \brief Free marker
   */
  inline void FreeMarker (const Marker &marker);

protected:
  /**
   * \brief Find free index
   */
  inline MarkerIndex FindFreeIndex (void) const;
  /**
   * \brief Increment marker value
   */
  inline MarkerValue NextValue (void);
  /**
   * \brief MUST BE implemented in inhereted class
   */
  virtual void ClearMarkersInObjects() = 0;
  /**
   * \brief Check if this value is busy
   */
  inline bool IsValueBusy (MarkerValue val) const;
  /**
   * \returns next free value
   */
  inline MarkerValue FindNextFreeValue (void);
  /**
   * \brief Clears unused markers in given object
   */
  inline void ClearUnusedMarkers (Marked *marked) const;

private:
  MarkerValue m_markers[MAX_GRAPH_MARKERS];
  bool m_isused[MAX_GRAPH_MARKERS];
  MarkerValue m_last;
};

}; // namespace graph

#endif /* MARKER_H */
