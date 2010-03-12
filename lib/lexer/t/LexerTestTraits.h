#ifndef __LEXERTESTTRAITS_H__
#define __LEXERTESTTRAITS_H__

#include <cxxtest/ValueTraits.h>

CXXTEST_ENUM_TRAITS (s1::Lexer::TokenType,
		     CXXTEST_ENUM_MEMBER (s1::Lexer::EndOfFile)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Invalid)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Identifier)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Numeric)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Semicolon)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::ParenL)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::ParenR)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::BracketL)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::BracketR)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::BraceL)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::BraceR)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Member)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Separator)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Equals)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::NotEquals)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Larger)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::LargerEqual)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Smaller)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::SmallerEqual)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Assign)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Plus)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Minus)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Mult)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Div)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::Mod)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::BitwiseInvert)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::LogicInvert)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::TernaryIf)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::TernaryElse)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::LogicAnd)
		     CXXTEST_ENUM_MEMBER (s1::Lexer::LogicOr)
);

namespace CxxTest 
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<UnicodeString> 
  {
    std::string utfStr;

  public:
    ValueTraits (const UnicodeString& s )
    {
      std::string tmp;
      StringByteSink<std::string> utfSink (&tmp);
      s.toUTF8 (utfSink);
      utfStr.append ("\"");
      utfStr.append (tmp);
      utfStr.append ("\"");
    }
    const char *asString() const { return utfStr.c_str(); }
  };
};

#endif // __LEXERTESTTRAITS_H__
