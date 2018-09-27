/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <boost/test/unit_test.hpp>

#include "lib/TestObject.h"

#define TEST_VAL_1      23
#define TEST_VAL_2      42

BOOST_AUTO_TEST_SUITE(ApiObject)

BOOST_AUTO_TEST_CASE(MemoryLayoutC)
{
  // Make sure object declarations produce the expected memory layout
  {
    TestA* test_a = (TestA*)0x100;
    s1_Object* test_obj = S1TYPE_CAST(s1_Object, test_a);
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_obj);
  }
  {
    TestA2* test_a2 = (TestA2*)0x100;
    TestA* test_a = S1TYPE_CAST(TestA, test_a2);
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_a2);
    s1_Object* test_obj1 = S1TYPE_CAST(s1_Object, test_a2);
    BOOST_CHECK_EQUAL((void*)test_a2, (void*)test_obj1);
    s1_Object* test_obj2 = S1TYPE_CAST(s1_Object, test_a);
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_obj2);
    BOOST_CHECK_EQUAL((void*)test_obj1, (void*)test_obj2);
  }
}

BOOST_AUTO_TEST_CASE(MemoryLayoutCXX)
{
  // Make sure object declarations produce the expected memory layout
  {
    cxxapi::TestA* test_a_cxx = (cxxapi::TestA*)0x100;
    TestA* test_a = test_a_cxx;
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_a_cxx);
    s1::Object* test_obj_cxx = test_a_cxx;
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_obj_cxx);
    BOOST_CHECK_EQUAL((void*)test_a_cxx, (void*)test_obj_cxx);
    s1_Object* test_obj = S1TYPE_CAST(s1_Object, test_a);
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_obj);
    BOOST_CHECK_EQUAL((void*)test_a_cxx, (void*)test_obj);
    BOOST_CHECK_EQUAL((void*)test_obj_cxx, (void*)test_obj);
  }
  {
    cxxapi::TestA2* test_a2_cxx = (cxxapi::TestA2*)0x100;
    cxxapi::TestA* test_a_cxx = test_a2_cxx;
    BOOST_CHECK_EQUAL((void*)test_a_cxx, (void*)test_a2_cxx);

    TestA2* test_a2 = test_a2_cxx;
    BOOST_CHECK_EQUAL((void*)test_a2, (void*)test_a2_cxx);
    TestA* test_a = S1TYPE_CAST(TestA, test_a2);
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_a2);
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_a_cxx);

    s1::Object* test_obj1_cxx = test_a2_cxx;
    BOOST_CHECK_EQUAL((void*)test_a2, (void*)test_obj1_cxx);
    BOOST_CHECK_EQUAL((void*)test_a2_cxx, (void*)test_obj1_cxx);
    s1_Object* test_obj1 = S1TYPE_CAST(s1_Object, test_a2);
    BOOST_CHECK_EQUAL((void*)test_a2, (void*)test_obj1);
    BOOST_CHECK_EQUAL((void*)test_obj1_cxx, (void*)test_obj1);

    s1::Object* test_obj2_cxx = test_a_cxx;
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_obj2_cxx);
    BOOST_CHECK_EQUAL((void*)test_a_cxx, (void*)test_obj2_cxx);
    s1_Object* test_obj2 = S1TYPE_CAST(s1_Object, test_a);
    BOOST_CHECK_EQUAL((void*)test_a, (void*)test_obj2);
    BOOST_CHECK_EQUAL((void*)test_obj2_cxx, (void*)test_obj2);

    BOOST_CHECK_EQUAL((void*)test_obj1, (void*)test_obj2);
    BOOST_CHECK_EQUAL((void*)test_obj1_cxx, (void*)test_obj2_cxx);
  }
}

BOOST_AUTO_TEST_CASE(CreateObjectC)
{
  // Check object creation
  TestA* test_a (CreateTestA (TEST_VAL_1));
  BOOST_CHECK_NE(test_a, (TestA*)0);
  int rc;
  rc = s1_get_ref_count(test_a);
  BOOST_CHECK_EQUAL(rc, 1);
  rc = s1_add_ref (test_a);
  BOOST_CHECK_EQUAL(rc, 2);

  BOOST_CHECK_EQUAL(TestA_GetValue (test_a), TEST_VAL_1);

  rc = s1_release (test_a);
  BOOST_CHECK_EQUAL(rc, 1);
  rc = s1_release (test_a);
  BOOST_CHECK_EQUAL(rc, 0);
}

BOOST_AUTO_TEST_CASE(CreateObjectCXX)
{
  // Check object creation
  cxxapi::TestA::Pointer test_a;
  test_a = cxxapi::TestA::Create (TEST_VAL_1);
  BOOST_CHECK_NE(test_a, (cxxapi::TestA*)0);
  int rc;
  rc = test_a->GetRefCount();
  BOOST_CHECK_EQUAL(rc, 1);

  BOOST_CHECK_EQUAL(test_a->GetValue(), TEST_VAL_1);
  BOOST_CHECK_EQUAL(TestA_GetValue (test_a), TEST_VAL_1);

  {
    cxxapi::TestA::Pointer test2 (test_a);
    rc = test_a->GetRefCount();
    BOOST_CHECK_EQUAL(rc, 2);
  }
  rc = test_a->GetRefCount();
  BOOST_CHECK_EQUAL(rc, 1);
}

BOOST_AUTO_TEST_CASE(CreateDerivedObjectC)
{
  // Check object creation
  TestA2* test_a2 (CreateTestA2 (TEST_VAL_1, TEST_VAL_2));
  BOOST_CHECK_NE(test_a2, (TestA2*)0);
  int rc;
  rc = s1_get_ref_count(test_a2);
  BOOST_CHECK_EQUAL(rc, 1);

  BOOST_CHECK_EQUAL(TestA_GetValue (S1TYPE_CAST(TestA, test_a2)), TEST_VAL_1);
  BOOST_CHECK_EQUAL(TestA2_GetValue2 (test_a2), TEST_VAL_2);

  rc = s1_release (test_a2);
  BOOST_CHECK_EQUAL(rc, 0);
}

BOOST_AUTO_TEST_CASE(CreateDerivedObjectCXX)
{
  // Check object creation
  cxxapi::TestA2::Pointer test_a2;
  test_a2 = cxxapi::TestA2::Create (TEST_VAL_1, TEST_VAL_2);
  BOOST_CHECK_NE(test_a2, (cxxapi::TestA2*)0);
  int rc;
  rc = test_a2->GetRefCount();
  BOOST_CHECK_EQUAL(rc, 1);

  BOOST_CHECK_EQUAL(test_a2->GetValue(), TEST_VAL_1);
  BOOST_CHECK_EQUAL(TestA_GetValue (S1TYPE_CAST(TestA, test_a2)), TEST_VAL_1);
  BOOST_CHECK_EQUAL(test_a2->GetValue2(), TEST_VAL_2);
  BOOST_CHECK_EQUAL(TestA2_GetValue2 (test_a2), TEST_VAL_2);

  {
    cxxapi::TestA2::Pointer test2 (test_a2);
    rc = test_a2->GetRefCount();
    BOOST_CHECK_EQUAL(rc, 2);
  }
  rc = test_a2->GetRefCount();
  BOOST_CHECK_EQUAL(rc, 1);
}

BOOST_AUTO_TEST_SUITE_END()
