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

#ifndef __TESTOBJECT_H__
#define __TESTOBJECT_H__

#include "s1/Object.h"
#include "s1/Ptr.h"

#define TEST_MAKE_NAME(N) (N, (namespace cxxapi { class N; }, ::cxxapi::N))
#define TYPE_INFO_TestA   (TEST_MAKE_NAME(TestA), S1TYPE_INFO_s1_Object)
#define TYPE_INFO_TestA2  (TEST_MAKE_NAME(TestA2), TYPE_INFO_TestA)

S1TYPE_DECLARE(TYPE_INFO_TestA);
S1TYPE_DECLARE(TYPE_INFO_TestA2);

TestA* CreateTestA (int v);
int TestA_GetValue (TestA* obj);

TestA2* CreateTestA2 (int v, int v2);
int TestA2_GetValue2 (TestA2* obj);

#include "s1/warn_off.h"

namespace cxxapi
{
  class TestA : public s1::cxxapi::Object
  {
  public:
    typedef s1::Ptr<TestA> Pointer;

    static Pointer Create (int v)
    {
      ::TestA* p (CreateTestA (v));
      return S1_RETURN_MOVE_REF(TestA, p);
    }
    
    int GetValue ()
    {
      return TestA_GetValue (this);
    }
  };

  class TestA2 : public TestA
  {
  public:
    typedef s1::Ptr<TestA2> Pointer;

    static Pointer Create (int v, int v2)
    {
      ::TestA2* p (CreateTestA2 (v, v2));
      return S1_RETURN_MOVE_REF(TestA2, p);
    }
    
    int GetValue2 ()
    {
      return TestA2_GetValue2 (this);
    }
  };
} // namespace cxxapi

#include "s1/warn_on.h"

#endif // __TESTOBJECT_H__
