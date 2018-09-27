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

#include "s1/Library.h"

#include "lib/TestLibraryObject.h"

BOOST_AUTO_TEST_SUITE(ApiLibraryObject)

BOOST_AUTO_TEST_CASE(CreateLibraryObjectC)
{
  s1_Library* lib;
  s1_ResultCode err = s1_create_library (&lib);
  BOOST_CHECK(S1_SUCCESSFUL(err));
  BOOST_CHECK_NE(lib, (s1_Library*)0);
  
  TestLO* lib_obj = CreateTestLO (lib);
  BOOST_CHECK(lib_obj);
  s1_Library* stored_lib (s1_libraryobject_get_library (S1TYPE_CAST(s1_LibraryObject, lib_obj)));
  BOOST_CHECK_EQUAL(stored_lib, lib);
  int rc = s1_release (lib_obj);
  BOOST_CHECK_EQUAL(rc, 0);

  rc = s1_release (lib);
  BOOST_CHECK_EQUAL(rc, 0);
}

BOOST_AUTO_TEST_CASE(CreateLibraryObjectCXX)
{
  s1::Library::Pointer lib;
  s1_ResultCode err = s1::Library::Create (lib);
  BOOST_CHECK(S1_SUCCESSFUL(err));
  BOOST_CHECK_NE(lib, (s1::Library*)0);
  int rc;
  rc = lib->GetRefCount();
  BOOST_CHECK_EQUAL(rc, 1);
  
  cxxapi::TestLO::Pointer lib_obj = cxxapi::TestLO::Create (lib);
  BOOST_CHECK(lib_obj);
  s1::Library::Pointer stored_lib (lib_obj->GetLibrary());
  BOOST_CHECK_EQUAL(stored_lib, lib);
}

BOOST_AUTO_TEST_SUITE_END()
