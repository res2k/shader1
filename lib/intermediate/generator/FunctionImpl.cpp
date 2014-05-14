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

#include "FunctionImpl.h"

#include "BlockImpl.h"
#include "NameImpl.h"
#include "ScopeImpl.h"

#include <boost/foreach.hpp>

namespace s1
{
  namespace intermediate
  {
    void IntermediateGeneratorSemanticsHandler::FunctionImpl::Finish()
    {
      boost::shared_ptr<BlockImpl> funcBlockImpl (boost::static_pointer_cast<BlockImpl> (block));
      funcBlockImpl->FinishBlock ();

      typedef boost::unordered_map<NameImplPtr, Scope::FunctionFormalParameter> AutoGlobalParamsMap;
      AutoGlobalParamsMap autoGlobalParams;

      // Add artificial arguments for global variables
      Sequence::RegisterImpMappings imports (funcBlockImpl->GetSequence()->GetImports());
      for (Sequence::RegisterImpMappings::const_iterator imported (imports.begin());
          imported != imports.end();
          ++imported)
      {
        NameImplPtr global (handler->globalScope->ResolveIdentifierInternal (imported->first));
        if (global)
        {
          // Synthesize an expression to assign the global with the default value
          ExpressionPtr nameExpr (handler->CreateVariableExpression (global));
          // Synthesize function parameter
          Scope::FunctionFormalParameter newParam;
          newParam.paramType = Scope::ptAutoGlobal;
          newParam.dir = Scope::dirIn;
          newParam.type = global->valueType;
          newParam.identifier = imported->first;
          newParam.defaultValue = nameExpr;
          autoGlobalParams[global] = newParam;
        }
      }

      intermediate::Sequence::RegisterExpMappings exports (funcBlockImpl->GetSequence()->GetExports());
      for (intermediate::Sequence::RegisterExpMappings::const_iterator exported (exports.begin());
            exported != exports.end();
            ++exported)
      {
        NameImplPtr global (handler->globalScope->ResolveIdentifierInternal (exported->first));
        if (global)
        {
          AutoGlobalParamsMap::iterator map_it (autoGlobalParams.find (global));
          if (map_it != autoGlobalParams.end())
          {
            // The param is both an input and output param, augment the existing param
            map_it->second.dir = Scope::dirInOut;
          }
          else
          {
            // Synthesize an expression to assign the global with the default value
            ExpressionPtr nameExpr (handler->CreateVariableExpression (global));
            // Synthesize function parameter
            Scope::FunctionFormalParameter newParam;
            newParam.paramType = Scope::ptAutoGlobal;
            newParam.dir = Scope::dirOut;
            newParam.type = global->valueType;
            newParam.identifier = exported->first;
            newParam.defaultValue = nameExpr;
            autoGlobalParams[global] = newParam;
          }
        }
      }

      BOOST_FOREACH(const AutoGlobalParamsMap::value_type& newParam, autoGlobalParams)
      {
        funcInfo->params.push_back (newParam.second);
      }
    }
  } // namespace intermediate
} // namespace s1
