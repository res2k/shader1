/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * Bit-scan functions
 */
#ifndef __BASE_BIT_SCAN_H__
#define __BASE_BIT_SCAN_H__

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace s1
{
  namespace bit_scan
  {
    //@{
    /**
     * Return 0-based index of the least significant set bit of \a x.
     * Return value is undefined if no bits are set in \a x.
     */
    static inline int lsb (unsigned int x)
    {
    #ifdef _MSC_VER
      unsigned long index;
      _BitScanForward (&index, x);
      return index;
    #else
      return __builtin_ffs (x) - 1;
    #endif
    }
    static inline int lsb (unsigned long x)
    {
    #ifdef _MSC_VER
      unsigned long index;
      _BitScanForward (&index, x);
      return index;
    #else
      return __builtin_ffsl (x) - 1;
    #endif
    }
    static inline int lsb (unsigned long long x)
    {
    #ifdef _MSC_VER
      #if defined(_M_X64) || defined(_M_ARM64)
        unsigned long index;
        _BitScanForward64 (&index, x);
        return index;
      #else
        unsigned long index;
        union
        {
          unsigned long long ull;
          unsigned long ul[2];
        } u;
        u.ull = x;
        if (_BitScanForward (&index, u.ul[0])) return index;
        _BitScanForward (&index, u.ul[1]);
        // Scanning from right, check low dword first
        return sizeof(unsigned long) * CHAR_BIT + index;
      #endif
    #else
      return __builtin_ffsll (x) - 1;
    #endif
    }
    //@}

    //@{
    /**
     * Return 0-based index of the most significant set bit of \a x.
     * Return value is undefined if no bits are set in \a x.
     */
    static inline int msb (unsigned int x)
    {
    #ifdef _MSC_VER
      unsigned long index;
      _BitScanReverse (&index, x);
      return index;
    #else
      return (CHAR_BIT * sizeof (unsigned int) - __builtin_clz (x) - 1);
    #endif
    }
    static inline int msb (unsigned long x)
    {
    #ifdef _MSC_VER
      unsigned long index;
      _BitScanReverse (&index, x);
      return index;
    #else
      return (CHAR_BIT * sizeof (unsigned long) - __builtin_clzl (x) - 1);
    #endif
    }
    static inline int msb (unsigned long long x)
    {
    #ifdef _MSC_VER
      #if defined(_M_X64) || defined(_M_ARM64)
        unsigned long index;
        _BitScanReverse64 (&index, x);
        return index;
      #else
        unsigned long index;
        union
        {
          unsigned long long ull;
          unsigned long ul[2];
        } u;
        u.ull = x;
        // Scanning from left, check high dword first
        if (_BitScanReverse (&index, u.ul[1])) return sizeof(unsigned long) * CHAR_BIT + index;
        _BitScanReverse (&index, u.ul[0]);
        return index;
      #endif
    #else
      return (CHAR_BIT * sizeof (unsigned long long) - __builtin_clzll (x) - 1);
    #endif
    }
    //@}

  } // namespace bit_scan
} // namespace s1

#endif // __BASE_BIT_SCAN_H__
