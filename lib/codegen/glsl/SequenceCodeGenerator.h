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

#ifndef __CODEGEN_GLSL_SEQUENCECODEGENERATOR_H__
#define __CODEGEN_GLSL_SEQUENCECODEGENERATOR_H__

#include "codegen/sl/SequenceCodeGenerator.h"

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      class Options;

      class SequenceCodeGenerator : public sl::SequenceCodeGenerator
      {
      protected:
        const Options& GetOptions () const;

        std::unique_ptr<sl::SequenceCodeGenerator> CreateForBlock (const intermediate::Sequence& seq,
                                                                   sl::ImportedNameResolver* nameRes) const override;
      public:
        SequenceCodeGenerator (const intermediate::Sequence& seq,
                               sl::ImportedNameResolver* nameRes,
                               const intermediate::ProgramFunction::TransferMappings& transferIn,
                               const intermediate::ProgramFunction::TransferMappings& transferOut,
                               const std::vector<uc::String>& outParams,
                               const sl::Traits& traits,
                               const Options& options);
      };
    } // namespace glsl
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_GLSL_SEQUENCECODEGENERATOR_H__
