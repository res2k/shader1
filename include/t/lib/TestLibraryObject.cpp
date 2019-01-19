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

// Hack to avoid <string_view> inclusion when building for "older" C++ standards
#include "base/config.h"
#if HAVE_STRING_VIEW
#undef HAVE_STRING_VIEW
#define HAVE_STRING_VIEW 0
#endif

#include "base/common.h"
#include "base/ResultCode_internal.h"
#include "s1/ResultCode_defs_base.h"

#include "TestLibraryObject.h"

#include "base/Library.h"
#include "base/LibraryObject.h"
#include "base/Object.h"

namespace impl
{
  class TestLO : public s1::LibraryObject
  {
  public:
    TestLO (s1::Library* lib) : LibraryObject (lib) {}
  };
} // namespace impl

TestLO* CreateTestLO (s1_Library* lib)
{
  impl::TestLO* new_obj (new impl::TestLO (s1::EvilUpcast<s1::Library> (lib)));
  new_obj->AddRef();
  return new_obj->DowncastEvil< ::TestLO> ();
}
