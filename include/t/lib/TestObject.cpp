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

#include "TestObject.h"

#include "base/Object.h"

namespace impl
{
  class TestA : public s1::Object
  {
    int v;
  public:
    TestA (int v) : v (v) {}
    
    int GetValue() const { return v; }
  };
  
  class TestA2 : public TestA
  {
    int v2;
  public:
    TestA2 (int v, int v2) : TestA (v), v2 (v2) {}
    
    int GetValue2() const { return v2; }
  };
} // namespace impl

TestA* CreateTestA (int v)
{
  impl::TestA* new_obj (new impl::TestA (v));
  new_obj->AddRef();
  return new_obj->DowncastEvil< ::TestA> ();
}

int TestA_GetValue (TestA* obj)
{
  return s1::EvilUpcast<impl::TestA> (obj)->GetValue ();
}

TestA2* CreateTestA2 (int v, int v2)
{
  impl::TestA2* new_obj (new impl::TestA2 (v, v2));
  new_obj->AddRef();
  return new_obj->DowncastEvil< ::TestA2> ();
}

int TestA2_GetValue2 (TestA2* obj)
{
  return s1::EvilUpcast<impl::TestA2> (obj)->GetValue2 ();
}
