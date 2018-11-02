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

#ifndef __BUILTIN_H__
#define __BUILTIN_H__

#include "intermediate/BuiltinFunction.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

#include "ScopeImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::Builtin
      : public boost::intrusive_ref_counter<Builtin>
    {
    public:
      typedef std::function<SequenceOpPtr (RegisterPtr dest, const std::vector<RegisterPtr>& inParams)>
        SeqOpFactoryFunc;

      Builtin (SeqOpFactoryFunc factoryFunc,
               const semantics::TypePtr& returnType,
               const uc::String& identifier,
               const semantics::FunctionFormalParameters& formalParameters)
       : factoryFunc (factoryFunc), returnType (returnType), identifier (identifier),
         formalParameters (formalParameters)
      {}

      SeqOpFactoryFunc GetSeqOpFactory () const { return factoryFunc; }
      const semantics::TypePtr& GetReturnType () const { return returnType; }
      const uc::String& GetIdentifier() const { return identifier; }
      const semantics::FunctionFormalParameters& GetFormalParameters() const
      { return formalParameters; }
    protected:
      SeqOpFactoryFunc factoryFunc;
      semantics::TypePtr returnType;
      uc::String identifier;
      semantics::FunctionFormalParameters formalParameters;
    };
  } // namespace intermediate
} // namespace s1

#endif // __BUILTIN_H__
