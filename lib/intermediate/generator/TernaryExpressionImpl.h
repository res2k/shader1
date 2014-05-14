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

#ifndef __INTERMEDIATE_TERNARYEXPRESSIONIMPL_H__
#define __INTERMEDIATE_TERNARYEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl : public ExpressionImpl
    {
      boost::shared_ptr<ExpressionImpl> condition;
      boost::shared_ptr<ExpressionImpl> ifExpr;
      boost::shared_ptr<ExpressionImpl> elseExpr;
    public:
      TernaryExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			     const boost::shared_ptr<ExpressionImpl>& condition,
			     const boost::shared_ptr<ExpressionImpl>& ifExpr,
			     const boost::shared_ptr<ExpressionImpl>& elseExpr);
      
      NameImplSet QueryWrittenNames (bool asLvalue);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
				 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_TERNARYEXPRESSIONIMPL_H__