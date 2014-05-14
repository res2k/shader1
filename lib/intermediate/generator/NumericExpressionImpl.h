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

#ifndef __INTERMEDIATE_NUMERICEXPRESSIONIMPL_H__
#define __INTERMEDIATE_NUMERICEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::NumericExpressionImpl : public ExpressionImpl
    {
      UnicodeString valueStr;
      SemanticsHandler::BaseType numberType;
    public:
      NumericExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			     const UnicodeString& valueStr);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
				 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_NUMERICEXPRESSIONIMPL_H__
