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

/**\file
 * Mutex wrapper
 */
#ifndef BASE_MUTEX_H_
#define BASE_MUTEX_H_

#include <mutex>

namespace s1
{
#if defined(_MSC_VER) && defined(S1_PREFER_CRITICAL_SECTIONS)
  /* Use a Win32 CRITICAL_SECTION instead of std::mutex.
   * On MSVC, std::mutex pulls in the Concurrency Runtime which is rather
   * heavyweight for 'just' a mutex.
   * CRITICAL_SECTIONs have virtually no dependencies and work just
   * as well. Caveat: They're not recursive! */
  class Mutex
  {
    struct FakeCriticalSection
    {
      void* p0;
      uint32_t l0, l1;
      void* p1;
      void* p2;
      uintptr_t uip0;
    };
    FakeCriticalSection criticalSection;
  #if defined(_DEBUG)
    uint32_t lockingThread = 0;
  #endif
  public:
    Mutex ();
    Mutex (const Mutex&) = delete;
    Mutex (Mutex&&) = delete;
    ~Mutex ();

    Mutex& operator= (const Mutex&) = delete;
    Mutex& operator= (Mutex&&) = delete;

    void lock ();
    void unlock ();
  };
#else
  typedef std::mutex Mutex;
#endif
} // namespace s1

#endif // BASE_MUTEX_H_
