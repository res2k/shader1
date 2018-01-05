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
 * Visitor interface for AST type
 */
#ifndef S1_PARSER_AST_VISITORTYPE_H_
#define S1_PARSER_AST_VISITORTYPE_H_

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      struct TypeArray;
      struct TypeIdentifier;
      struct TypeWellKnown;

      /// Visitor interface for AST type
      struct VisitorType
      {
        virtual ~VisitorType() {}

        virtual void operator() (const TypeArray& type) = 0;
        virtual void operator() (const TypeIdentifier& type) = 0;
        virtual void operator() (const TypeWellKnown& type) = 0;
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_VISITORTYPE_H_
