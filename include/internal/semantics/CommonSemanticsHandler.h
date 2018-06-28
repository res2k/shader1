/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_COMMONSEMANTICSHANDLER_H_
#define SEMANTICS_COMMONSEMANTICSHANDLER_H_

#include "Block.h"
#include "Function.h"
#include "Handler.h"
#include "Scope.h"

#include <boost/unordered_map.hpp>

namespace s1
{
  namespace semantics
  {
    /**
     * Common implementation of semantics handler behaviour that is the same across
     * most semantics handlers (such as name or scope handling).
     */
    class CommonSemanticsHandler : public Handler
    {
    protected:
      /**\name Type utilities
       * @{ */
      static CommonType* GetHigherPrecisionType (CommonType* t1, CommonType* t2);
      static BaseType DetectNumericType (const uc::String& numericStr);
      /**@}*/
      
      /**\name Attribute utilities
       * @{ */
      TypePtr GetAttributeType (CommonType* expressionType, const Attribute& attr);
      /** @} */

      class CommonScope : public Scope
      {
        friend class CommonSemanticsHandler;
        
        typedef boost::unordered_map<uc::String, NamePtr> IdentifierMap;
        IdentifierMap identifiers;
        
        bool CheckIdentifierUnique (const uc::String& identifier);
        
        CommonSemanticsHandler* handler;
        boost::intrusive_ptr<CommonScope> parent;
        ScopeLevel level;

        class CommonFunction : public Function
        {
          BlockPtr block;
        public:
          CommonFunction (const BlockPtr& block) : block (block) {}
          BlockPtr GetBody() { return block; }
          void Finish() {}
        };
      public:
        CommonScope (CommonSemanticsHandler* handler, CommonScope* parent, ScopeLevel level);
        ScopeLevel GetLevel() const { return level; }
        
        NamePtr AddVariable (TypePtr type,
          const uc::String& identifier,
          ExpressionPtr initialValue,
          bool constant);
          
        NamePtr AddTypeAlias (TypePtr aliasedType,
          const uc::String& identifier);
          
        FunctionPtr AddFunction (TypePtr returnType,
          const uc::String& identifier,
          const FunctionFormalParameters& params);
      
        result_NamePtr ResolveIdentifier (const uc::String& identifier);
      };
      
    public:  
      TypePtr CreateType (BaseType type);
      TypePtr CreateSamplerType (SamplerType dim);
      TypePtr CreateArrayType (TypePtr baseType);
      TypePtr CreateVectorType (TypePtr baseType,
                                           unsigned int components);
      TypePtr CreateMatrixType (TypePtr baseType,
                                           unsigned int columns,
                                           unsigned int rows);
      
      ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel);
    };
    
  } // namespace semantics
} // namespace s1
    
#endif // SEMANTICS_COMMONSEMANTICSHANDLER_H_
