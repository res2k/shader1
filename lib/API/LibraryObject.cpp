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
#include "base/ResultCode_internal.h"

#include "s1/LibraryObject.h"

#include "base/LibraryObject.h"

#include <new>

s1_Library* s1_libraryobject_get_library (s1_LibraryObject* obj)
{
  S1_ASSERT_MSG(obj, "NULL LibraryObject", nullptr);
  s1::LibraryObject* libObj (s1::EvilUpcast<s1::LibraryObject> (obj));
  return libObj->GetLibrary()->DowncastEvil<s1_Library> ();
}
