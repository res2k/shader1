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

#ifndef __BASE_UC_STREAM_H__
#define __BASE_UC_STREAM_H__

#include "Char.h"
#include "UTF8Decoder.h"

namespace s1
{
namespace uc
{
  /**
   * Stream for Unicode characters from some encoded source.
   */
  class Stream
  {
  protected:
    /// UTF-8 decoder object
    UTF8Decoder decoder;
    
    const char* streamInBufferPtr;
    size_t streamInBufferRemaining;
    
    Char32 currentChar;
    UTF8Decoder::DecodeResult currentDecodeResult;
    
    /// Refill stream input buffer
    bool RefillBuffer ();
  public:
    /// Stream input source
    struct Source
    {
      virtual ~Source () {}
      /**
       * Get next available data.
       * \param data Reference to pointer receiving pointer to next data.
       * \returns Number of available bytes.
       *  Return 0 if the source is out of data.
       */
      virtual size_t NextData (const char*& data) = 0;
    };

    /**
     * Constructor.
     * \param inSource Input byte source.
     */
    Stream (Source& inSource);
    ~Stream();
    
    /// Returns whether more characters are available
    operator bool() const throw();
    bool operator!() const throw() { return !(bool)(*this); }

    /// Advance stream
    Stream& operator++() throw();
    
    /// Return current character
    Char32 operator* () const;
  protected:
    Source& inSource;
  private:
    Stream (const Stream& other); // forbidden
  };
} // namespace uc
} // namespace s1

#endif // __BASE_UC_STREAM_H__
