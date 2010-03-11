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
     ucBufferEndError (U_ZERO_ERROR), currentChar (noCharacter),
     streamInBufferRemaining (0)
  {
    ICUError err;
    uconv = ucnv_open (encoding, err);
    err.check();
    ucnv_setToUCallBack (uconv, UCNV_TO_U_CALLBACK_STOP, 0, 0, 0, err);
    err.check();
    
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
    try
    {
      UChar uc = GetNextUChar ();
      if (U_FAILURE (currentError))
      {
	/* Error occured earlier during conversion to Unicode,
	   fetching current char will raise that. */
	currentChar = errorCharacter;
	return *this;
      }
      if (U_IS_SURROGATE(uc))
      {
	UChar32 uc32 = 0;
	if (U_IS_SURROGATE_LEAD(uc))
	{
	  uc32 = 0x10000 + ((uc & 0x03ff) << 10);
	  uc = GetNextUChar ();
	  if (U_FAILURE (currentError))
	  {
	    // Give errors from ICU precedence
	    currentChar = errorCharacter;
	    return *this;
	  }
	  if (U_IS_SURROGATE_TRAIL(uc))
	  {
	    uc32 |= (uc & 0x3ff);
	    currentChar = uc32;
	  }
	  else
	  {
	    currentChar = errorCharacter;
	    currentError = U_ILLEGAL_CHAR_FOUND;
	  }
	}
	else
	{
	  currentChar = errorCharacter;
	  currentError = U_ILLEGAL_CHAR_FOUND;
	}
      }
      else
	currentChar = uc;
    }
    catch (UnicodeStreamEndOfInputException&)
    {
      currentChar = noCharacter;
    }
    return *this;
  }
  
  UChar32 UnicodeStream::operator* () const
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

  UChar UnicodeStream::GetNextUChar ()
  {
    if ((ucBufferRemaining == 0) && U_SUCCESS(ucBufferEndError))
    {
      RefillUCBuffer ();
    }
    
    /* Don't use -1 as we want to, in case of a conversion error, _pretend_
       there is a character, but then sneakily throw an exception when it's
       tried to obtain it! */
    UChar32 ret = errorCharacter;
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
    return ret;
  }
  
  /// Refill unicode buffer
  void UnicodeStream::RefillUCBuffer ()
  {
    assert(ucBufferEndError == U_ZERO_ERROR);
    if (streamInBufferRemaining == 0)
    {
      if (!inStream.good())
      {
	throw UnicodeStreamEndOfInputException ();
      }
      
      inStream.read (streamInBuffer, sizeof (streamInBuffer));
      streamInBufferPtr = streamInBuffer;
      streamInBufferRemaining = inStream.gcount();
      
      if (streamInBufferRemaining == 0)
      {
	// Nothing was read
	throw UnicodeStreamEndOfInputException ();
      }
    }
    
    ICUError err;
    const char* source = streamInBufferPtr;
    UChar* target = ucBuffer;
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
       Since we buffer as much UChars as input bytes, a buffer overflow should
       _not_ occur. (A single byte expanding into a surrogate pait? Won't happen.)
     */
  }

} // namespace s1
