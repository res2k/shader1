#ifndef __INTERMEDIATE_EXCEPTION_H__
#define __INTERMEDIATE_EXCEPTION_H__

#include "ErrorCode.h"
#include "lexer/Lexer.h"

#include <exception>

namespace s1
{
  namespace intermediate
  {
    class Exception : public std::exception
    {
      ErrorCode code;
    public:
      ~Exception() throw () {}
      
      Exception (ErrorCode code) : code (code) {}
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_EXCEPTION_H__
