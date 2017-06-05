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

#ifndef __LEXERTESTTRAITS_H__
#define __LEXERTESTTRAITS_H__

#include <cxxtest/ValueTraits.h>

CXXTEST_ENUM_TRAITS (s1::lexer::TokenType,
                     CXXTEST_ENUM_MEMBER (s1::lexer::Invalid)
                     CXXTEST_ENUM_MEMBER (s1::lexer::EndOfFile)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Unknown)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Identifier)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Numeric)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Semicolon)
                     CXXTEST_ENUM_MEMBER (s1::lexer::ParenL)
                     CXXTEST_ENUM_MEMBER (s1::lexer::ParenR)
                     CXXTEST_ENUM_MEMBER (s1::lexer::BracketL)
                     CXXTEST_ENUM_MEMBER (s1::lexer::BracketR)
                     CXXTEST_ENUM_MEMBER (s1::lexer::BraceL)
                     CXXTEST_ENUM_MEMBER (s1::lexer::BraceR)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Member)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Separator)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Equals)
                     CXXTEST_ENUM_MEMBER (s1::lexer::NotEquals)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Larger)
                     CXXTEST_ENUM_MEMBER (s1::lexer::LargerEqual)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Smaller)
                     CXXTEST_ENUM_MEMBER (s1::lexer::SmallerEqual)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Assign)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Plus)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Minus)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Mult)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Div)
                     CXXTEST_ENUM_MEMBER (s1::lexer::Mod)
                     CXXTEST_ENUM_MEMBER (s1::lexer::BitwiseInvert)
                     CXXTEST_ENUM_MEMBER (s1::lexer::LogicInvert)
                     CXXTEST_ENUM_MEMBER (s1::lexer::TernaryIf)
                     CXXTEST_ENUM_MEMBER (s1::lexer::TernaryElse)
                     CXXTEST_ENUM_MEMBER (s1::lexer::LogicAnd)
                     CXXTEST_ENUM_MEMBER (s1::lexer::LogicOr)
);

namespace CxxTest 
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<s1::uc::String> 
  {
    std::string utfStr;

  public:
    ValueTraits (const s1::uc::String& s )
    {
      std::string tmp;
      s.toUTF8String (tmp);
      utfStr.append ("\"");
      utfStr.append (tmp);
      utfStr.append ("\"");
    }
    const char *asString() const { return utfStr.c_str(); }
  };
};

#endif // __LEXERTESTTRAITS_H__
