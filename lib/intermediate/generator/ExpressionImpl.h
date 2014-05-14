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

#ifndef __INTERMEDIATE_EXPRESSIONIMPL_H__
#define __INTERMEDIATE_EXPRESSIONIMPL_H__

#include "CommandImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ExpressionImpl : public CommandImpl
    {
    public:
      ExpressionImpl (IntermediateGeneratorSemanticsHandler* handler);
      
      virtual NameImplPtr GetExpressionName() { return NameImplPtr (); }
      
      void AddToSequence (BlockImpl& block);
      
      virtual NameImplSet QueryWrittenNames (bool asLvalue);
      
      virtual boost::shared_ptr<TypeImpl> GetValueType() = 0;
      virtual RegisterPtr AddToSequence (BlockImpl& block,
					 RegisterClassification targetClassify,
					 bool asLvalue = false) = 0;
      virtual void AddToSequencePostAction (BlockImpl& block,
					    const RegisterPtr& target,
					    bool wasLvalue) {}
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_EXPRESSIONIMPL_H__
