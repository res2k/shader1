#ifndef __BASE_UNICODESTREAMINVALIDCHARACTEREXCEPTION_H__
#define __BASE_UNICODESTREAMINVALIDCHARACTEREXCEPTION_H__

#include "UnicodeStreamException.h"

namespace s1
{
  /**
   * Exception thrown when UnicodeStream encounters an invalid character
   */
  class UnicodeStreamInvalidCharacterException : public UnicodeStreamException
  {
  public:
    /**
     * Construct a new exception object.
     * \param uerr The error code returned by ICU.
     */
    UnicodeStreamInvalidCharacterException (UErrorCode uerr)
     : UnicodeStreamException (uerr) {}
  };
  
} // namespace s1

#endif // __BASE_UNICODESTREAMINVALIDCHARACTEREXCEPTION_H__
