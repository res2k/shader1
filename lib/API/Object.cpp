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

#include "base/Object.h"

#include "base/common.h"

#undef s1_add_ref
#undef s1_release
#undef s1_get_ref_count

int s1_add_ref (s1_Object* obj)
{
  S1_ASSERT_MSG(obj, "NULL Object", 0);
  return s1::EvilUpcast<s1::Object> (obj)->AddRef();
}

int s1_release (s1_Object* obj)
{
  S1_ASSERT_MSG(obj, "NULL Object", 0);
  return s1::EvilUpcast<s1::Object> (obj)->Release();
}

int s1_get_ref_count (s1_Object* obj)
{
  S1_ASSERT_MSG(obj, "NULL Object", 0);
  return s1::EvilUpcast<s1::Object> (obj)->GetRefCount();
}
