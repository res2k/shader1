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

#include "base/common.h"

#include "s1/cxxapi_detail.h"

#include "base/DebugMessageHandler.h"
#include "base/format/Formatter.h"
#include "base/format/uc_String.h"

#include "base/format/Formatter.tpp"

static s1::format::StaticFormatter FormatUnhandledException ("Unhandled exception in {0}: {1}");
static s1::format::StaticFormatter FormatUnhandledExceptionWithFunc ("Unhandled exception in {0} ({1}): {2}");

void _s1_print_unhandled_exception (const char* locationDescription, const char* function, const char* what)
{
  s1::uc::String whatStr;
  if (what)
  {
    whatStr = s1::uc::String::fromLocal8Bit (what);
  }
  else
  {
    whatStr = "<unknown exception>";
  }
  s1::uc::String msg;
  if (function)
  {
    msg = FormatUnhandledExceptionWithFunc.to<s1::uc::String> (locationDescription, function, whatStr);
  }
  else
  {
    msg = FormatUnhandledException.to<s1::uc::String> (locationDescription, whatStr);
  }
  s1::PrintDebugMessage (msg);
}
