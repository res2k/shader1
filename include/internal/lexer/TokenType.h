/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**
 * Lexer token types
 */
#ifndef __LEXER_TOKENTYPE_H__
#define __LEXER_TOKENTYPE_H__

namespace s1
{
  namespace lexer
  {
    enum TokenType
    {
      /// "Vector" type flag
      VecFlag = 0x10000,
      /// "Matrix" type flag
      MatFlag = 0x20000,
      /// Bit mask for type flags
      TypeFlagMask = 0x30000,

      /// Invalid token (resulting from e.g. a stray character)
      Invalid = -2,
      /// End of input was reached
      EndOfFile = -1,
      
      /// Unrecognized token
      Unknown = 0,
      /// Identifier
      Identifier,
      /// Numeric (hex or float number)
      Numeric,
      
      /**\name Operators, Symbols
       * @{ */
      /// ';'
      Semicolon,
      
      /// '('
      ParenL,
      /// ')'
      ParenR,
      /// '['
      BracketL,
      /// ']'
      BracketR,
      /// '{'
      BraceL,
      /// '}'
      BraceR,
      
      /// '.'
      Member,
      /// ','
      Separator,
      
      /// '=='
      Equals,
      /// '!='
      NotEquals,
      /// '>'
      Larger,
      /// '>='
      LargerEqual,
      /// '<'
      Smaller,
      /// '<='
      SmallerEqual,
      
      /// '='
      Assign,
      
      /// '+'
      Plus,
      /// '-'
      Minus,
      /// '*'
      Mult,
      /// '/'
      Div,
      /// '%'
      Mod,
      
      /// '~'
      BitwiseInvert,
      /// '!'
      LogicInvert,
      
      /// '?'
      TernaryIf,
      /// ':'
      TernaryElse,
      
      /// '||'
      LogicOr,
      /// '&&'
      LogicAnd,
      /** @} */
      
      /**\name Keywords
       * @{ */
      kwReturn,
      
      kwTrue,
      kwFalse,

      kwSampler1D,
      kwSampler2D,
      kwSampler3D,
      kwSamplerCUBE,
      
      kwTypedef,
      kwVoid,
      kwIn,
      kwOut,
      kwConst,
      kwUniform,
      kwAttribute,
      
      kwIf,
      kwElse,
      
      kwWhile,
      kwFor,

      kwBool,
      kwUnsigned,
      kwInt,
      kwFloat,
      kwBoolVec = kwBool | VecFlag,
      kwUnsignedVec = kwUnsigned | VecFlag,
      kwIntVec = kwInt | VecFlag,
      kwFloatVec = kwFloat | VecFlag,
      kwBoolMat = kwBool | MatFlag,
      kwUnsignedMat = kwUnsigned | MatFlag,
      kwIntMat = kwInt | MatFlag,
      kwFloatMat =kwFloat  | MatFlag
      /** @} */
    };
  } // namespace lexer
} // namespace s1

#endif // __LEXER_TOKENTYPE_H__
