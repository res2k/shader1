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
 * Utility: Barrier synchronization
 */
#ifndef BASE_T_BARRIER_H_
#define BASE_T_BARRIER_H_

#include <condition_variable>
#include <mutex>
#include <thread>

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

#endif // BASE_T_BARRIER_H_
