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

Marked::Marked()
{
}


bool
Marked::Mark (const Marker &marker)
{
  if (markers[marker.index] == marker.value)
    {
      return false;
    }
  markers[marker.index] = marker.value;
  return true;
}

bool
Marked::isMarked (const Marker &marker)
{
  if (markers[marker.index] == marker.value)
    {
      return true;
    }
  return false;
}

bool
Marked::Unmark (const Marker &marker)
{
  if (markers[marker.index] == marker.value)
    {
      markers[marker.index] = GRAPH_MARKER_CLEAN;
      return true;
    }
  return false;
}

void Clear (MarkerIndex i)
{
   markers[i] = GRAPH_MARKER_CLEAN;
}

/**
 * Class that creates/frees markers
 */
class MarkerManager
{
    /**
     * Markers
     */
    MarkerValue markers[ MAX_GRAPH_MARKERS];
    bool is_used[ MAX_GRAPH_MARKERS];
    MarkerValue last;

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
public:

    /**
     * Default Constructor
     */
    MarkerManager()
    {
        MarkerIndex i;

        /** Initialize markers */
        for ( i = 0; i < MAX_GRAPH_MARKERS; i++)
        {
            markers [ i] = GRAPH_MARKER_CLEAN;
            is_used [ i] = false;
        }
        last = GRAPH_MARKER_FIRST;
    }

    /**
     * Acquire new marker. Markers MUST be freed after use,
     * otherwise you run to markers number limit.
     */
    Marker newMarker()
    {
        try {
            Marker new_marker;

            new_marker.index = findFreeIndex();
            is_used[ new_marker.index] = true;
            new_marker.value = findNextFreeValue();
            markers[ new_marker.index] = new_marker.value;
            return new_marker;
        } catch ( MarkerErrorType type)
        {
            /** Handle errors */
            switch ( type)
            {
                case M_ERROR_GENERIC:
                default:
                    assert(0);
            }
            assert(0);
        }
        return newMarker();
    }
    /**
     * Free marker
     */
    inline void freeMarker( Marker m)
    {
        is_used[ m.index] = false;
    }
};
}; // namespace graph

#endif /* MARKER_H */
