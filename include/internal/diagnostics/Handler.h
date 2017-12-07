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

#include "lexer/Token.h"
#include "lexer/TokenType.h"

#include <type_traits>

namespace s1
{
  namespace diagnostics
  {
    struct Handler
    {
      virtual ~Handler() {}

      /// Parse error
      template<typename T>
      typename std::enable_if<std::is_enum<T>::value>::type ParseError (T code,
                                                                        const lexer::Token& encounteredToken,
                                                                        lexer::TokenType expectedToken)
      {
        ParseErrorImpl (static_cast<unsigned int> (code), encounteredToken, expectedToken);
      }
      /// Semantic errors: Usually detected during intermediate generation
      template<typename T>
      typename std::enable_if<std::is_enum<T>::value>::type SemanticError (T code)
      {
        SemanticErrorImpl (static_cast<unsigned int> (code));
      }
    protected:
      virtual void ParseErrorImpl (unsigned int code,
                                   const lexer::Token& encounteredToken,
                                   lexer::TokenType expectedToken) = 0;
      virtual void SemanticErrorImpl (unsigned int code) = 0;
    };
  } // namespace diagnostics
} // namespace s1

#endif // DIAGNOSTICS_HANDLER_H__
