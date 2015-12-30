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

#include "codegen/common/UniqueIdentifiersSet.h"

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace codegen
  {
    static format::StaticFormatter FormatUniqueID ("{0}_{1}");

    uc::String UniqueIdentifiersSet::Get (const uc::String& identifier)
    {
      uc::String newIdent = identifier;
      unsigned int n = 0;
      while (seenIdentifiers.find (newIdent) != seenIdentifiers.end ())
      {
        newIdent = FormatUniqueID.to<uc::String> (identifier, ++n);
      }
      seenIdentifiers.insert (newIdent);
      return newIdent;
    }
  } // namespace codegen
} // namespace s1
