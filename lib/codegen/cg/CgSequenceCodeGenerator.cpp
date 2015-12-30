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
                                                               const CgOptions& options,
                                                               const uc::String& v2fName)
     : sl::SequenceCodeGenerator (seq, nameRes, transferIn, transferOut, outParams, CgTraits::instance, options),
       v2fName (v2fName)
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
                                          GetCgOptions(), v2fName));
      return std::move (p);
    }

    static format::StaticFormatter FormatTransferID ("{0}.{1}");

    std::vector<CgGenerator::SequenceCodeGenerator::InputTransferPair>
      CgGenerator::SequenceCodeGenerator::GenerateTransfersIn () const
    {
      std::vector<CgGenerator::SequenceCodeGenerator::InputTransferPair> result;
      for (const auto& transfer : transferIn)
      {
        result.emplace_back (transfer.second,
                             FormatTransferID.to<uc::String> (v2fName, traits.ConvertIdentifier (transfer.first)));
      }
      return result;
    }

    std::vector<CgGenerator::SequenceCodeGenerator::OutputTransferPair>
      CgGenerator::SequenceCodeGenerator::GenerateTransfersOut () const
    {
      std::vector<CgGenerator::SequenceCodeGenerator::OutputTransferPair> result;
      for (const auto& transfer : transferOut)
      {
        result.emplace_back (FormatTransferID.to<uc::String> (v2fName, traits.ConvertIdentifier (transfer.first)),
                             transfer.second);
      }
      return result;
    }
  } // namespace codegen
} // namespace s1
