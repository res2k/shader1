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
#include "intermediate/ProgramFunction.h"

#include "splitter/Frequency.h"

#include <boost/utility/in_place_factory.hpp>

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
    
    ProgramFunctionPtr Program::GetEntryFunction () const
    {
      auto found_func = std::find_if (functions.begin (), functions.end (),
                                      [&](const ProgramFunctionPtr& func)
                                      {
                                        return func->IsEntryFunction();
                                      });
      if (found_func != functions.end ()) return *found_func;
      return ProgramFunctionPtr ();
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

    void Program::SetParameterArraySizes (const Program::ParameterArraySizes& sizes)
    {
      paramArraySizes = sizes;
    }
    
    const Program::ParameterArraySizes& Program::GetParameterArraySizes () const
    {
      return paramArraySizes;
    }

    const FunctionGraph& Program::GetFunctionGraph ()
    {
      if (!functionGraph)
        functionGraph = boost::in_place<> (*this);
      return *functionGraph;
    }
    
    int Program::GetTargetFrequency (ParameterTarget target)
    {
      switch (target)
      {
      case Position:	return splitter::freqVertex;
      case Color:	return splitter::freqFragment;
      }
      S1_ASSERT_NOT_REACHED (-1);
    }
    
  } // namespace intermediate
} // namespace s1

