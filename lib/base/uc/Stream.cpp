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

#include "base/common.h"

#include "base/uc/Stream.h"

#include "base/uc/StreamException.h"
#include "base/uc/StreamInvalidCharacterException.h"
#include "base/uc/StreamEndOfInputException.h"

namespace s1
{
namespace uc
{
  //-------------------------------------------------------------------------
  
  Stream::Stream (Source& inSource)
   : streamInBufferRemaining (0), currentChar (InvalidChar32),
     currentDecodeResult (UTF8Decoder::drSuccess), inSource (inSource)
  {
    // Initial fill
    streamInBufferRemaining = inSource.NextData (streamInBufferPtr);
    // Go to first character
    if (*this) ++(*this);
  }
  
  Stream::~Stream()
  {
  }

  Stream::operator bool() const throw()
  {
    return
      (currentChar != InvalidChar32) // We have a current character
      || (streamInBufferRemaining > 0) // ... or still buffered chars to decode
      || (streamInBufferPtr != nullptr); // ... or still raw input data
  }

  Stream& Stream::operator++() throw()
  {
    do
    {
      const char* streamInBufferPrev = streamInBufferPtr;
      currentDecodeResult = decoder (streamInBufferPtr,
                                     streamInBufferPtr + streamInBufferRemaining,
                                     currentChar);
      bool bufferConsumed = streamInBufferPtr != streamInBufferPrev;
      streamInBufferRemaining -= (streamInBufferPtr - streamInBufferPrev);
      if (currentDecodeResult == UTF8Decoder::drInputUnderrun)
      {
        // Try to refill buffer
        if (!RefillBuffer())
        {
          if (bufferConsumed)
          {
            // Input undderun?
            // Refill failed?
            // Input buffer was consumed?
            // Means we have a trailing incomplete character
            currentDecodeResult = UTF8Decoder::drCharacterIncomplete;
            currentChar = ReplacementChar;
          }
          else
          {
            currentChar = InvalidChar32;
          }
          return *this;
        }
      }
    }
    while (currentDecodeResult < 0);
    return *this;
  }
  
  uc::Char32 Stream::operator* () const
  {
    if (currentDecodeResult != UTF8Decoder::drSuccess)
    {
      if ((currentDecodeResult == UTF8Decoder::drCharacterInvalid)
        || (currentDecodeResult == UTF8Decoder::drCharacterIncomplete)
        || (currentDecodeResult == UTF8Decoder::drEncodingInvalid))
      {
        throw StreamInvalidCharacterException (currentDecodeResult);
      }
      else if (currentDecodeResult == UTF8Decoder::drInputUnderrun)
      {
        throw StreamEndOfInputException ();
      }
      else
      {
        throw StreamException (currentDecodeResult);
      }
    }
    else if (!(*this))
    {
      throw StreamEndOfInputException ();
    }
    else
    {
      return currentChar;
    }
  }

  bool Stream::RefillBuffer ()
  {
    S1_ASSERT (streamInBufferRemaining == 0, true);

    if (!streamInBufferPtr)
    {
      // Assume source is out of data
      return false;
    }
      
    streamInBufferRemaining = inSource.NextData (streamInBufferPtr);
    if (streamInBufferRemaining == 0)
    {
      // Nothing was read, assume source is out of data
      streamInBufferPtr = nullptr;
      return false;
    }
    return true;
  }

} // namespace uc
} // namespace s1
