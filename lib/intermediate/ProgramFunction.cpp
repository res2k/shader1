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

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/ProgramFunction.h"

#include "BlockImpl.h"

namespace s1
{
  namespace intermediate
  {
    ProgramFunction::ProgramFunction (const uc::String& originalIdentifier,
				      const uc::String& identifier,
				      const parser::SemanticsHandler::Scope::FunctionFormalParameters& params,
				      const SequencePtr& body,
				      bool isEntryFunction)
     : originalIdentifier (originalIdentifier) ,identifier (identifier),
       params (params), body (body),
       isEntryFunction (isEntryFunction)
    {
    }

    void ProgramFunction::SetTransferMapping (const uc::String& transferVal,
					      const RegisterPtr& programReg)
    {
      transferMappings.push_back (std::make_pair (transferVal, programReg));
    }
  } // namespace intermediate
} // namespace s1

