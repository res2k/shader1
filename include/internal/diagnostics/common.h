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
 * Common diagnostics definitions
 */
#ifndef S1_DIAGNOSTICS_COMMON_H_
#define S1_DIAGNOSTICS_COMMON_H_

/// Compute a base value for diagnostics enums
#define S1_DIAGNOSTICS_ERRORS_BASE_VALUE(val)       (val << 16) + 1
// TODO...
//#define S1_DIAGNOSTICS_WARNINGS_BASE_VALUE(val)     (val << 16) + 0x8000 + 1

#endif // S1_DIAGNOSTICS_COMMON_H_
