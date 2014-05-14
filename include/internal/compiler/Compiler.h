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

#ifndef __COMPILER_COMPILER_H__
#define __COMPILER_COMPILER_H__

#include <boost/intrusive_ptr.hpp>

namespace s1
{
  class Library;

  // "Friendly" class for compiler controls
  class Compiler
  {
    Library* lib;
  public:
    Compiler (Library* lib = nullptr) : lib (lib) {}

    class Options;
    typedef boost::intrusive_ptr<Options> OptionsPtr;
    
    OptionsPtr CreateOptions ();
    
    class Backend;
    typedef boost::intrusive_ptr<Backend> BackendPtr;
    
    BackendPtr CreateBackendCg ();
    
    class Program;
    typedef boost::intrusive_ptr<Program> ProgramPtr;
    
    ProgramPtr CreateProgram (std::istream& input, const UnicodeString& entryFunction);
    
    // Methods to specify input
    // Optimization getters/setters
    // Methods to query messages/warnings/errors
    // Methods to set input parameter frequency
    // (Command line parser?)
    // Probably also: query input parameters
    // Getting output (using a "Backend"?)
    // Also need to query: main function names,
    //   mappings between original and output param names,
    //   (...Backend dependent)
  
    // -> Also "Program" object for these aspects?
  };
} // namespace s1

#endif // __COMPILER_COMPILER_H__
