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

#include "base/uc/Char.h"

#include "UCDDataLookup.h"

namespace s1
{
  namespace uc
  {
    #include "ucd/White_Space.inc"
    #include "ucd/XID_Continue.inc"
    #include "ucd/XID_Start.inc"

    bool IsWhiteSpace (Char32 ch)
    {
      return IsCharInSet (ch, ucd_White_Space);
    }

    bool IsIDStart (Char32 ch)
    {
      return IsCharInSet (ch, ucd_XID_Start);
    }

    bool IsIDContinue (Char32 ch)
    {
      return IsCharInSet (ch, ucd_XID_Continue);
    }

  } // namespace uc
} // namespace s1
