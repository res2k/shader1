/*
    Shader1
    Copyright (c) 2017-2018 Frank Richter


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

#include "base/ResultCode_internal.h"

#include "base/Library.h"

#include "Barrier.h"

#define THREAD_A_CODE_1     S1_MAKE_ERROR(0xf, 1001)
#define THREAD_B_CODE_1     S1_MAKE_SUCCESS(0xf, 2001)

#define THREAD_A_CODE_2     S1_MAKE_SUCCESS(0xf, 1002)
#define THREAD_B_CODE_2     S1_MAKE_ERROR(0xf, 2002)

// Test that library last errors are stored per thread
BOOST_AUTO_TEST_SUITE(LibraryLastError)

BOOST_AUTO_TEST_CASE(LastError1)
{
  s1::Library lib;

  Barrier barrier (2);

  s1_ResultCode codeA = 0, codeB = 0;
  std::thread threadA (
    [&]()
    {
      lib.SetLastError (THREAD_A_CODE_1);
      barrier.wait ();
      codeA = lib.GetLastError ();
      barrier.wait ();
    }
  );
  std::thread threadB (
    [&]()
    {
      barrier.wait ();
      lib.SetLastError (THREAD_B_CODE_1);
      barrier.wait ();
      codeB = lib.GetLastError ();
    }
  );
  threadA.join();
  threadB.join();
  BOOST_CHECK_EQUAL(codeA, THREAD_A_CODE_1);
  BOOST_CHECK_EQUAL(codeB, THREAD_B_CODE_1);
}

BOOST_AUTO_TEST_CASE(LastError2)
{
  s1::Library lib;

  Barrier barrier (2);

  s1_ResultCode codeA = 0, codeB = 0;
  std::thread threadA (
    [&]()
    {
      barrier.wait ();
      lib.SetLastError (THREAD_A_CODE_2);
      barrier.wait ();
      codeA = lib.GetLastError ();
    }
  );
  std::thread threadB (
    [&]()
    {
      lib.SetLastError (THREAD_B_CODE_2);
      barrier.wait ();
      codeB = lib.GetLastError ();
      barrier.wait ();
    }
  );
  threadA.join();
  threadB.join();
  BOOST_CHECK_EQUAL(codeA, THREAD_A_CODE_2);
  BOOST_CHECK_EQUAL(codeB, THREAD_B_CODE_2);
}

BOOST_AUTO_TEST_SUITE_END()
