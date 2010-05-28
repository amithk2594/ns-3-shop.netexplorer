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

Numbered::Numbered ()
{
  for (NumIndex i = 0; i < MAX_NUMERATIONS; i++)
    {
      m_nums[i] = NUM_VALUE_CLEAN;
    }
}

Numbered::~Numbered ()
{
}

bool
Numbered::SetNumber (const Numeration &num, GraphNum value)
{
  if (value >= NUMBER_MAX)
    {
      throw NUM_ERROR_NUMBER_OUT_OF_RANGE;
    }

  m_nums[num.index] = num.value;
  m_numbers[num.index] = value;

//  if (m_nums[num.index] == num.value)
//    {
  return false;
//    }
//  return true;
}

GraphNum
Numbered::GetNumber (const Numeration &num)
{
  if (m_nums[num.index] == num.value)
    {
      return m_numbers[num.index];
    }
  return NUMBER_NO_NUM;
}

bool
Numbered::IsNumbered (const Numeration &num)
{
  return m_nums[num.index] == num.value;
}

bool
Numbered::UnNumber (const Numeration &num)
{
  if (m_nums[num.index] == num.value)
    {
      m_nums[num.index] = NUM_VALUE_CLEAN;
      return true;
    }

  return false;
}

void
Numbered::Clear (NumIndex i)
{
  m_nums[i] = NUM_VALUE_CLEAN;
}

//// NumerationManager
NumerationManager::NumerationManager ()
{
  for (NumIndex i = 0; i < MAX_NUMERATIONS; i++)
    {
      m_nums[i] = NUM_VALUE_CLEAN;
      m_isused[i] = false;
    }

  m_last = NUM_VALUE_FIRST;
}

NumerationManager::~NumerationManager ()
{
}

Numeration
NumerationManager::NewNum (void)
{
  Numeration num;
  num.index = FindFreeIndex ();
  m_isused[num.index] = true;
  num.value = FindNextFreeValue ();
  m_nums[num.index] = num.value;
  return num;
}

void
NumerationManager::FreeNum (const Numeration& num)
{
  m_isused[num.index] = false;
}

NumIndex
NumerationManager::FindFreeIndex (void) const
{
  for (NumIndex i = 0; i < MAX_NUMERATIONS; i++)
    {
      if (!m_isused[i])
        {
          return i;
        }
    }

  throw NUM_ERROR_OUT_OF_INDEXES;
  return 0;
}

NumValue
NumerationManager::NextValue (void)
{
  if (m_last == NUM_VALUE_LAST)
    {
      m_last = NUM_VALUE_FIRST;
    }
  else
    {
      m_last++;
    }
  return m_last;
}

bool
NumerationManager::IsValueBusy (NumValue val) const
{
  for (NumIndex i = 0; i < MAX_NUMERATIONS; i++)
    {
      if ( m_isused[i] && nums[i] == val)
        return true;
    }
  return false;
}

NumValue
NumerationManager::FindNextFreeValue (void)
{
  NumIndex i = 0;
  bool reached_limit = false;
  NumValue res = last;

  while (IsValueBusy (res))
    {
      if (res == NUM_VAL_LAST)
        {
          assert <NumErrorType> (!reached_limit, NUM_ERROR_OUT_OF_VALUES);
          ClearNumerationsInObjects();
          reached_limit = true;
        }
      res = NextValue();
    }
  return res;
}

void
NumerationManager::ClearUnusedNumerations (Numbered *numbered)
{
  for (NumIndex i = 0; i < MAX_NUMERATIONS; i++)
    {
      if (!m_isused[i]) numbered->Clear (i);
    }
}

}; // namespace graph
