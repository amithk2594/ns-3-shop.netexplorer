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

#ifndef NUMERATION_H
#define NUMERATION_H

#include "graph-common.h"

namespace graph {

typedef uint16_t NumIndex;
typedef uint32_t NumValue;

/**
 * Num-related constants
 */
const short int MAX_NUMERATIONS = 10;
const NumValue NUM_VALUE_CLEAN = 0;
const NumValue NUM_VALUE_FIRST = 1;
const NumValue NUM_VALUE_LAST = (NumValue)-1;
const GraphNum NUMBER_NO_NUM = (GraphNum) -1;
const GraphNum NUMBER_MAX = ((GraphNum) -1) - 1;

/**
 * \enum Possible num errors
 */
enum NumErrorType
{
  /** Some error occured */
  NUM_ERROR_GENERIC,
  /** We've ran out of indexes. Someone forgot to free nums? */
  NUM_ERROR_OUT_OF_INDEXES,
  /** We're out of values. Seems to be interanl error of num class.*/
  NUM_ERROR_OUT_OF_VALUES,
  /** Number is too big */
  NUM_ERROR_NUMBER_OUT_OF_RANGE,
  /** Number of error types */
  NUM_ERROR_NUM
};

/**
 * \brief Number description
 */
class Numeration
{
  NumIndex index;
  NumValue value;

  friend class Numbered;
  friend class NumerationManager;
};

/**
 * \brief Represents an object that can be involved in numeration
 */
class Numbered
{
public:
  /**
   * \brief Create numered object
   */
  Numbered ();
  virtual ~Numbered ();

  /**
   * \brief Mark node with num. 
   * \returns false if node is already numbered.
   */
  inline bool SetNumber (const Numeration &num, GraphNum value);
  /**
   * \returns number in given numeration or NO_NUM if it was not numbered yet
   */
  inline GraphNum GetNumber (const Numeration &num);
  /**
   * \returns true if node is numbered in this numeration
   */
  inline bool IsNumbered (const Numeration &num);
  /**
   * \returns true if node has been numbered in this numeration and "unnumber" it
   */
  inline bool UnNumber (const Numeration &num);
  /**
   * Clears value for given index
   */
  inline void Clear (NumIndex i);

private:   
  NumValue m_nums[MAX_NUMERATIONS];
  GraphNum m_numbers[MAX_NUMERATIONS];
};

/**
 * \brief Numeration manager
 */
class NumerationManager
{
public:
  /**
   * \brief Default Constructor
   */
  NumerationManager ();
  virtual ~NumerationManager ();
  /**
   * \brief Acquire new num. Nums MUST be freed after use.
   */
  Numeration NewNum (void);
  /**
   * \brief Free num
   */
  inline void FreeNum (const Numeration& num);

protected:
  /**
   * \brief Find free index
   */
  inline NumIndex FindFreeIndex (void) const;
  /**
   * \brief Increment num value
   */
  inline NumValue NextValue (void);
  /**
   * \brief Check if this value is busy
   */
  inline bool IsValueBusy (NumValue val) const;
  /**
   * \brief Return next free value
   */
  inline NumValue FindNextFreeValue (void);
  /**
   * \brief Clears unused markers in given object
   */
  inline void ClearUnusedNumerations (Numbered *numbered)
  /**
   * \brief MUST BE implemented in inhereted class 
   */
  virtual void ClearNumerationsInObjects() = 0;  

private:
  NumValue m_nums[MAX_NUMERATIONS];
  bool m_isused[MAX_NUMERATIONS];
  NumValue last;
};

}; // namespace graph

#endif /* NUMERATION_H */
