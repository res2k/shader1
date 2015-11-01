//
// This file is auto-generated!
//
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

#include "s1/ResultCode.h"

#include "base/common.h"

#include "ResultCodeHelper.h"

{{def code_ident(name)}}S1_{{name}}{{enddef}}

const char* s1_get_result_code_str (s1_ResultCode code)
{
  switch (S1_CLEAR_EXTRA(code))
  {
  {{for comp in components}}
  {{for code in comp.getchildren()}}
    case {{code_ident(code.attrib['name'])}}:
    {{if 'name_ext' in code.attrib}}
      return s1::detail::GetExtendedErrorDescr (code,
        "{{code.find('descr').text}}");
    {{else}}
      return "{{code.find('descr').text}}";
    {{endif}}
  {{endfor}}
  {{endfor}}
  }
  return nullptr;
}
