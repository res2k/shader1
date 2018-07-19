/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __INTERMEDIATE_BINARYEXPRESSIONIMPL_H__
#define __INTERMEDIATE_BINARYEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

#include <boost/optional.hpp>

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::BinaryExpressionImpl : public ExpressionImpl
    {
    protected:
      boost::intrusive_ptr<ExpressionImpl> operand1;
      boost::intrusive_ptr<ExpressionImpl> operand2;

      struct OperandReg
      {
        RegisterPtr orig, reg;

        OperandReg () {}
        OperandReg (const RegisterPtr& r) : orig (r), reg (r) {}
      };
      typedef std::tuple<OperandReg, OperandReg> RegisterPtrTuple;
      /**
       * Add operand expressions to block and return registers for use as sources,
       * casted to requested type, if necessary.
       */
      boost::optional<RegisterPtrTuple> GetSourceRegisters (BlockImpl& block,
                                                            semantics::Type* asType);
    public:
      BinaryExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
                            ExpressionContext&& context,
                            ExpressionImpl* operand1,
                            ExpressionImpl* operand2);
      
      NameSet QueryWrittenNames (bool asLvalue);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_BINARYEXPRESSIONIMPL_H__
