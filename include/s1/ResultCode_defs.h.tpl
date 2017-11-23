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
 * Result code values definitions
 * \internal
 * Master header pulling in files with actual definitions
 */
#ifndef __S1_RESULTCODE_DEFS_H__
#define __S1_RESULTCODE_DEFS_H__

#if defined(S1_BUILD)
  #error "Include specific result code headers!"
#endif

#if !defined(DOXYGEN_RUN)
{{for comp in components}}
#include "ResultCode_defs_{{comp.attrib['name'].lower()}}.h"
{{endfor}}
#endif // !defined(DOXYGEN_RUN)

#endif // __S1_RESULTCODE_DEFS_H__
