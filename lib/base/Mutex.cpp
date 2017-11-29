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

#include "base/common.h"
#include "base/Mutex.h"

#if defined(_MSC_VER) && defined(S1_PREFER_CRITICAL_SECTIONS)
#include <Windows.h>
#endif

namespace s1
{
#if defined(_MSC_VER) && defined(S1_PREFER_CRITICAL_SECTIONS)

  Mutex::Mutex ()
  {
    static_assert (sizeof(FakeCriticalSection) >= sizeof(CRITICAL_SECTION),
                   "FakeCriticalSection too small!");
    static_assert (alignof(FakeCriticalSection) >= alignof(CRITICAL_SECTION),
                   "FakeCriticalSection wrongly aligned!");
    InitializeCriticalSection (reinterpret_cast<CRITICAL_SECTION*> (&criticalSection));
  }

  Mutex::~Mutex ()
  {
    DeleteCriticalSection (reinterpret_cast<CRITICAL_SECTION*> (&criticalSection));
  }

  void Mutex::lock ()
  {
  #if defined(_DEBUG)
    // Catch recursive lock attempts
    auto currentThread = GetCurrentThreadId();
    bool alreadyLocking = lockingThread == currentThread;
    assert (!alreadyLocking);
  #endif
    EnterCriticalSection (reinterpret_cast<CRITICAL_SECTION*> (&criticalSection));
  #if defined(_DEBUG)
    lockingThread = currentThread;
  #endif
  }

  void Mutex::unlock ()
  {
  #if defined(_DEBUG)
    lockingThread = 0;
  #endif
    LeaveCriticalSection (reinterpret_cast<CRITICAL_SECTION*> (&criticalSection));
  }
#endif // defined(_MSC_VER) && defined(S1_PREFER_CRITICAL_SECTIONS)
} // namespace s1
