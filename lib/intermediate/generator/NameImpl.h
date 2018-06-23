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

namespace s1
{
  namespace intermediate
  {
    struct IntermediateGeneratorSemanticsHandler::NameImpl : public CommonName
    {
      ScopeImpl* ownerScope;

      // Output parameter
      bool isOutputParam = false;

      NameImpl (ScopeImpl* ownerScope,
                const uc::String& identifier, NameType type,
                TypeImpl* typeOfName)
        : CommonName (identifier, type, typeOfName), ownerScope (ownerScope) {}
      NameImpl (ScopeImpl* ownerScope,
                const uc::String& identifier,
                TypeImpl* typeOfName,
                ExpressionPtr value, bool constant, bool isOutputParam = false)
        : CommonName (identifier, typeOfName, value, constant), ownerScope (ownerScope),
          isOutputParam (isOutputParam) {}

      TypeImplPtr GetValueTypeImpl () const { return boost::static_pointer_cast<TypeImpl> (valueType); }
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_NAMEIMPL_H__
