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

#ifndef __PARSER_COMMONSEMANTICSHANDLER_H__
#define __PARSER_COMMONSEMANTICSHANDLER_H__

#include "SemanticsHandler.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_map.hpp>

namespace s1
{
  namespace parser
  {
    /**
     * Common implementation of semantics handler behaviour that is the same across
     * most semantics handlers (such as name or scope handling).
     */
    class CommonSemanticsHandler : public SemanticsHandler
    {
    protected:
      struct CommonType : public Type
      {
        Class typeClass;
        BaseType base;
        SamplerType sampler;
        TypePtr avmBase;
        unsigned int vectorDim;
        unsigned int matrixCols;
        unsigned int matrixRows;
        
        CommonType (BaseType base) : typeClass (Base), base (base) {}
        CommonType (SamplerType sampler) : typeClass (Sampler), sampler (sampler) {}
        CommonType (TypePtr aBase) : typeClass (Array), avmBase (aBase) {}
        CommonType (TypePtr vBase, unsigned int d)
         : typeClass (Vector), avmBase (vBase), vectorDim (d) {}
        CommonType (TypePtr mBase, unsigned int c, unsigned int r)
         : typeClass (Matrix), avmBase (mBase), matrixCols (c), matrixRows (r) {}
        
        Class GetTypeClass() const { return typeClass; }
        BaseType GetBaseType() const { return base; }
        SamplerType GetSamplerType() const { return sampler; }
        
        TypePtr GetArrayVectorMatrixBaseType() const { return avmBase; }
        
        unsigned int GetVectorTypeComponents() const { return vectorDim; }
        
        unsigned int GetMatrixTypeCols() const { return matrixCols; }
        unsigned int GetMatrixTypeRows() const { return matrixRows; }
        
        /// Returns whether this type is losslessly assignable to \a to.
        bool CompatibleLossless (const CommonType& to) const;
        bool CompatibleLossy (const s1::parser::CommonSemanticsHandler::CommonType& to) const;
        bool IsEqual (const CommonType& other) const;
        bool IsPrecisionHigherEqual (const CommonType& other) const;
        
        uc::String ToString() const;
      };
      
      /**\name Type utilities
       * @{ */
      static boost::shared_ptr<CommonType> GetHigherPrecisionType (
        const boost::shared_ptr<CommonType>& t1, const boost::shared_ptr<CommonType>& t2);
      static BaseType DetectNumericType (const uc::String& numericStr);
      /**@}*/
      
      /**\name Attribute utilities
       * @{ */
      struct Attribute
      {
        enum AttrClass
        {
          Unknown,
          
          arrayLength,
          
          matrixRow,
          matrixCol,
          matrixTranspose,
          matrixInvert,
          
          vectorSwizzle
        };
        AttrClass attrClass;
        unsigned char swizzleCompNum;
        unsigned char swizzleComps;
        
        Attribute (AttrClass ac) : attrClass (ac), swizzleCompNum (0), swizzleComps (0) {}
        Attribute (unsigned char swizNum, unsigned char comp1 = 0, unsigned char comp2 = 0,
                   unsigned char comp3 = 0, unsigned char comp4 = 0)
         : attrClass (vectorSwizzle), swizzleCompNum (swizNum),
           swizzleComps ((comp1 & 3) | ((comp2 & 3) << 2) | ((comp3 & 3) << 4) | ((comp4 & 3) << 6))
        {}
      };
      static Attribute IdentifyAttribute (const uc::String& attributeStr);
      TypePtr GetAttributeType (const boost::shared_ptr<CommonType>& expressionType,
                                const Attribute& attr);
      /** @} */
      
      struct CommonName : public Name
      {
        uc::String identifier;
        NameType type;
        
        /* Variables/Constants: type of variable/constant
         * Functions: type of return value
         * Type aliases: aliased type
         */
        TypePtr valueType;
        // Variables/Constants: value
        ExpressionPtr varValue;
        // Distinguish between variable/constant
        bool varConstant;
        
        CommonName (const uc::String& identifier, NameType type, TypePtr typeOfName)
         : identifier (identifier), type (type), valueType (typeOfName) {}
        CommonName (const uc::String& identifier, TypePtr typeOfName,
                    ExpressionPtr value, bool constant)
         : identifier (identifier), type (Variable), valueType (typeOfName),
           varValue (value), varConstant (constant) {}
        
        NameType GetType() { return type; }
        TypePtr GetAliasedType()
        { return type == TypeAlias ? valueType : TypePtr (); }
        const uc::String& GetIdentifier () { return identifier; }
        bool IsConstantVariable () { return (type == Variable) && varConstant; }
        TypePtr GetValueType () { return valueType; }
      };

      class CommonScope : public Scope,
                          public boost::enable_shared_from_this<CommonScope>
      {
        friend class CommonSemanticsHandler;
        
        typedef boost::unordered_map<uc::String, NamePtr> IdentifierMap;
        IdentifierMap identifiers;
        
        bool CheckIdentifierUnique (const uc::String& identifier);
        
        CommonSemanticsHandler* handler;
        boost::shared_ptr<CommonScope> parent;
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
        CommonScope (CommonSemanticsHandler* handler,
                     const boost::shared_ptr<CommonScope>& parent, ScopeLevel level);
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
    
  } // namespace parser
} // namespace s1
    
#endif // __PARSER_COMMONSEMANTICSHANDLER_H__
