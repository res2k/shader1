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

#ifndef SEMANTICS_COMMONSCOPE_H_
#define SEMANTICS_COMMONSCOPE_H_

#include "Block.h"
#include "Function.h"
#include "Scope.h"

#include <boost/unordered_map.hpp>

namespace s1
{
  namespace semantics
  {
    class CommonHandler;

    class CommonScope : public Scope
    {
      friend class CommonHandler;
      
      CommonHandler* handler;
      ScopeLevel level;
    public:
      CommonScope (CommonHandler* handler, CommonScope* parent, ScopeLevel level);
      ScopeLevel GetLevel() const { return level; }
      
      NameTypeAliasPtr AddTypeAlias (TypePtr aliasedType,
        const uc::String& identifier) override;
        
      FunctionPtr AddFunction (TypePtr returnType,
        const uc::String& identifier,
        const FunctionFormalParameters& params);
    };
  } // namespace semantics
} // namespace s1
    
#endif // SEMANTICS_COMMONSCOPE_H_
