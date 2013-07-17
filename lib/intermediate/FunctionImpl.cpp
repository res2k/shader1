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
          funcInfo->params.push_back (newParam);
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
          // Synthesize an expression to assign the global with the default value
          ExpressionPtr nameExpr (handler->CreateVariableExpression (global));
          // Synthesize function parameter
          Scope::FunctionFormalParameter newParam;
          newParam.paramType = Scope::ptAutoGlobal;
          newParam.dir = Scope::dirOut;
          newParam.type = global->valueType;
          newParam.identifier = exported->first;
          newParam.defaultValue = nameExpr;
          funcInfo->params.push_back (newParam);
        }
      }
    }
  } // namespace intermediate
} // namespace s1
