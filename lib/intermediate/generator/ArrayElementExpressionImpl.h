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

#ifndef __INTERMEDIATE_ARRAYELEMENTEXPRESSIONIMPL_H__
#define __INTERMEDIATE_ARRAYELEMENTEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl : public ExpressionImpl
    {
      ExpressionPtr arrayExpr;
      ExpressionPtr indexExpr;
    public:
      ArrayElementExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
                                  ExpressionContext&& context,
                                  const ExpressionPtr& arrayExpr,
                                  const ExpressionPtr& indexExpr);
      
      semantics::TypePtr GetValueType ();
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
                                 bool asLvalue = false);
      void AddToSequencePostAction (BlockImpl& block, const RegisterPtr& target,
                                    bool wasLvalue);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_ARRAYELEMENTEXPRESSIONIMPL_H__s
