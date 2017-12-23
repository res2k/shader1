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
# Strip leading & trailing spaces from all lines
def striplines(str):
  return "\n".join(map(lambda s: s.strip(), str.strip().splitlines()))

# Use hash to obtain some (hopefully) unique base value for diagnostic codes
def hash16(x):
  return hash(x) & 0xffff
}}

#include "diagnostics/common.h"

namespace s1
{
  namespace {{namespace}}
  {
    /// Diagnostic error code values for: {{namespace}}
    enum struct Error : unsigned int
    {
{{for comp_loop, comp in looper(components)}}
{{if len(comp.findall('error')) > 0}}
      /**\name Group: {{comp.attrib['name']}}
       * @{ */
{{for loop, error in looper(comp.findall('error'))}}
      {{if error.find('doc') == None}}/// {{error.find('descr').text}}{{else}}/**
       * {{error.find('descr').text}}
       * {{error.find('doc').text|striplines|autoindent}}
       */{{endif}}
      {{error.attrib['id']}}{{if loop.first}} = S1_DIAGNOSTICS_ERRORS_BASE_VALUE({{comp.attrib['name']|hash16}}u){{endif}}{{if not (comp_loop.last and loop.last)}},{{endif}}
{{endfor}}
      /** @} */
{{endif}}
{{endfor}}
    };

    /// Diagnostic warning code values for: {{namespace}}
    enum struct Warning : unsigned int
    {
{{for comp_loop, comp in looper(components)}}
{{if len(comp.findall('warning')) > 0}}
      /**\name Group: {{comp.attrib['name']}}
       * @{ */
{{for loop, warning in looper(comp.findall('warning'))}}
      {{if warning.find('doc') == None}}/// {{warning.find('descr').text}}{{else}}/**
       * {{warning.find('descr').text}}
       * {{warning.find('doc').text|striplines|autoindent}}
       */{{endif}}
      {{warning.attrib['id']}}{{if loop.first}} = S1_DIAGNOSTICS_WARNINGS_BASE_VALUE({{comp.attrib['name']|hash16}}u){{endif}}{{if not (comp_loop.last and loop.last)}},{{endif}}
{{endfor}}
      /** @} */
{{endif}}
{{endfor}}
    };
  } // namespace {{namespace}}
} // namespace s1

#endif // {{namespace.upper()}}_DIAGNOSTICS_H_

