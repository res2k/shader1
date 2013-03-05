#ifndef __BASE_UNICODESTREAMEXCEPTION_H__
#define __BASE_UNICODESTREAMEXCEPTION_H__

#include <unicode/utypes.h>

namespace s1
{
  /**
   * Base class for for UnicodeStream exceptions
   */
  class UnicodeStreamException : public std::exception
  {
    UErrorCode uerr;
  public:
    /**
     * Construct a new exception object.
     * \param uerr The error code returned by ICU.
     */
    UnicodeStreamException (UErrorCode uerr) : uerr (uerr) {}
    
    const char* what () const throw() { return u_errorName (uerr); }
  };
  
} // namespace s1

#endif // __BASE_UNICODESTREAMEXCEPTION_H__
