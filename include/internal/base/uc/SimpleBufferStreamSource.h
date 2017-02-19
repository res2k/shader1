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
 * s1::uc::String::Source implementation on a simple byte buffer
 */
#ifndef __BASE_UC_SIMPLEBUFFERSTREAMSOURCE_H__
#define __BASE_UC_SIMPLEBUFFERSTREAMSOURCE_H__

#include "Stream.h"

namespace s1
{
  namespace uc
  {
    class SimpleBufferStreamSource : public Stream::Source
    {
      const char* inputData;
      size_t inputSize;
    public:
      SimpleBufferStreamSource (const char* inputData, size_t inputSize)
        : inputData (inputData), inputSize (inputSize) {}

      size_t NextData (const char*& data) override
      {
        size_t returnSize = inputSize;
        data = inputData;
        inputData = nullptr;
        inputSize = 0;
        return returnSize;
      }
    };
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_SIMPLEBUFFERSTREAMSOURCE_H__
