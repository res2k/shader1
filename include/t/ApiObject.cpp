#include <cxxtest/TestSuite.h>

#include "lib/TestObject.h"

#define TEST_VAL_1      23
#define TEST_VAL_2      42

class ApiObjectTestSuite : public CxxTest::TestSuite 
{
public:
  void testMemoryLayoutC (void)
  {
    // Make sure object declarations produce the expected memory layout
    {
      TestA* test_a = (TestA*)0x100;
      s1_Object* test_obj = S1TYPE_CAST(test_a, s1_Object);
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_obj);
    }
    {
      TestA2* test_a2 = (TestA2*)0x100;
      TestA* test_a = S1TYPE_CAST(test_a2, TestA);
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_a2);
      s1_Object* test_obj1 = S1TYPE_CAST(test_a2, s1_Object);
      TS_ASSERT_EQUALS((void*)test_a2, (void*)test_obj1);
      s1_Object* test_obj2 = S1TYPE_CAST(test_a, s1_Object);
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_obj2);
      TS_ASSERT_EQUALS((void*)test_obj1, (void*)test_obj2);
    }
  }

  void testMemoryLayoutCXX (void)
  {
    // Make sure object declarations produce the expected memory layout
    {
      cxxapi::TestA* test_a_cxx = (cxxapi::TestA*)0x100;
      TestA* test_a = test_a_cxx->Cpointer();
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_a_cxx);
      s1::Object* test_obj_cxx = test_a_cxx;
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_obj_cxx);
      TS_ASSERT_EQUALS((void*)test_a_cxx, (void*)test_obj_cxx);
      s1_Object* test_obj = S1TYPE_CAST(test_a, s1_Object);
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_obj);
      TS_ASSERT_EQUALS((void*)test_a_cxx, (void*)test_obj);
      TS_ASSERT_EQUALS((void*)test_obj_cxx, (void*)test_obj);
    }
    {
      cxxapi::TestA2* test_a2_cxx = (cxxapi::TestA2*)0x100;
      cxxapi::TestA* test_a_cxx = test_a2_cxx;
      TS_ASSERT_EQUALS((void*)test_a_cxx, (void*)test_a2_cxx);
      
      TestA2* test_a2 = test_a2_cxx->Cpointer();
      TS_ASSERT_EQUALS((void*)test_a2, (void*)test_a2_cxx);
      TestA* test_a = S1TYPE_CAST(test_a2, TestA);
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_a2);
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_a_cxx);

      s1::Object* test_obj1_cxx = test_a2_cxx;
      TS_ASSERT_EQUALS((void*)test_a2, (void*)test_obj1_cxx);
      TS_ASSERT_EQUALS((void*)test_a2_cxx, (void*)test_obj1_cxx);
      s1_Object* test_obj1 = S1TYPE_CAST(test_a2, s1_Object);
      TS_ASSERT_EQUALS((void*)test_a2, (void*)test_obj1);
      TS_ASSERT_EQUALS((void*)test_obj1_cxx, (void*)test_obj1);
      
      s1::Object* test_obj2_cxx = test_a_cxx;
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_obj2_cxx);
      TS_ASSERT_EQUALS((void*)test_a_cxx, (void*)test_obj2_cxx);
      s1_Object* test_obj2 = S1TYPE_CAST(test_a, s1_Object);
      TS_ASSERT_EQUALS((void*)test_a, (void*)test_obj2);
      TS_ASSERT_EQUALS((void*)test_obj2_cxx, (void*)test_obj2);
      
      TS_ASSERT_EQUALS((void*)test_obj1, (void*)test_obj2);
      TS_ASSERT_EQUALS((void*)test_obj1_cxx, (void*)test_obj2_cxx);
    }
  }

  void testCreateObjectC (void)
  {
    // Check object creation
    TestA* test_a (CreateTestA (TEST_VAL_1));
    TS_ASSERT_DIFFERS(test_a, (TestA*)0);
    int rc;
    rc = s1_get_ref_count(test_a);
    TS_ASSERT_EQUALS(rc, 1);
    rc = s1_add_ref (test_a);
    TS_ASSERT_EQUALS(rc, 2);
    
    TS_ASSERT_EQUALS(TestA_GetValue (test_a), TEST_VAL_1);

    rc = s1_release (test_a);
    TS_ASSERT_EQUALS(rc, 1);
    rc = s1_release (test_a);
    TS_ASSERT_EQUALS(rc, 0);
  }

  void testCreateObjectCXX (void)
  {
    // Check object creation
    cxxapi::TestA::Pointer test_a;
    test_a = cxxapi::TestA::Create (TEST_VAL_1);
    TS_ASSERT_DIFFERS(test_a, (cxxapi::TestA*)0);
    int rc;
    rc = test_a->GetRefCount();
    TS_ASSERT_EQUALS(rc, 1);

    TS_ASSERT_EQUALS(test_a->GetValue(), TEST_VAL_1);
    TS_ASSERT_EQUALS(TestA_GetValue (test_a->Cpointer()), TEST_VAL_1);
    
    {
      cxxapi::TestA::Pointer test2 (test_a);
      rc = test_a->GetRefCount();
      TS_ASSERT_EQUALS(rc, 2);
    }
    rc = test_a->GetRefCount();
    TS_ASSERT_EQUALS(rc, 1);
  }

  void testCreateDerivedObjectC (void)
  {
    // Check object creation
    TestA2* test_a2 (CreateTestA2 (TEST_VAL_1, TEST_VAL_2));
    TS_ASSERT_DIFFERS(test_a2, (TestA2*)0);
    int rc;
    rc = s1_get_ref_count(test_a2);
    TS_ASSERT_EQUALS(rc, 1);
    
    TS_ASSERT_EQUALS(TestA_GetValue (S1TYPE_CAST(test_a2, TestA)), TEST_VAL_1);
    TS_ASSERT_EQUALS(TestA2_GetValue2 (test_a2), TEST_VAL_2);

    rc = s1_release (test_a2);
    TS_ASSERT_EQUALS(rc, 0);
  }

  void testCreateDerivedObjectCXX (void)
  {
    // Check object creation
    cxxapi::TestA2::Pointer test_a2;
    test_a2 = cxxapi::TestA2::Create (TEST_VAL_1, TEST_VAL_2);
    TS_ASSERT_DIFFERS(test_a2, (cxxapi::TestA2*)0);
    int rc;
    rc = test_a2->GetRefCount();
    TS_ASSERT_EQUALS(rc, 1);

    TS_ASSERT_EQUALS(test_a2->GetValue(), TEST_VAL_1);
    TS_ASSERT_EQUALS(TestA_GetValue (S1TYPE_CAST(test_a2->Cpointer(), TestA)), TEST_VAL_1);
    TS_ASSERT_EQUALS(test_a2->GetValue2(), TEST_VAL_2);
    TS_ASSERT_EQUALS(TestA2_GetValue2 (test_a2->Cpointer()), TEST_VAL_2);
    
    {
      cxxapi::TestA2::Pointer test2 (test_a2);
      rc = test_a2->GetRefCount();
      TS_ASSERT_EQUALS(rc, 2);
    }
    rc = test_a2->GetRefCount();
    TS_ASSERT_EQUALS(rc, 1);
  }
};
