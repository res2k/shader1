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
 * Diagnostics handler interface
 */
#ifndef DIAGNOSTICS_HANDLER_H__
#define DIAGNOSTICS_HANDLER_H__

#include "base/nullopt.h"
#include "lexer/Token.h"
#include "lexer/TokenLocation.h"
#include "lexer/TokenType.h"

#include <type_traits>

#include <boost/variant.hpp>

namespace s1
{
  namespace diagnostics
  {
    struct Handler
    {
      virtual ~Handler() {}

      typedef boost::variant<nullopt_t,
                             const lexer::Token&,
                             const lexer::TokenType&> ErrorInfoType;

      /// Lexer error
      template<typename T>
      typename std::enable_if<std::is_enum<T>::value>::type LexerError (T code,
                                                                        const lexer::TokenLocation& location)
      {
        LexerErrorImpl (static_cast<unsigned int> (code), location);
      }
      /// Parse error
      template<typename T>
      typename std::enable_if<std::is_enum<T>::value>::type ParseError (T code,
                                                                        ErrorInfoType info1 = nullopt,
                                                                        ErrorInfoType info2 = nullopt)
      {
        ParseErrorImpl (static_cast<unsigned int> (code), info1, info2);
      }
      /// Semantic errors: Usually detected during intermediate generation
      template<typename T>
      typename std::enable_if<std::is_enum<T>::value>::type SemanticError (T code)
      {
        SemanticErrorImpl (static_cast<unsigned int> (code));
      }
    protected:
      virtual void LexerErrorImpl (unsigned int code, const lexer::TokenLocation& location) = 0;
      virtual void ParseErrorImpl (unsigned int code,
                                   ErrorInfoType info1,
                                   ErrorInfoType info2) = 0;
      virtual void SemanticErrorImpl (unsigned int code) = 0;
    };
  } // namespace diagnostics
} // namespace s1

#endif // DIAGNOSTICS_HANDLER_H__
