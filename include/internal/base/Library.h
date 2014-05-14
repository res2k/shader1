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

/**\file
 * "Library" object
 */
#ifndef __BASE_LIBRARY_H__
#define __BASE_LIBRARY_H__

#include "base/Object.h"
#include "compiler/Compiler.h"

#include "s1/Error.h"

#include <boost/intrusive_ptr.hpp>

namespace s1
{
  class Library : public Object
  {
    s1_ResultCode lastError; // TODO: Store with thread affinity
    /// The internal factory object
    Compiler compiler;  // TODO: Can probably be removed
  public:
    Library() : lastError (S1_SUCCESS), compiler (this) {}
    
    // TODO: Store stuff like memory allocator, global options etc... here
    s1_ResultCode GetLastError () { return lastError; }
    void SetLastError (s1_ResultCode code) { lastError = code; }
    
    Compiler& GetCompiler() { return compiler; }
  };
  typedef boost::intrusive_ptr<Library> LibraryPtr;
} // namespace s1

#endif // __BASE_LIBRARY_H__
