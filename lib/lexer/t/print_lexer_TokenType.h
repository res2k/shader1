/*
    Shader1
    Copyright (c) 2018 Frank Richter


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
 * Helper: Define operator<< for s1::lexer::TokenType
 */

#ifndef PRINT_LEXER_TOKENTYPE_H_
#define PRINT_LEXER_TOKENTYPE_H_

#include "lexer/TokenType.h"

#include <iostream>

namespace s1
{
  namespace lexer
  {
    static std::ostream& operator<< (std::ostream& stream, const TokenType& tt)
    {
      int tt_value = static_cast<int> (tt);
      #define PRINT_FLAG(X)   if ((tt_value & X) == X) { stream << #X << " | "; tt_value &= ~X; }
      PRINT_FLAG(VecFlag)
      PRINT_FLAG(MatFlag)
      #undef PRINT_FLAG

      switch (tt_value)
      {
      #define PRINT_TOKEN(X)  case X: stream << #X; return stream;

      PRINT_TOKEN (Invalid)
      PRINT_TOKEN (EndOfFile)
      PRINT_TOKEN (Unknown)
      PRINT_TOKEN (Identifier)
      PRINT_TOKEN (Numeric)
      PRINT_TOKEN (Semicolon)
      PRINT_TOKEN (ParenL)
      PRINT_TOKEN (ParenR)
      PRINT_TOKEN (BracketL)
      PRINT_TOKEN (BracketR)
      PRINT_TOKEN (BraceL)
      PRINT_TOKEN (BraceR)
      PRINT_TOKEN (Member)
      PRINT_TOKEN (Separator)
      PRINT_TOKEN (Equals)
      PRINT_TOKEN (NotEquals)
      PRINT_TOKEN (Larger)
      PRINT_TOKEN (LargerEqual)
      PRINT_TOKEN (Smaller)
      PRINT_TOKEN (SmallerEqual)
      PRINT_TOKEN (Assign)
      PRINT_TOKEN (Plus)
      PRINT_TOKEN (Minus)
      PRINT_TOKEN (Mult)
      PRINT_TOKEN (Div)
      PRINT_TOKEN (Mod)
      PRINT_TOKEN (BitwiseInvert)
      PRINT_TOKEN (LogicInvert)
      PRINT_TOKEN (TernaryIf)
      PRINT_TOKEN (TernaryElse)
      PRINT_TOKEN (LogicAnd)
      PRINT_TOKEN (LogicOr)
      PRINT_TOKEN (kwReturn)
      PRINT_TOKEN (kwTrue)
      PRINT_TOKEN (kwFalse)
      PRINT_TOKEN (kwSampler1D)
      PRINT_TOKEN (kwSampler2D)
      PRINT_TOKEN (kwSampler3D)
      PRINT_TOKEN (kwSamplerCUBE)
      PRINT_TOKEN (kwTypedef)
      PRINT_TOKEN (kwVoid)
      PRINT_TOKEN (kwIn)
      PRINT_TOKEN (kwOut)
      PRINT_TOKEN (kwConst)
      PRINT_TOKEN (kwUniform)
      PRINT_TOKEN (kwAttribute)
      PRINT_TOKEN (kwIf)
      PRINT_TOKEN (kwElse)
      PRINT_TOKEN (kwWhile)
      PRINT_TOKEN (kwFor)
      PRINT_TOKEN (kwBool)
      PRINT_TOKEN (kwUnsigned)
      PRINT_TOKEN (kwInt)
      PRINT_TOKEN (kwFloat)

      #undef PRINT_TOKEN

      // Flags & combinations, handled above, but here to silence warnings
      case VecFlag:
      case MatFlag:
      case TypeFlagMask:
      case kwBoolVec:
      case kwUnsignedVec:
      case kwIntVec:
      case kwFloatVec:
      case kwBoolMat:
      case kwUnsignedMat:
      case kwIntMat:
      case kwFloatMat:
        break;
      }
      stream << static_cast<int> (tt);
      return stream;
    }
  } // namespace lexer
} // namespace s1

#endif // PRINT_LEXER_TOKENTYPE_H_
