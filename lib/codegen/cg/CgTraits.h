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
* Cg code generation language traits
*/
#ifndef __CODEGEN_CG_CGTRAITS_H__
#define __CODEGEN_CG_CGTRAITS_H__

#include "codegen/sl/Traits.h"

namespace s1
{
  namespace codegen
  {
    class CgTraits : public sl::Traits
    {
      uc::String typeStrBool;
      uc::String typeStrFloat;
      uc::String typeStrInt;
      uc::String typeStrUInt;
    public:
      static CgTraits instance;

      CgTraits ();

      uc::String ConvertIdentifier (const uc::String& identifier) const override;
      uc::String TypeString (intermediate::BasicType type) const override;
      std::pair<uc::String, uc::String> TypeString (const parser::SemanticsHandler::TypePtr& type,
                                                    const size_t* arraySize) const override;
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_CG_CGTRAITS_H__
