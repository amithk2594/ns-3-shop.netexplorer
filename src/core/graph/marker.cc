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

namespace graph {

Marked::Marked ()
{
  for (uint32_t i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
      m_markers[i] = GRAPH_MARKER_CLEAN;
    }
}

bool
Marked::Mark (const Marker &marker)
{
  if (m_markers[marker.index] == marker.value)
    {
      return false;
    }

  m_markers[marker.index] = marker.value;
  return true;
}

bool
Marked::isMarked (const Marker &marker)
{
  return m_markers[marker.index] == marker.value;
}

bool
Marked::Unmark (const Marker &marker)
{
  if (m_markers[marker.index] == marker.value)
    {
      m_markers[marker.index] = GRAPH_MARKER_CLEAN;
      return true;
    }

  return false;
}

void Clear (MarkerIndex i)
{
   m_markers[i] = GRAPH_MARKER_CLEAN;
}

//// MarkerManager

MarkerManager::MarkerManager()
{
  for (MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
      m_markers[i] = GRAPH_MARKER_CLEAN;
      m_isused[i] = false;
    }

  m_last = GRAPH_MARKER_FIRST;
}

Marker
MarkerManager::NewMarker()
{
  Marker marker;
  marker.index = FindFreeIndex ();
  m_isused[marker.index] = true;
  marker.value = FindNextFreeValue ();
  markers [marker.index] = marker.value;
  return marker;
}

void
MarkerManager::FreeMarker (const Marker &marker)
{
  m_isused[marker.index] = false;
}

MarkerIndex
MarkerManager::FindFreeIndex(void) const
{
  for (MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
      if (!m_isused[i])
        {
          return i;
        }
    }

  throw M_ERROR_OUT_OF_INDEXES;
  return 0;
}

MarkerValue
MarkerManager::NextValue (void)
{
  if (m_last == GRAPH_MARKER_LAST)
    {
      m_last = GRAPH_MARKER_FIRST;
    }
  else
    {
      m_last++;
    }

  return m_last;
}

bool
MarkerManager::IsValueBusy (MarkerValue val) const
{
  for (MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
      if (m_isused[i] && markers[i] == val)
          return true;
    }

  return false;
}

MarkerValue
MarkerManager::FindNextFreeValue (void)
{
  MarkerIndex i = 0;
  bool reached_limit = false;
  MarkerValue res = last;

  while (IsValueBusy (res))
    {
      if (res == GRAPH_MARKER_LAST)
        {
          assert<MarkerErrorType> (!reached_limit, M_ERROR_OUT_OF_VALUES);
          ClearMarkersInObjects ();
          reached_limit = true;
        }
      res = NextValue();
    }
  return res;
}

void
MarkerManager::ClearUnusedMarkers (Marked *marked) const
{
  for (MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
      if (!m_isused[i]) marked->Clear (i);
    }
}

}; // namespace graph
