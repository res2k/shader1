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

#ifndef __INTERMEDIATE_PROGRAM_H__
#define __INTERMEDIATE_PROGRAM_H__

#include "forwarddecl.h"
#include "FunctionGraph.h"
#include "IntermediateGeneratorSemanticsHandler.h"

#include <boost/optional.hpp>
#include <boost/unordered_map.hpp>
#include <vector>

namespace s1
{
  namespace intermediate
  {
    class Program
    {
    protected:
      friend class IntermediateGeneratorSemanticsHandler;
      
      std::vector<ProgramFunctionPtr> functions;
    public:
      void AddFunction (const ProgramFunctionPtr& func);
      
      size_t GetNumFunctions () const;
      ProgramFunctionPtr GetFunction (size_t n) const;
      ProgramFunctionPtr GetEntryFunction () const;
      
      void AddTransferValue (const IntermediateGeneratorSemanticsHandler::TypePtr& type,
                             const uc::String& name);
      typedef std::pair<IntermediateGeneratorSemanticsHandler::TypePtr, uc::String> TransferValuePair;
      typedef std::vector<TransferValuePair> TransferValues;
      const TransferValues& GetTransferValues () const { return transferValues; }
      
      enum ParameterTarget
      {
        /// Vertex output position
        Position,
        /// Fragment output color
        Color
      };
      typedef std::unordered_map<uc::String, ParameterTarget> OutputParameters;
      void SetOutputParameter (const uc::String& name, ParameterTarget target);
      const OutputParameters& GetOutputParameters () const;
      
      typedef std::unordered_map<uc::String, size_t> ParameterArraySizes;
      void SetParameterArraySizes (const ParameterArraySizes& sizes);
      const ParameterArraySizes& GetParameterArraySizes () const;

      /// Return function graph for program
      const FunctionGraph& GetFunctionGraph ();
      
      // @@@ Actualls, should perhaps better be in splitter ...
      static int GetTargetFrequency (ParameterTarget target);
    private:
      TransferValues transferValues;
      OutputParameters outputParams;
      ParameterArraySizes paramArraySizes;
      boost::optional<FunctionGraph> functionGraph;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAM_H__