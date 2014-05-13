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

#ifndef __BASE_UNICODESTREAM_H__
#define __BASE_UNICODESTREAM_H__

#include "base/uc/Char.h"

#include <unicode/errorcode.h>
#include <unicode/utypes.h>

#include <istream>

struct UConverter;

namespace s1
{
  /**
   * Stream for Unicode characters from some encoded source.
   */
  class UnicodeStream
  {
  protected:
    class ICUError : public U_NAMESPACE_QUALIFIER ErrorCode
    {
    public:
      virtual void handleFailure() const;
    #if (U_ICU_VERSION_MAJOR_NUM * 100 + U_ICU_VERSION_MINOR_NUM) < 404
      // Source compatibility w/ ICU 4.2
      inline void assertSuccess() const { check(); }
    #endif
    };
    
    std::istream& inStream;
    /// ICU converter object
    UConverter* uconv;
    
    enum
    {
      /// Size of internal unicode characters buffer
      UCBufferSize = 1024
    };
    uc::Char ucBuffer[UCBufferSize];
    
    const uc::Char* ucBufferPtr;
    size_t ucBufferRemaining;
    /**
     * ICU error that occured during filling the buffer.
     * The buffer is filled up until an error occured. So the error needs to
     * be propagated only once we reach the end of the internal buffer.
     */
    UErrorCode ucBufferEndError;
    
    /**
     * Internal buffer for data from input stream.
     * Stored to allow resuming after ICU reports an error.
     */
    char streamInBuffer[UCBufferSize];
    const char* streamInBufferPtr;
    size_t streamInBufferRemaining;
    
    uc::Char32 currentChar;
    UErrorCode currentError;
    enum
    {
      errorCharacter = 0xFFFF,
      noCharacter = -1
    };
    
    /// Get next UTF-16 character from buffer, refill if necessary
    bool GetNextUChar (uc::Char& c);
    /// Refill unicode buffer
    bool RefillUCBuffer ();
  public:
    /**
     * Constructor.
     * \param inStream Input byte stream.
     * \param encoding ICU encoding name of the input.
     */
    UnicodeStream (std::istream& inStream, const char* encoding);
    ~UnicodeStream();
    
    /// Returns whether more characters are available
    operator bool() const throw();
    bool operator!() const throw() { return !(bool)(*this); }

    /// Advance stream
    UnicodeStream& operator++() throw();
    
    /// Return current character
    uc::Char32 operator* () const;
  private:
    UnicodeStream (const UnicodeStream& other); // forbidden
  };
  
} // namespace s1

#endif // __BASE_UNICODESTREAM_H__
