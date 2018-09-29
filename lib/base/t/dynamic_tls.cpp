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

#include "base/dynamic_tls.h"

#include <atomic>
#include <ctime>
#include <mutex>
#include <random>
#include <thread>

#include "Barrier.h"

BOOST_AUTO_TEST_SUITE(DynamicTls)

// Stress test: Lots of TLS allocated, deallocated in random order
BOOST_AUTO_TEST_CASE(Stress)
{
  const size_t handleCount = 2048; // Magic number: larger than internal pointer_tree block size

  std::vector<s1::dynamic_tls::Handle> tlsHandles (handleCount, s1::dynamic_tls::InvalidHandle);
  for (auto& handle : tlsHandles)
  {
    handle = s1::dynamic_tls::Alloc ();
  }

  std::mt19937 rng (static_cast<unsigned int> (std::time (0)));
  std::shuffle (tlsHandles.begin(), tlsHandles.end(), rng);

  unsigned int numThreads = 4;
  std::vector<std::thread> testThreads;
  testThreads.reserve (numThreads);
  std::mutex rngMutex;
  std::atomic<unsigned int> correctValues;
  correctValues.store (0);
  for (unsigned int i = 0; i < numThreads; i++)
  {
    testThreads.emplace_back (
      // Note: clang warns about handleCount; MSVC breaks w/o it
      [i, numThreads, handleCount, &rng, &rngMutex, &tlsHandles, &correctValues]()
      {
        for (unsigned int h = i; h < handleCount; h += numThreads)
        {
          uintptr_t testValue;
          unsigned int delay;
          {
            std::unique_lock<std::mutex> lockRNG (rngMutex);
            testValue = rng ();
            delay = rng ();
          }
          s1::dynamic_tls::SetValue (tlsHandles[h], reinterpret_cast<void*> (testValue));
          std::this_thread::sleep_for (std::chrono::nanoseconds (delay >> 12));
          uintptr_t testValue2 = reinterpret_cast<uintptr_t> (s1::dynamic_tls::GetValue (tlsHandles[h]));
          if (testValue == testValue2) correctValues.fetch_add (1);;
        }
      }
    );
  }

  for (auto& thread : testThreads)
  {
    thread.join();
  }

  BOOST_CHECK_EQUAL(correctValues.load(), handleCount);

  for (auto tlsHandle : tlsHandles)
  {
    s1::dynamic_tls::Free (tlsHandle);
  }
}

// Slot use-after-deallocate
BOOST_AUTO_TEST_CASE(UseAfterFree)
{
  auto tlsHandle = s1::dynamic_tls::Alloc ();
  s1::dynamic_tls::SetValue (tlsHandle, &tlsHandle);

  Barrier barrier (2);

  void* retrievedValue;
  std::thread useThread (
    [tlsHandle, &barrier, &retrievedValue]()
    {
      barrier.wait();
      retrievedValue = s1::dynamic_tls::GetValue (tlsHandle);
    }
  );
  s1::dynamic_tls::Free (tlsHandle);
  barrier.wait ();
  useThread.join();

  BOOST_CHECK_NE(retrievedValue, &tlsHandle);
  BOOST_CHECK_EQUAL(retrievedValue, nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
