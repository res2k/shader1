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

#include "FunctionCodeGenerator.h"

#include "CgTraits.h"
#include "SequenceCodeGenerator.h"

#include "codegen/cg/CgOptions.h"

#if 0
#include "intermediate/ProgramFunction.h"
#include "splitter/Frequency.h"
#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <string>
#include <sstream>
#endif

namespace s1
{
  namespace codegen
  {
    CgGenerator::FunctionCodeGenerator::FunctionCodeGenerator (const CgOptions& options)
      : sl::FunctionCodeGenerator (CgTraits::instance, options)
    {
    }

    const CgOptions& CgGenerator::FunctionCodeGenerator::GetCgOptions () const
    {
      return static_cast<const CgOptions&> (options);
    }

    std::unique_ptr<sl::SequenceCodeGenerator>
      CgGenerator::FunctionCodeGenerator::CreateSeqGen (const intermediate::Sequence& seq,
                                                        sl::ImportedNameResolver* nameRes,
                                                        const intermediate::ProgramFunction::TransferMappings& transferIn,
                                                        const intermediate::ProgramFunction::TransferMappings& transferOut,
                                                        const std::vector<std::string>& outParams) const
    {
      std::unique_ptr<SequenceCodeGenerator> p;
      p.reset (new SequenceCodeGenerator (seq, nameRes, transferIn, transferOut, outParams, GetCgOptions()));
      return std::move (p);
    }
  } // namespace codegen
} // namespace s1

