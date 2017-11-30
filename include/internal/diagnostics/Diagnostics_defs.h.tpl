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
/**\file
 * Diagnostic code values for: {{namespace}}
 */
#ifndef {{namespace.upper()}}_DIAGNOSTICS_H_
#define {{namespace.upper()}}_DIAGNOSTICS_H_

{{py:
# Use hash to obtain some (hopefully) unique base value for diagnostic codes
def hash16(x):
  return hash(x) & 0xffff
}}

#include "diagnostics/common.h"

namespace s1
{
  namespace {{namespace}}
  {
    /// Diagnostic code values for: {{namespace}}
    enum struct Error : unsigned int
    {
{{for comp_loop, comp in looper(components)}}
      /**\name Group: {{comp.attrib['name']}}
       * @{ */
{{for loop, error in looper(comp.findall('error'))}}
      /// {{error.find('descr').text}}
      {{error.attrib['id']}}{{if loop.first}} = S1_DIAGNOSTICS_ERRORS_BASE_VALUE({{comp.attrib['name']|hash16}}u){{endif}}{{if not (comp_loop.last and loop.last)}},{{endif}}
{{endfor}}
      /** @} */
{{endfor}}
    };
  } // namespace {{namespace}}
} // namespace s1

#endif // {{namespace.upper()}}_DIAGNOSTICS_H_

