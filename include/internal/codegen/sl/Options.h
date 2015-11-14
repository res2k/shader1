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
 * Common SL code generation options container
 */
#ifndef __CODEGEN_SL_OPTIONS_H__
#define __CODEGEN_SL_OPTIONS_H__

#include "codegen/common/Options.h"

namespace s1
{
  namespace codegen
  {
    namespace sl
    {
      class Options : public codegen::Options<sl::Options>
      {
      protected:
        class SLDeclarations : public CommonDeclarations {};

        friend class OptionsBase<Options>;
        virtual const Declarations& GetDeclarations () const = 0;
      public:
        Options () {}
      };
    } // namespace sl
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SL_OPTIONS_H__
