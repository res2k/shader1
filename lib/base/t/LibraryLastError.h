/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <cxxtest/TestSuite.h>

#include "base/ResultCode_internal.h"

#include "base/Library.h"

#define THREAD_A_CODE_1     S1_MAKE_ERROR(0xf, 1001)
#define THREAD_B_CODE_1     S1_MAKE_SUCCESS(0xf, 2001)

#define THREAD_A_CODE_2     S1_MAKE_SUCCESS(0xf, 1002)
#define THREAD_B_CODE_2     S1_MAKE_ERROR(0xf, 2002)

// Test that library last errors are stored per thread
class LibraryLastErrorTestSuite : public CxxTest::TestSuite 
{
  class Barrier
  {
    std::mutex m;
    std::condition_variable cv;
    unsigned int threshold;
    unsigned int count = 0;
    unsigned int sense = 0;
  public:
    Barrier (unsigned int threshold) : threshold (threshold) {}

    void wait ()
    {
      std::unique_lock<std::mutex> lock (m);
      auto localSense = sense;
      if (++count == threshold)
      {
        count = 0;
        ++sense;
        cv.notify_all ();
      }
      else
      {
        cv.wait (lock, [=](){ return sense != localSense; });
      }
    }
  };
public:
  void testLastError1 ()
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
    TS_ASSERT_EQUALS(codeA, THREAD_A_CODE_1);
    TS_ASSERT_EQUALS(codeB, THREAD_B_CODE_1);
  }

  void testLastError2 ()
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
    TS_ASSERT_EQUALS(codeA, THREAD_A_CODE_2);
    TS_ASSERT_EQUALS(codeB, THREAD_B_CODE_2);
  }
};
