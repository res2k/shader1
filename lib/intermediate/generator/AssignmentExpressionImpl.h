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

#ifndef __INTERMEDIATE_ASSIGNMENTEXPRESSIONIMPL_H__
#define __INTERMEDIATE_ASSIGNMENTEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl : public ExpressionImpl
    {
      boost::intrusive_ptr<ExpressionImpl> target;
      boost::intrusive_ptr<ExpressionImpl> value;
    public:
      AssignmentExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
                                ExpressionContext&& context,
                                ExpressionImpl* target,
                                ExpressionImpl* value);
      
      NameSet QueryWrittenNames (bool asLvalue);
      
      semantics::TypePtr GetValueType();
      
      void AddToSequence (BlockImpl& block);
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
                                 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_ASSIGNMENTEXPRESSIONIMPL_H__
