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

#include "base/common.h"
#include "intermediate/Program.h"

#include "splitter/Frequency.h"

namespace s1
{
  namespace intermediate
  {
    void Program::AddFunction (const ProgramFunctionPtr& func)
    {
      functions.push_back (func);
    }
      
    size_t Program::GetNumFunctions () const
    {
      return functions.size();
    }
    
    ProgramFunctionPtr Program::GetFunction (size_t n) const
    {
      return functions[n];
    }
    
    void Program::AddTransferValue (const IntermediateGeneratorSemanticsHandler::TypePtr& type,
				    const uc::String& name)
    {
      transferValues.emplace_back (type, name);
    }
    
    void Program::SetOutputParameter (const uc::String& name, ParameterTarget target)
    {
      outputParams[name] = target;
    }
    
    const Program::OutputParameters& Program::GetOutputParameters () const
    {
      return outputParams;
    }

    void Program::SetParameterArraySize (const uc::String& name, size_t size)
    {
      paramArraySizes[name] = size;
    }
    
    const Program::ParameterArraySizes& Program::GetParameterArraySizes () const
    {
      return paramArraySizes;
    }
    
    int Program::GetTargetFrequency (ParameterTarget target)
    {
      switch (target)
      {
      case Position:	return splitter::freqVertex;
      case Color:	return splitter::freqFragment;
      }
      assert (false);
      return -1;
    }
    
  } // namespace intermediate
} // namespace s1

