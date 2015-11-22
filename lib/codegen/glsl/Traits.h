/*
Shader1
Copyright (c) 2015 Frank Richter


This library is free software; you can redistribute it and/or
modify it under the terms of the wxWindows Library Licence.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Please see the wxWindows Library Licence for more details.
The licence texts can be found in the source distribution files
LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * GLSL code generation language traits
 */
#ifndef __CODEGEN_GLSL_TRAITS_H__
#define __CODEGEN_GLSL_TRAITS_H__

#include "codegen/sl/Traits.h"

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      class Traits : public sl::Traits
      {
        uc::String typeStrBool;
        uc::String typeStrFloat;
        uc::String typeStrInt;
        uc::String typeStrUInt;
      public:
        static Traits instance;

        Traits ();

        uc::String TypeString (intermediate::BasicType type) const override;
        uc::String FormatVector (intermediate::BasicType type, unsigned int componentCount) const override;
        uc::String FormatMatrix (intermediate::BasicType type, unsigned int rowCount,
                                 unsigned int colCount) const override;
        std::pair<uc::String, uc::String> TypeString (const parser::SemanticsHandler::TypePtr& type,
                                                      const size_t* arraySize) const override;
      };
    } // namespace glsl
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_GLSL_TRAITS_H__
