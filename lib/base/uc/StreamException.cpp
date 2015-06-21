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

#include "base/common.h"

#include "base/uc/StreamException.h"

namespace s1
{
  namespace uc
  {
    const char* StreamException::what () const throw()
    {
      switch(result)
      {
      case UTF8Decoder::drSuccess:              return "success";
      case UTF8Decoder::drInputUnderrun:        return "input underrun";
      case UTF8Decoder::drCharacterIncomplete:  return "character incomplete";
      case UTF8Decoder::drCharacterInvalid:     return "character invalid";
      case UTF8Decoder::drEncodingInvalid:      return "encoding invalid";
      }
      return "StreamException";
    }
  } // namespace uc
} // namespace s1
