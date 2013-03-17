#ifndef __TESTOBJECT_H__
#define __TESTOBJECT_H__

#include "s1/Object.h"
#include "s1/Ptr.h"

#define TYPE_INFO_TestA   (TestA, S1TYPE_INFO_s1_Object)
#define TYPE_INFO_TestA2  (TestA2, TYPE_INFO_TestA)

S1TYPE_DECLARE(TYPE_INFO_TestA);
S1TYPE_DECLARE(TYPE_INFO_TestA2);

TestA* CreateTestA (int v);
int TestA_GetValue (TestA* obj);

TestA2* CreateTestA2 (int v, int v2);
int TestA2_GetValue2 (TestA2* obj);

namespace cxxapi
{
  class TestA : public ::TestA,
                public s1::cxxapi::Object
  {
  public:
    typedef s1::Ptr<TestA> Pointer;

    static Pointer Create (int v)
    {
      ::TestA* p (CreateTestA (v));
      return Pointer (static_cast<TestA*> (p), Pointer::TakeReference ());
    }
    
    int GetValue ()
    {
      return TestA_GetValue (this);
    }
  };

  class TestA2 : public ::TestA2,
                 public TestA
  {
  public:
    typedef s1::Ptr<TestA2> Pointer;

    static Pointer Create (int v, int v2)
    {
      ::TestA2* p (CreateTestA2 (v, v2));
      return Pointer (static_cast<TestA2*> (p), Pointer::TakeReference ());
    }
    
    int GetValue2 ()
    {
      return TestA2_GetValue2 (this);
    }
  };
} // namespace cxxapi

#endif // __TESTOBJECT_H__
