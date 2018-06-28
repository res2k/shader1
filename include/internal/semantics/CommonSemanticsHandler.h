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

#include "Handler.h"

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
