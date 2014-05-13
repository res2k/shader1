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

#include "base/UnicodeStream.h"
#include "base/UnicodeStreamException.h"
#include "base/UnicodeStreamInvalidCharacterException.h"
#include "base/UnicodeStreamEndOfInputException.h"

#include <assert.h>
#include <unicode/ucnv.h>

namespace s1
{
  void UnicodeStream::ICUError::handleFailure() const
  {
    throw UnicodeStreamException (errorCode);
  }
  
  //-------------------------------------------------------------------------
  
  UnicodeStream::UnicodeStream (std::istream& inStream, const char* encoding)
   : inStream (inStream), ucBufferRemaining (0),
     ucBufferEndError (U_ZERO_ERROR), streamInBufferRemaining (0),
     currentChar (noCharacter)
  {
    ICUError err;
    uconv = ucnv_open (encoding, err);
    err.assertSuccess();
    ucnv_setToUCallBack (uconv, UCNV_TO_U_CALLBACK_STOP, 0, 0, 0, err);
    err.assertSuccess();
    
    // Go to first character
    if (*this) ++(*this);
  }
  
  UnicodeStream::~UnicodeStream()
  {
    if (uconv) ucnv_close (uconv);
  }

  UnicodeStream::operator bool() const throw()
  {
    if (!uconv) return false;
    
    return
      (currentChar != noCharacter) // We have a current character
      || (ucBufferRemaining > 0) // ... or still chars in the internal UC buffer
      || !inStream.eof(); // ... or still raw input data
  }

  UnicodeStream& UnicodeStream::operator++() throw()
  {
    if (!uconv) return *this;
    
    currentError = U_ZERO_ERROR;
    uc::Char uc;
    if (!GetNextUChar (uc))
    {
      currentChar = noCharacter;
      return *this;
    }
    if (U_FAILURE (currentError))
    {
      /* Error occured earlier during conversion to Unicode,
	  fetching current char will raise that. */
      currentChar = errorCharacter;
      return *this;
    }
    if (U_IS_SURROGATE(uc))
    {
      uc::Char uc2;
      if (!GetNextUChar (uc2))
      {
        currentChar = noCharacter;
        return *this;
      }
      if (U_FAILURE (currentError))
      {
	// Give errors from ICU precedence
	currentChar = errorCharacter;
	return *this;
      }
      // Assume ICU always gives us a correct pair of surrogates
      assert(U_IS_SURROGATE_LEAD(uc));
      assert(U_IS_SURROGATE_TRAIL(uc2));
	
      currentChar = 0x10000 + ((uc & 0x03ff) << 10);
      currentChar |= (uc2 & 0x3ff);
    }
    else
      currentChar = uc;
    return *this;
  }
  
  uc::Char32 UnicodeStream::operator* () const
  {
    if (U_FAILURE(currentError))
    {
      if ((currentError == U_INVALID_CHAR_FOUND)
	|| (currentError == U_TRUNCATED_CHAR_FOUND)
	|| (currentError == U_ILLEGAL_CHAR_FOUND))
      {
	throw UnicodeStreamInvalidCharacterException (currentError);
      }
      else
      {
	throw UnicodeStreamException (currentError);
      }
    }
    else if (!(*this))
    {
      throw UnicodeStreamEndOfInputException ();
    }
    else
    {
      return currentChar;
    }
  }

  bool UnicodeStream::GetNextUChar (uc::Char& c)
  {
    if ((ucBufferRemaining == 0) && U_SUCCESS(ucBufferEndError))
    {
      if (!RefillUCBuffer ()) return false;
    }
    
    /* Don't use -1 as we want to, in case of a conversion error, _pretend_
       there is a character, but then sneakily throw an exception when it's
       tried to obtain it! */
    uc::Char32 ret = errorCharacter;
    if (ucBufferRemaining > 0)
    {
      ucBufferRemaining--;
      ret = *(ucBufferPtr++);
    }
    else
    {
      currentError = ucBufferEndError;
      // Clear end-of-buffer error so next call will refill buffer
      ucBufferEndError = U_ZERO_ERROR;
    }
    c = ret;
    return true;
  }
  
  /// Refill unicode buffer
  bool UnicodeStream::RefillUCBuffer ()
  {
    assert(ucBufferEndError == U_ZERO_ERROR);
    if (streamInBufferRemaining == 0)
    {
      if (!inStream.good())
      {
	return false;
      }
      
      inStream.read (streamInBuffer, sizeof (streamInBuffer));
      streamInBufferPtr = streamInBuffer;
      streamInBufferRemaining = size_t (inStream.gcount());
      
      if (streamInBufferRemaining == 0)
      {
	// Nothing was read
	return false;
      }
    }
    
    ICUError err;
    const char* source = streamInBufferPtr;
    uc::Char* target = ucBuffer;
    ucnv_toUnicode (uconv, &target, target + UCBufferSize,
		    &source, source + streamInBufferRemaining, 0,
		    !inStream.good(), err);
    ucBufferPtr = ucBuffer;
    ucBufferRemaining = target - ucBuffer;
    streamInBufferRemaining -= (source - streamInBufferPtr);
    streamInBufferPtr = source;
    ucBufferEndError = err;
    /* Since 'source' should point after the last byte consumed, it should
       also point beyond any troublesome input.
       Since we buffer as much uc::Chars as input bytes, a buffer overflow should
       _not_ occur. (A single byte expanding into a surrogate pair? Won't happen.)
     */

    return true;
  }

} // namespace s1
