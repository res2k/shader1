/*
    Shader1
    Copyright (c) 2010-2015 Frank Richter


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
#include "PrependGlobalsInit.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    PrependGlobalsInit::PrependGlobalsInit (const SequenceBuilderPtr& newSequenceBuilder,
                                            const SequencePtr& oldSequence,
                                            const SequencePtr& initSeq)
      : CloningSequenceVisitor (newSequenceBuilder)
    {
      Sequence::RegisterImpMappings newImports (oldSequence->GetImports());
      if (initSeq)
      {
        Sequence::IdentifierToRegMap initExports;

        for(const auto& exp : initSeq->GetExports())
        {
          RegisterPtr reg;
          Sequence::RegisterImpMappings::iterator impIt (
              std::find_if (newImports.begin(), newImports.end(),
                            boost::bind (&Sequence::IdentRegPair::first, _1) == exp.first));
          if (impIt != newImports.end())
          {
            reg = impIt->second;
            newImports.erase (impIt);
          }
          else
          {
            // Make sure the global is exported (even though not used in the old sequence)
            reg = newSequenceBuilder->AllocateRegister (exp.second->GetOriginalType (), exp.first);
          }
          
          initExports[exp.first] = reg;
        }
        SequenceOpPtr newOp (new SequenceOpBlock (initSeq,
                                                  Sequence::IdentifierToRegMap (),
                                                  initExports));
        AddOpToSequence (newOp);
      }
      newSequenceBuilder->AddImports (newImports);
      newSequenceBuilder->AddExports (oldSequence->GetExports());
    }
    
    CloningSequenceVisitor* PrependGlobalsInit::Clone (const SequenceBuilderPtr& newSequenceBuilder,
                                                       const SequencePtr& oldSequence,
                                                       const RegisterMap& regMap)
    {
      return new PrependGlobalsInit (newSequenceBuilder, oldSequence, SequencePtr ());
    }
      
    CloningSequenceVisitor* PrependGlobalsInit::Clone (const SequenceBuilderPtr& newSequenceBuilder,
                                                       const RegisterMap& regMap)
    {
      S1_ASSERT_NOT_REACHED (nullptr);
    }
  } // namespace intermediate
} // namespace s1
