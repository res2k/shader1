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

#ifndef __BASE_COMMON_H__
#define __BASE_COMMON_H__

/**\file
 * Common definitions
 */

#include "base/config.h"
#include <boost/cstdint.hpp> // must be include before any unicode/ headers to avoid macro conflict

/**\def S1_APPROX_DIGITS(Type)
 * Approximate number of characters needed to display a value of type
 * \a Type in decimal system.
 */
/* (sizeof(x)*25)/10+1 is an approximation of the number of characters
  * needed to display x in decimal system. (x can be at most 256^sizeof(x).
  * You need log10(256^sizeof(x)) characters, becoming
  * sizeof(x)*log10(256). 25/10 is an (over-)approximation of log10(256).) */
#define S1_APPROX_DIGITS(Type)          ((sizeof(Type) * 25) / 10)
static const size_t charsToFormatUint = S1_APPROX_DIGITS(unsigned int);
// Add 1 for '-'
static const size_t charsToFormatInt = S1_APPROX_DIGITS(unsigned int) + 1;

// Used all over the place
#include "uc/String.h"

// Debugging helpers
#include <assert.h>
#define S1_ASSERT_RET_VOID
#define S1_ASSERT(x, ret)            { assert (x); if (!(x)) { return ret; } }
// TODO: Implement custom assertion messages
#define S1_ASSERT_MSG(x, msg, ret)   S1_ASSERT(x, ret)

#include "nullptr.h"

#endif // __BASE_COMMON_H__
