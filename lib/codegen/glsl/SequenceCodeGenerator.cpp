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

#include "SequenceCodeGenerator.h"

#include "Traits.h"

#include "codegen/glsl/Options.h"

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      SequenceCodeGenerator::SequenceCodeGenerator (const intermediate::Sequence& seq,
                                                    sl::ImportedNameResolver* nameRes,
                                                    const intermediate::ProgramFunction::TransferMappings& transferIn,
                                                    const intermediate::ProgramFunction::TransferMappings& transferOut,
                                                    const std::vector<std::string>& outParams,
                                                    const sl::Traits& traits,
                                                    const Options& options)
        : sl::SequenceCodeGenerator (seq, nameRes, transferIn, transferOut, outParams, traits, options)
      {
      }

      const Options& SequenceCodeGenerator::GetOptions () const
      {
        return static_cast<const Options&> (options);
      }

      std::unique_ptr<sl::SequenceCodeGenerator>
        SequenceCodeGenerator::CreateForBlock (const intermediate::Sequence& seq,
                                               sl::ImportedNameResolver* nameRes) const
      {
        static intermediate::ProgramFunction::TransferMappings emptyMappings;
        std::unique_ptr<SequenceCodeGenerator> p;
        p.reset (new SequenceCodeGenerator (seq, nameRes,
                                            emptyMappings, emptyMappings,
                                            outParams,
                                            traits, GetOptions ()));
        return std::move (p);
      }
    } // namespace glsl
  } // namespace codegen
} // namespace s1
