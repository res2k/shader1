/*
    Shader1
    Copyright (c) 2016 Frank Richter


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
#include "CloningHelper.h"

#include "intermediate/Program.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace optimize
  {
    intermediate::ProgramPtr CloningHelper::CloneProgramMeta (intermediate::ProgramPtr inputProg)
    {
      intermediate::ProgramPtr newProgram (boost::make_shared<intermediate::Program> ());
      for (const intermediate::Program::TransferValuePair& tvp : inputProg->GetTransferValues ())
      {
        newProgram->AddTransferValue (tvp.first, tvp.second);
      }
      for (auto outputParam : inputProg->GetOutputParameters ())
      {
        newProgram->SetOutputParameter (outputParam.first, outputParam.second);
      }
      newProgram->SetParameterArraySizes (inputProg->GetParameterArraySizes ());

      return newProgram;
    }
  } // namespace optimize
} // namespace s1
