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

#include "CgSequenceCodeGenerator.h"

#include "CgTraits.h"

#include "codegen/cg/CgOptions.h"

namespace s1
{
  namespace codegen
  {
    CgGenerator::SequenceCodeGenerator::SequenceCodeGenerator (const intermediate::Sequence& seq,
							       sl::ImportedNameResolver* nameRes,
							       const intermediate::ProgramFunction::TransferMappings& transferIn,
							       const intermediate::ProgramFunction::TransferMappings& transferOut,
							       const std::vector<uc::String>& outParams,
                                                               const CgOptions& options)
     : sl::SequenceCodeGenerator (seq, nameRes, transferIn, transferOut, outParams, CgTraits::instance, options)
    {
    }

    const CgOptions& CgGenerator::SequenceCodeGenerator::GetCgOptions () const
    {
      return static_cast<const CgOptions&> (options);
    }

    std::unique_ptr<sl::SequenceCodeGenerator>
      CgGenerator::SequenceCodeGenerator::CreateForBlock (const intermediate::Sequence& seq,
                                                          sl::ImportedNameResolver* nameRes) const
    {
      static intermediate::ProgramFunction::TransferMappings emptyMappings;
      std::unique_ptr<SequenceCodeGenerator> p;
      p.reset (new SequenceCodeGenerator (seq, nameRes,
                                          emptyMappings, emptyMappings,
                                          outParams,
                                          GetCgOptions()));
      return std::move (p);
    }

  } // namespace codegen
} // namespace s1
