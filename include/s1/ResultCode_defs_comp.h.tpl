//
// This file is auto-generated!
//
/*
    Shader1
    Copyright (c) 2015-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/
{{def comp_ident(comp)}}S1_RESULT_COMP_{{comp}}{{enddef}}
{{def code_ident(name)}}S1_{{name}}{{enddef}}
{{def code_make_def(comp, tag)}}{{if 'type' in tag.attrib and tag.attrib['type'] == 'success'}}S1_MAKE_SUCCESS{{else}}S1_MAKE_ERROR{{endif}}({{comp_ident(comp)}}, {{tag.attrib['value']}}){{enddef}}
{{def code_make_def_ext(comp, tag, ext)}}{{if 'type' in tag.attrib and tag.attrib['type'] == 'success'}}S1_MAKE_SUCCESS_X{{else}}S1_MAKE_ERROR_X{{endif}}({{comp_ident(comp)}}, {{tag.attrib['value']}}, {{ext}}){{enddef}}
{{py:
# Format a string for multi-line doxygen comment
def striplines(str):
  return "\n".join(map(lambda s: s.strip(), str.strip().splitlines()))
}}

{{for comp in components}}
/*
 * Result code values for: {{comp.attrib['docgroup']}}
 */
#ifndef __S1_RESULTCODE_DEFS_{{comp.attrib['name']}}_H__
#define __S1_RESULTCODE_DEFS_{{comp.attrib['name']}}_H__

/**\name {{comp.attrib['docgroup']}}
 * @{ */
/**\def {{comp_ident(comp.attrib['name'])}}
 * \hideinitializer
 * Result component: {{comp.attrib['descr']}}
 */
#define {{comp_ident(comp.attrib['name'])}} {{comp.attrib['value']}}u

{{for code in comp.getchildren()}}
/**\def {{code_ident(code.attrib['name'])}}
 * \hideinitializer
 * {{if code.find('doc') != None}}{{code.find('doc').text|striplines|autoindent}}{{else}}{{code.find('descr').text|striplines|autoindent}}{{endif}}
 */
#define {{code_ident(code.attrib['name'])}} {{code_make_def(comp.attrib['name'], code)}}
{{if 'name_ext' in code.attrib}}
/**\def {{code_ident(code.attrib['name_ext'])}}(N)
 * \hideinitializer
 * {{if code.find('doc_ext') != None}}{{code.find('doc_ext').text|striplines|autoindent}}{{else}}{{code.find('descr').text|striplines|autoindent}}{{endif}}
 */
#define {{code_ident(code.attrib['name_ext'])}}(N) {{code_make_def_ext(comp.attrib['name'], code, '((unsigned)N) + 1')}}
{{endif}}
{{endfor}}
/** @} */

#endif // __S1_RESULTCODE_DEFS_{{comp.attrib['name']}}_H__
{{endfor}}

