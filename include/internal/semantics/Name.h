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

#ifndef SEMANTICS_NAME_H_
#define SEMANTICS_NAME_H_

#include "Base.h"
#include "FunctionFormalParameter.h"
#include "Scope.h"

#include <boost/optional.hpp>
#include <boost/type_traits/copy_cv.hpp>

namespace s1
{
  namespace semantics
  {
    /**
     * A name refers to a variable, an attribute of it or an element.
     */
    class Name : public Base
    {
    public:
      /// Type of name (function or variable/parameter)
      enum NameType { Function, Variable, TypeAlias };
    protected:
      Scope* ownerScope;
      uc::String identifier;

      Name (Scope* ownerScope, const uc::String& identifier) : ownerScope (ownerScope), identifier (identifier) {}

      template<typename T, NameType ExpectedType>
      static T* internal_upcast (typename boost::copy_cv<Name, T>::type* name)
      {
        if (!name) return nullptr;
        if (name->GetType() != ExpectedType) return nullptr;
        return static_cast<T*> (name);
      }
    public:
      virtual ~Name() {}

      /// Get scope this name is declared in
      Scope* GetOwnerScope() const { return ownerScope; }

      /// Get type of name
      virtual NameType GetType() const = 0;

      /// Get name identifier
      const uc::String& GetIdentifier () { return identifier; }
    };

    class NameFunction : public Name
    {
    public:
      typedef std::vector<BaseFunctionPtr> BaseFunctionPtrVec;
    protected:
      /// Overloads for the function name
      BaseFunctionPtrVec overloads;
    public:
      NameFunction (Scope* ownerScope, const uc::String& identifier);
      ~NameFunction ();

      NameType GetType() const override { return Function; }

      /// Add an overload for the function name
      semantics::Function* AddOverload (Type* returnType, const FunctionFormalParameters& params, Scope* funcScope, Block* body);
      /// Add an overload with a built-in function
      semantics::BuiltinFunction* AddBuiltin (Type* returnType, const FunctionFormalParameters& params, Builtin which);
      /// Get overloads for the function name
      const BaseFunctionPtrVec& GetOverloads () const { return overloads; }
      /// Collect overloads fitting the given parameter types
      BaseFunctionPtrVec CollectOverloadCandidates (const std::vector<Type*>& paramTypes) const;

      static NameFunction* upcast (Name* name) { return internal_upcast<NameFunction, Function> (name); }
      static const NameFunction* upcast (const Name* name) { return internal_upcast<const NameFunction, Function> (name); }
    };

    class NameVariable : public Name
    {
      /// Type of variable/constant
      TypePtr valueType;
      /// value
      ExpressionPtr varValue;
      /// Distinguish between variable/constant
      bool varConstant;

      // Parameter info, if it's originally a function parameter
      boost::optional<FunctionFormalParameter> paramInfo;
    public:
      NameVariable (Scope* ownerScope, const uc::String& identifier, Type* valueType,
                    Expression* value, bool constant)
        : Name (ownerScope, identifier), valueType (valueType),
          varValue (value), varConstant (constant) {}
      NameVariable (Scope* ownerScope, const FunctionFormalParameter& param)
        : NameVariable (ownerScope, param.identifier, param.type.get(), param.defaultValue.get(),
                        param.dir == FunctionFormalParameter::dirIn)
      {
        paramInfo = param;
      }

      NameType GetType() const override { return Variable; }

      /// Return type of variable value
      TypePtr GetValueType () { return valueType; }

      /// Get initialization value for variables
      Expression* GetValue() const { return varValue.get(); }

      /// Return whether a variable name is a constant
      bool IsConstant () { return varConstant; }

      /// Return parameter info, if name was originally a function parameter
      const FunctionFormalParameter* GetParamInfo() const { return paramInfo.get_ptr(); }

      static NameVariable* upcast (Name* name) { return internal_upcast<NameVariable, Variable> (name); }
      static const NameVariable* upcast (const Name* name) { return internal_upcast<const NameVariable, Variable> (name); }
    };

    class NameTypeAlias : public Name
    {
      /// aliased type
      TypePtr aliasedType;
    public:
      NameTypeAlias (Scope* ownerScope, const uc::String& identifier, Type* aliasedType)
        : Name (ownerScope,  identifier), aliasedType (aliasedType) {}

      NameType GetType() const override { return TypeAlias; }

      /// Get aliased type
      TypePtr GetAliasedType() { return aliasedType; }

      static NameTypeAlias* upcast (Name* name) { return internal_upcast<NameTypeAlias, TypeAlias> (name); }
      static const NameTypeAlias* upcast (const Name* name) { return internal_upcast<const NameTypeAlias, TypeAlias> (name); }
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_NAME_H_
