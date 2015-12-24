/*
Shader1
Copyright (c) 2015 Frank Richter


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
#include "base/ResultCode_internal.h"

#include "compiler/Backend.h"

namespace s1
{
  std::string Compiler::Backend::FlattenStringArray (const codegen::StringsArrayPtr& strings)
  {
    std::string str;
    for (size_t i = 0; i < strings->Size(); i++)
    {
      str.append (strings->Get (i));
      str.append ("\n");
    }
    return str;
  }

  //-------------------------------------------------------------------------

  Compiler::Backend::Options::FlagPair Compiler::Backend::Options::ParseFlagPair (const uc::String& string)
  {
    if (string.startsWith ("no-"))
    {
      return std::make_pair (uc::String (string.data() + 3, string.length() - 3), false);
    }
    else
    {
      return std::make_pair (string, true);
    }
  }
} // namespace s1
