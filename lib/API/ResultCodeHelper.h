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

#ifndef __RESULTCODEHELPER_H__
#define __RESULTCODEHELPER_H__

#include "s1/ResultCode.h"

namespace s1
{
  namespace detail
  {
    /**
     * Provide an error description for an 'extended' error code
     * (includes some numerical extra info).
     */
    const char* GetExtendedErrorDescr (s1_ResultCode code, const char* baseDescr,
                                       const char* extDescr);
  } // namespace detail
} // namespace s1

#endif // __RESULTCODEHELPER_H__