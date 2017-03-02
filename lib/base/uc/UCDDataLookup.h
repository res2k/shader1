/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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
 * Lookup functions for data sets generated from UCD data
 */

#ifndef __UCDDATALOOKUP_H__
#define __UCDDATALOOKUP_H__

namespace s1
{
  namespace uc
  {
    /**
     * Returns the index of a character in an UCD range array,
     * or \c ~0u if not found.
     */
    template<typename T, unsigned int N>
    static inline unsigned int IndexInRanges (Char32 ch, const T (&ranges)[N])
    {
      const unsigned int numChars = N/2;
      unsigned int first = 0, last = numChars;
      while (first < last)
      {
        unsigned int m = (first+last)/2;
        if (ch < ranges[m*2])
        {
          last = m;
        }
        else if ((m+1 < last) && (ch >= ranges[(m+1)*2]))
        {
          first = m+1;
        }
        else
        {
          return (ch >= ranges[m*2]) && (ch <= ranges[m*2+1]) ? m : ~0;
        }
      }
      return ~0u;
    }

    /// Returns whether a character is in a character "set".
    template<typename T, unsigned int N>
    static inline bool IsCharInSet (Char32 ch, const T (&set)[N])
    {
      return IndexInRanges (ch, set) != ~0u;
    }

    /// Looks up an UCD property for a character
    template<typename T, typename DataType>
    static inline T LookupProperty (Char32 ch, const DataType& data, const T& defaultVal)
    {
      unsigned int keyIndex (IndexInRanges (ch, data.key));
      if (keyIndex == ~0u) return defaultVal;
      unsigned int dataOfs = ch - data.key[keyIndex * 2] + data.idx[keyIndex];
      return data.data[dataOfs];
    }

    /// Looks up a character sequence for another character
    template<typename DataType, typename ResultFunc>
    static inline bool LookupSeq (Char32 ch, const DataType& data, ResultFunc f)
    {
      unsigned int keyIndex (IndexInRanges (ch, data.key));
      if (keyIndex == ~0u) return false;
      unsigned int dataOfs = ch - data.key[keyIndex * 2] + data.idx[keyIndex];
      uint32_t charData = data.data[dataOfs];
      unsigned int seqLen = (charData >> 24) + 1;
      if (seqLen == 1)
      {
        // One-char sequences are stored 'inline'
        f (data.data[dataOfs]);
      }
      else
      {
        // Multi-char sequences are in the 'seqdata' table
        auto seq = &(data.seqdata[charData & 0xffffff]);
        auto seqEnd = seq + seqLen;
        UTF16Decoder decoder;
        while (seq < seqEnd)
        {
          Char32 ch;
          if (decoder (seq, seqEnd, ch) != UTF16Decoder::drSuccess) break;
          f (ch);
        }
      }
      return true;
    }
  } // namespace uc
} // namespace s1

#endif // __UCDDATALOOKUP_H__
