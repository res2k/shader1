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

#ifndef __COMPILER_BACKEND_H__
#define __COMPILER_BACKEND_H__

#include "base/LibraryObject.h"

#include "Compiler.h"

#include "intermediate/forwarddecl.h"
#include "splitter/Frequency.h"

namespace s1
{
  class Compiler::Backend : public LibraryObject
  {
  public:
    class Program : public LibraryObject
    {
    public:
      Program (Library* lib) : LibraryObject (lib) {}

      virtual const std::string& GetProgramString () = 0;
    };
    typedef boost::intrusive_ptr<Program> ProgramPtr;

    class Options : public LibraryObject
    {
    protected:
      typedef std::pair<uc::String, bool> FlagPair;
      FlagPair ParseFlagPair (const uc::String& string);
    public:
      Options (Library* lib) : LibraryObject (lib) {}

      virtual bool SetFromStr (const char* string) = 0;
    };
    typedef boost::intrusive_ptr<Options> OptionsPtr;

    /// Default Options implementation backed by codegen options
    template<typename CodegenOptions>
    class OptionsImpl : public Options
    {
    protected:
      CodegenOptions optionsContainer;
    public:
      OptionsImpl (Library* lib) : Options (lib) {}

      bool SetFromStr (const char* string) override
      {
        uc::String string_u (string);
        /* TODO: Check if string contains '=' or ' ', parse as a name/value
         * pair then */
        FlagPair flag = ParseFlagPair (string_u);
        return optionsContainer.SetFlag (flag.first, flag.second);
      }

      const CodegenOptions& GetContainer () const { return optionsContainer; }
    };

    Backend (Library* lib) : LibraryObject (lib) {}
    
    enum CompileTarget
    {
      targetUnsplit,
      targetVP,
      targetFP
    };
    virtual ProgramPtr GenerateProgram (CompileTarget target,
					const intermediate::ProgramPtr& prog,
                                        OptionsPtr options) = 0;
  };
} // namespace s1

#endif // __COMPILER_BACKEND_H__
