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

#include "base/common.h"

#include "base/uc/Exception.h"
#include "base/uc/String.h"

#include <algorithm>
#include <boost/container/vector.hpp>
#include <boost/foreach.hpp>

#include "UCDDataLookup.h"

namespace s1
{
  namespace uc
  {
    #include "ucd_CanonicalDecomp.inc"
    #include "ucd_CombiningClass.inc"
    #include "ucd_NFD_QC_No.inc"

    String String::Normalized (NormalizationMode mode)
    {
      switch (mode)
      {
      case normNFD:
          return NormalizeNFD();
          break;
      }
      // Fallback case. Really, should not happen...
      return *this;
    }

    // Do a quick check if a normalization is required
    static bool NeedNormalizationNFD (const String& str)
    {
      // Algorithm is described in UAX #15
      uint8_t lastCombiningClass = 0;
      String::CharacterIterator idIt (str);
      while (idIt.hasNext())
      {
        Char32 ch = idIt.next32PostInc();
        uint8_t combiningClass = LookupProperty (ch, ucd_CombiningClass, static_cast<uint8_t> (0));
        if ((combiningClass != 0) && (lastCombiningClass > combiningClass))
          return true;
        if (IsCharInSet (ch, ucd_NFD_QC_No))
          return true;
        lastCombiningClass = combiningClass;
      }
      return false;
    }

    namespace
    {
      class CombiningMarksBuffer
      {
        boost::container::vector<Char32> vec;

        static bool CombiningMarkCompare (Char32 ch1, Char32 ch2)
        {
          uint8_t class1 = LookupProperty (ch1, ucd_CombiningClass, static_cast<uint8_t> (0));
          uint8_t class2 = LookupProperty (ch2, ucd_CombiningClass, static_cast<uint8_t> (0));
          return (class1 < class2);
        }
      public:
        void Flush (String& str)
        {
          if (!vec.empty())
          {
            std::stable_sort (vec.begin(), vec.end(), &CombiningMarkCompare);
            BOOST_FOREACH(Char32 mark, vec)
            {
              str.append (mark);
            }
            vec.clear();
          }
        }
        void Push (Char32 ch)
        {
          vec.push_back (ch);
        }
      };

      // Hangul decomposition
      namespace hangul
      {
        static const Char32 SBase = 0xAC00;
        static const Char32 LBase = 0x1100;
        static const Char32 VBase = 0x1161;
        static const Char32 TBase = 0x11A7;
        static const int LCount = 19;
        static const int VCount = 21;
        static const int TCount = 28;
        static const int NCount = VCount * TCount;
        static const int SCount = LCount * NCount;

        static inline bool IsPrecomposed (Char32 ch)
        {
          return (ch >= SBase) && (ch < SBase + SCount);
        }

        static inline void Decompose (Char32 ch, Char32& LPart, Char32& VPart, Char32& TPart)
        {
          int SIndex = ch - SBase;
          int LIndex = SIndex / NCount;
          int VIndex = (SIndex % NCount) / TCount;
          int TIndex = SIndex % TCount;
          LPart = LBase + LIndex;
          VPart = VBase + VIndex;
          TPart = TIndex > 0 ? TBase + TIndex : 0;
        }
      } // namespace hangul
    } // anonymous namespace

    String String::NormalizeNFD ()
    {
      if (!NeedNormalizationNFD (*this))
        return *this;

      //
      // Actual normalization
      //
      struct State
      {
        String str;
        CombiningMarksBuffer combiningMarks;

        void Flush() { combiningMarks.Flush (str); }
        void HandleChar (Char32 ch)
        {
          uint8_t combiningClass = LookupProperty (ch, ucd_CombiningClass, static_cast<uint8_t> (0));
          if (combiningClass == 0)
          {
            // Character is a starter character.
            combiningMarks.Flush (str);
            str.append (ch);
          }
          else
          {
            // Combining mark
            combiningMarks.Push (ch);
          }
        }
      };
      // TODO: reserve some memory for destination str
      State normalizationState;
      CharacterIterator idIt (*this);
      while (idIt.hasNext())
      {
        Char32 ch = idIt.next32PostInc();
        if (hangul::IsPrecomposed (ch))
        {
          Char32 L, V, T;
          hangul::Decompose (ch, L, V, T);
          normalizationState.HandleChar (L);
          normalizationState.HandleChar (V);
          if (T != 0)
            normalizationState.HandleChar (T);
          continue;
        }
        const Char32* decompSeq;
        unsigned int decompSeqSize;
        if (LookupSeq (ch, ucd_CanonicalDecomp, decompSeq, decompSeqSize))
        {
          /* Note: the decomposition data is stored fully decomposed,
           * recursion is not necessary */
          for (unsigned int i = 0; i < decompSeqSize; i++)
          {
            normalizationState.HandleChar (decompSeq[i]);
          }
        }
        else
        {
          // Not decomposed.
          normalizationState.HandleChar (ch);
        }
      }
      normalizationState.Flush ();
      return normalizationState.str;
    }
  } // namespace uc
} // namespace s1
