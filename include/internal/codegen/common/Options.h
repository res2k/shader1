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
 * Cmmon code generation options container
 */
#ifndef __CODEGEN_OPTIONS_H__
#define __CODEGEN_OPTIONS_H__

#include "OptionsBase.h"

namespace s1
{
  namespace codegen
  {
    template<typename Derived>
    class Options : public OptionsBase<Derived>
    {
    protected:
      /// Flag value for 'debug annotations' option
      boost::optional<bool> debugAnnotationsFlag;

      class CommonDeclarations : public OptionsBase<Derived>::Declarations
      {
      public:
        CommonDeclarations ()
        {
          this->RegisterFlag ("annotate", &Options::debugAnnotationsFlag);
        }
      };
    public:
      /// Constructor: all uninitialized
      Options () {}
      /// Constructor: default values for all
      Options (bool debugAnnotate) : debugAnnotationsFlag (debugAnnotate) {}
      /// Return value of debug annotation flag.
      bool GetDebugAnnotationFlag () const
      {
        return *debugAnnotationsFlag;
      }
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_OPTIONS_H__
