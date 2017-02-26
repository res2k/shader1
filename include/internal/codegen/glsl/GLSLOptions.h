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
 * GLSL code generation options container
 */
#ifndef __CODEGEN_GLSL_OPTIONS_H__
#define __CODEGEN_GLSL_OPTIONS_H__

#include "codegen/sl/SLOptions.h"

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      class Options : public sl::Options
      {
      protected:
        /**
         * Options to instruct backend to avoid generating the 'in' qualifier
         * for function parameters. (Workaround for Qt QML shader support.)
         */
        boost::optional<bool> avoidQualifierIn;

        class GLSLDeclarations : public SLDeclarations
        {
        public:
          GLSLDeclarations ()
          {
            this->RegisterFlag ("avoid-qualifier-in", &Options::avoidQualifierIn);
          }
        };
        static GLSLDeclarations declarations;
        const Declarations& GetDeclarations () const override { return declarations; }
      public:
        /// Constructor: all uninitialized
        Options () {}
        /// Constructor: default values for all
        Options (bool debugAnnotate) : sl::Options (debugAnnotate) {}

        /// Whether to avoid generating the 'in' qualifier for function parameters
        bool GetAvoidQualifierIn () const { return avoidQualifierIn.value_or (false); }
      };
    } //
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_GLSL_OPTIONS_H__
