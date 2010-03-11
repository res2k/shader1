#ifndef __BASE_UNICODESTREAMENDOFINPUTEXCEPTION_H__
#define __BASE_UNICODESTREAMENDOFINPUTEXCEPTION_H__

#include "UnicodeStreamException.h"

namespace s1
{
  /**
   * Exception thrown when trying to read from a UnicodeStream beyond it's
   * end
   */
  class UnicodeStreamEndOfInputException : public UnicodeStreamException
  {
  public:
    /**
     * Construct a new exception object.
     */
    UnicodeStreamEndOfInputException () : UnicodeStreamException (U_ZERO_ERROR) {}
    
    const char* what () const throw() { return "end of input"; }
  };
  
} // namespace s1

#endif // __BASE_UNICODESTREAMENDOFINPUTEXCEPTION_H__
