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
#include "{{namespace}}/Diagnostics.h"

{{py:
# Format component names as CamelCase
def camel_case(s):
  return ''.join(filter(lambda x: x.isalpha(), s.title()))
}}

namespace s1
{
  namespace diagnostics
  {
    namespace detail
    {
{{for comp in components}}
      const char* GetDescriptionStringFor{{comp.attrib['name']|camel_case}} (unsigned int code)
      {
        switch (code)
        {
{{for loop, error in looper(comp.findall('error'))}}
        case static_cast<unsigned int> ({{namespace}}::Error::{{error.attrib['id']}}):
          return "{{error.find('descr').text}}";
{{endfor}}
{{for loop, warning in looper(comp.findall('warning'))}}
        case static_cast<unsigned int> ({{namespace}}::Warning::{{warning.attrib['id']}}):
          return "{{warning.find('descr').text}}";
{{endfor}}
        }
        return nullptr;
      }

{{endfor}}
    } // namespace detail
  } //namespace diagnostics
} // namespace s1
