#ifndef __PARSER_ERRORCODE_H__
#define __PARSER_ERRORCODE_H__

namespace s1
{
  namespace parser
  {
    enum ErrorCode
    {
      UnexpectedToken,
      IdentifierAlreadyDeclared,
      IdentifierUndeclared,
      DeclarationNotAllowedInScope
    };
  } // namespace parser
} // namespace s1

#endif // __PARSER_ERRORCODE_H__
