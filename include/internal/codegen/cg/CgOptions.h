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
* Cg code generation options container
*/
#ifndef __CODEGEN_CG_CGOPTIONS_H__
#define __CODEGEN_CG_CGOPTIONS_H__

#include "codegen/common/Options.h"

namespace s1
{
  namespace codegen
  {
    class CgOptions : public Options<CgOptions>
    {
    protected:
      class CgDeclarations : public CommonDeclarations {};
      static CgDeclarations declarations;
      friend class OptionsBase<CgOptions>;
      const Declarations& GetDeclarations () const { return declarations;  }
    public:
      CgOptions () {}
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_CG_CGOPTIONS_H__
