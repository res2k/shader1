//
// This file is auto-generated!
//
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

#include "base/common.h"

#include "diagnostics/common.h"

{{py:
# Format component names as CamelCase
def camel_case(s):
  return ''.join(filter(lambda x: x.isalpha(), s.title()))

# Use hash to obtain some (hopefully) unique base value for diagnostic codes
def hash16(x):
  return hash(x) & 0xffff
}}

#define _EXTRACT_CODE_BASE(code)      ((code) >> 16)

namespace s1
{
  namespace diagnostics
  {
    namespace detail
    {
{{for comp in components}}
      const char* GetIdStringFor{{comp.attrib['name']|camel_case}} (unsigned int code);
{{endfor}}
    } // namespace detail

    const char* GetIdString (unsigned int code)
    {
      switch(_EXTRACT_CODE_BASE(code))
      {
{{for comp in components}}
      case {{comp.attrib['name']|hash16}}:
        return detail::GetIdStringFor{{comp.attrib['name']|camel_case}} (code);
{{endfor}}
      }
      return nullptr;
    }
  } //namespace diagnostics
} // namespace s1
