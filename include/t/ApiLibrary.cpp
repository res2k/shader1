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

BOOST_AUTO_TEST_SUITE(ApiLibrary)

BOOST_AUTO_TEST_CASE(CreateLibraryC)
{
  // Check object creation
  s1_Library* lib;
  s1_ResultCode err = s1_create_library (&lib);
  BOOST_CHECK(S1_SUCCESSFUL(err));
  BOOST_CHECK_NE(lib, (s1_Library*)0);
  int rc;
  rc = s1_get_ref_count(lib);
  BOOST_CHECK_EQUAL(rc, 1);
  rc = s1_release (lib);
  BOOST_CHECK_EQUAL(rc, 0);
}

BOOST_AUTO_TEST_CASE(WrongCreateLibraryC)
{
  s1_ResultCode err = s1_create_library (0);
  BOOST_CHECK(S1_ERROR_EQUAL(err, S1_E_INVALID_ARG));
}

BOOST_AUTO_TEST_CASE(CreateLibraryCXX)
{
  // Check object creation
  s1::Library::Pointer lib;
  s1_ResultCode err = s1::Library::Create (lib);
  BOOST_CHECK(S1_SUCCESSFUL(err));
  BOOST_CHECK_NE(lib, (s1::Library*)0);
  int rc;
  rc = lib->GetRefCount();
  BOOST_CHECK_EQUAL(rc, 1);
}

BOOST_AUTO_TEST_SUITE_END()
