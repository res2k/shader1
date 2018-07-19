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

#ifndef __INTERMEDIATE_NAMEIMPL_H__
#define __INTERMEDIATE_NAMEIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "semantics/CommonName.h"

#include <boost/optional.hpp>

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::NameImpl : public semantics::CommonName
    {
      ScopeImpl* ownerScope;

      // Parameter info, if it's originally a function parameter
      boost::optional<semantics::Scope::FunctionFormalParameter> paramInfo;
    public:
      NameImpl (ScopeImpl* ownerScope,
                const uc::String& identifier, NameType type,
                semantics::Type* typeOfName)
        : CommonName (identifier, type, typeOfName), ownerScope (ownerScope) {}
      NameImpl (ScopeImpl* ownerScope,
                const uc::String& identifier,
                semantics::Type* typeOfName,
                ExpressionPtr value, bool constant)
        : CommonName (identifier, typeOfName, value, constant), ownerScope (ownerScope) {}
      NameImpl (ScopeImpl* ownerScope,
                const semantics::Scope::FunctionFormalParameter& param)
        : CommonName (param.identifier, param.type.get(), param.defaultValue,
                      param.dir == semantics::Scope::dirIn),
          ownerScope (ownerScope), paramInfo (param) {}

      ScopeImpl* GetOwnerScope() const { return ownerScope; }
      const semantics::Scope::FunctionFormalParameter* GetParamInfo() const { return paramInfo.get_ptr(); }
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_NAMEIMPL_H__
