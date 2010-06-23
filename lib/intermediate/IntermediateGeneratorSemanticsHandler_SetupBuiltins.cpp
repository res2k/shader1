#include "base/common.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

#include "Builtin.h"
#include "ScopeImpl.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::Scope::FunctionFormalParameters
    IntermediateGeneratorSemanticsHandler::MakeFormalParameters1 (const TypePtr& type)
    {
      IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionFormalParameters params;

      IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionFormalParameter fp;
      fp.type = type;
      fp.identifier = UnicodeString ("a");
      fp.dir = IntermediateGeneratorSemanticsHandler::ScopeImpl::dirIn;
      params.push_back (fp);

      return params;
    }
    
    IntermediateGeneratorSemanticsHandler::Scope::FunctionFormalParameters
    IntermediateGeneratorSemanticsHandler::MakeFormalParameters2 (const TypePtr& type)
    {
      return MakeFormalParameters2 (type, type);
    }
    
    IntermediateGeneratorSemanticsHandler::Scope::FunctionFormalParameters
    IntermediateGeneratorSemanticsHandler::MakeFormalParameters2 (const TypePtr& typeA, const TypePtr& typeB)
    {
      IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionFormalParameters params;

      IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionFormalParameter fp;
      fp.type = typeA;
      fp.identifier = UnicodeString ("a");
      fp.dir = IntermediateGeneratorSemanticsHandler::ScopeImpl::dirIn;
      params.push_back (fp);

      fp.type = typeB;
      fp.identifier = UnicodeString ("b");
      params.push_back (fp);
      
      return params;
    }
    
    void IntermediateGeneratorSemanticsHandler::SetupBuiltins (const ScopeImplPtr& scope)
    {
      // TODO: Check if less functions can be used; e.g. instead of all the overloads for vector types, let casts do the work
      
      TypePtr vecTypeInt[5];
      TypePtr vecTypeUInt[5];
      TypePtr vecTypeFloat[5];
      for (unsigned int c = 1; c < 5; c++)
      {
	vecTypeInt[c] = CreateVectorType (intType, c);
	vecTypeUInt[c] = CreateVectorType (uintType, c);
	vecTypeFloat[c] = CreateVectorType (floatType, c);
      }
      
      TypePtr matTypeInt[5][5];
      TypePtr matTypeUInt[5][5];
      TypePtr matTypeFloat[5][5];
      for (unsigned int c = 1; c < 5; c++)
      {
	for (unsigned int r = 1; r < 5; r++)
	{
	  matTypeInt[r][c] = CreateMatrixType (intType, c, r);
	  matTypeUInt[r][c] = CreateMatrixType (uintType, c, r);
	  matTypeFloat[r][c] = CreateMatrixType (floatType, c, r);
	}
      }
      
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::pow,
							      floatType,
							      UnicodeString ("pow"),
							      MakeFormalParameters2 (floatType)));
							      
      for (unsigned int c = 1; c < 5; c++)
      {
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::dot,
								vecTypeInt[c],
								UnicodeString ("dot"),
								MakeFormalParameters2 (vecTypeInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::dot,
								vecTypeUInt[c],
								UnicodeString ("dot"),
								MakeFormalParameters2 (vecTypeUInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::dot,
								vecTypeFloat[c],
								UnicodeString ("dot"),
								MakeFormalParameters2 (vecTypeFloat[c])));
      }
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::cross,
							      vecTypeInt[3],
							      UnicodeString ("cross"),
							      MakeFormalParameters2 (vecTypeInt[3])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::cross,
							      vecTypeUInt[3],
							      UnicodeString ("cross"),
							      MakeFormalParameters2 (vecTypeUInt[3])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::cross,
							      vecTypeFloat[3],
							      UnicodeString ("cross"),
							      MakeFormalParameters2 (vecTypeFloat[3])));

      for (unsigned int l = 1; l < 5; l++)
      {
	for (unsigned int m = 1; m < 5; m++)
	{
	  for (unsigned int n = 1; n < 5; n++)
	  {
	    scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::mul,
								    matTypeInt[l][n],
								    UnicodeString ("mul"),
								    MakeFormalParameters2 (matTypeInt[l][m], matTypeInt[m][n])));
	    scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::mul,
								    matTypeUInt[l][n],
								    UnicodeString ("mul"),
								    MakeFormalParameters2 (matTypeUInt[l][m], matTypeUInt[m][n])));
	    scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::mul,
								    matTypeFloat[l][n],
								    UnicodeString ("mul"),
								    MakeFormalParameters2 (matTypeFloat[l][m], matTypeFloat[m][n])));
	  }
	}
      }

      for (unsigned int m = 1; m < 5; m++)
      {
	for (unsigned int n = 1; n < 5; n++)
	{
	  scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::mul,
								  vecTypeInt[n],
								  UnicodeString ("mul"),
								  MakeFormalParameters2 (vecTypeInt[m], matTypeInt[m][n])));
	  scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::mul,
								  vecTypeUInt[n],
								  UnicodeString ("mul"),
								  MakeFormalParameters2 (vecTypeUInt[m], matTypeUInt[m][n])));
	  scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::mul,
								  vecTypeFloat[n],
								  UnicodeString ("mul"),
								  MakeFormalParameters2 (vecTypeFloat[m], matTypeFloat[m][n])));
	}
      }
      
      for (unsigned int l = 1; l < 5; l++)
      {
	for (unsigned int m = 1; m < 5; m++)
	{
	  scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::mul,
								  vecTypeInt[l],
								  UnicodeString ("mul"),
								  MakeFormalParameters2 (matTypeInt[l][m], vecTypeInt[m])));
	  scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::mul,
								  vecTypeUInt[l],
								  UnicodeString ("mul"),
								  MakeFormalParameters2 (matTypeUInt[l][m], vecTypeUInt[m])));
	  scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::mul,
								  vecTypeFloat[l],
								  UnicodeString ("mul"),
								  MakeFormalParameters2 (matTypeFloat[l][m], vecTypeFloat[m])));
	}
      }
      
      for (unsigned int c = 1; c < 5; c++)
      {
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::normalize,
								vecTypeFloat[c],
								UnicodeString ("normalize"),
								MakeFormalParameters1 (vecTypeInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::normalize,
								vecTypeFloat[c],
								UnicodeString ("normalize"),
								MakeFormalParameters1 (vecTypeUInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::normalize,
								vecTypeFloat[c],
								UnicodeString ("normalize"),
								MakeFormalParameters1 (vecTypeFloat[c])));
      }
      
      for (unsigned int c = 1; c < 5; c++)
      {
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::length,
								floatType,
								UnicodeString ("length"),
								MakeFormalParameters1 (vecTypeInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::length,
								floatType,
								UnicodeString ("length"),
								MakeFormalParameters1 (vecTypeUInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::length,
								floatType,
								UnicodeString ("length"),
								MakeFormalParameters1 (vecTypeFloat[c])));
      }
      
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::tex1D,
							      vecTypeFloat[4],
							      UnicodeString ("tex1D"),
							      MakeFormalParameters2 (CreateSamplerType (_1D), vecTypeInt[1])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::tex1D,
							      vecTypeFloat[4],
							      UnicodeString ("tex1D"),
							      MakeFormalParameters2 (CreateSamplerType (_1D), vecTypeUInt[1])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::tex1D,
							      vecTypeFloat[4],
							      UnicodeString ("tex1D"),
							      MakeFormalParameters2 (CreateSamplerType (_1D), vecTypeFloat[1])));
      
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::tex2D,
							      vecTypeFloat[4],
							      UnicodeString ("tex2D"),
							      MakeFormalParameters2 (CreateSamplerType (_2D), vecTypeInt[2])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::tex2D,
							      vecTypeFloat[4],
							      UnicodeString ("tex2D"),
							      MakeFormalParameters2 (CreateSamplerType (_2D), vecTypeUInt[2])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::tex2D,
							      vecTypeFloat[4],
							      UnicodeString ("tex2D"),
							      MakeFormalParameters2 (CreateSamplerType (_2D), vecTypeFloat[2])));
      
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::tex3D,
							      vecTypeFloat[4],
							      UnicodeString ("tex3D"),
							      MakeFormalParameters2 (CreateSamplerType (_3D), vecTypeInt[3])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::tex3D,
							      vecTypeFloat[4],
							      UnicodeString ("tex3D"),
							      MakeFormalParameters2 (CreateSamplerType (_3D), vecTypeUInt[3])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::tex3D,
							      vecTypeFloat[4],
							      UnicodeString ("tex3D"),
							      MakeFormalParameters2 (CreateSamplerType (_3D), vecTypeFloat[3])));
      
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::texCUBE,
							      vecTypeFloat[4],
							      UnicodeString ("texCUBE"),
							      MakeFormalParameters2 (CreateSamplerType (CUBE), vecTypeInt[3])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::texCUBE,
							      vecTypeFloat[4],
							      UnicodeString ("texCUBE"),
							      MakeFormalParameters2 (CreateSamplerType (CUBE), vecTypeUInt[3])));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::texCUBE,
							      vecTypeFloat[4],
							      UnicodeString ("texCUBE"),
							      MakeFormalParameters2 (CreateSamplerType (CUBE), vecTypeFloat[3])));
							      
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::min,
							      intType,
							      UnicodeString ("min"),
							      MakeFormalParameters2 (intType)));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::min,
							      uintType,
							      UnicodeString ("min"),
							      MakeFormalParameters2 (uintType)));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::min,
							      floatType,
							      UnicodeString ("min"),
							      MakeFormalParameters2 (floatType)));
      for (unsigned int c = 1; c < 5; c++)
      {
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::min,
								vecTypeInt[c],
								UnicodeString ("min"),
								MakeFormalParameters2 (vecTypeInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::min,
								vecTypeUInt[c],
								UnicodeString ("min"),
								MakeFormalParameters2 (vecTypeUInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::min,
								vecTypeFloat[c],
								UnicodeString ("min"),
								MakeFormalParameters2 (vecTypeFloat[c])));
      }
							      
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::max,
							      intType,
							      UnicodeString ("max"),
							      MakeFormalParameters2 (intType)));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::max,
							      uintType,
							      UnicodeString ("max"),
							      MakeFormalParameters2 (uintType)));
      scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::max,
							      floatType,
							      UnicodeString ("max"),
							      MakeFormalParameters2 (floatType)));
      for (unsigned int c = 1; c < 5; c++)
      {
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::max,
								vecTypeInt[c],
								UnicodeString ("max"),
								MakeFormalParameters2 (vecTypeInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::max,
								vecTypeUInt[c],
								UnicodeString ("max"),
								MakeFormalParameters2 (vecTypeUInt[c])));
	scope->AddBuiltinFunction (boost::make_shared<Builtin> (intermediate::max,
								vecTypeFloat[c],
								UnicodeString ("max"),
								MakeFormalParameters2 (vecTypeFloat[c])));
      }
    }
  } // namespace intermediate
} // namespace s1
