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

#include "base/common.h"

#include "base/uc/Exception.h"
#include "base/uc/String.h"

#include <unicode/errorcode.h>
#include <unicode/normlzr.h>

namespace s1
{
  namespace uc
  {
    String String::Normalized (NormalizationMode mode)
    {
      UNormalizationMode icu_mode (UNORM_NONE);
      switch (mode)
      {
      case normNFD: icu_mode = UNORM_NFD;       break;
      }
      ErrorCode err;
      UnicodeString strN;
      Normalizer::normalize (*this, icu_mode, 0, strN, err);
      if (err > 0) throw Exception (err);
      return String (strN);
    }
  } // namespace uc
} // namespace s1
