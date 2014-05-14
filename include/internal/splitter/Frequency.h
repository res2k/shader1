/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __SPLITTER_FREQUENCY_H__
#define __SPLITTER_FREQUENCY_H__

namespace s1
{
  namespace splitter
  {
    enum Frequency
    {
      freqUniform,
      freqVertex,
      freqFragment,
      
      freqNum,
      freqHighest = freqNum - 1
    };
    enum
    {
      freqFlagU = 1 << freqUniform,
      freqFlagV = 1 << freqVertex,
      freqFlagF = 1 << freqFragment,
    };
  } // namespace splitter
} // namespace s1

#endif // __SPLITTER_FREQUENCY_H__
