/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include "base/common.h"
#include "base/ResultCode_internal.h"

#include "CompiledProgram.h"

#include "compiler/Backend.h"
#include "compiler/BackendProgram.h"

#include "ByteStream.h"

/// Compute length of joined string array
static size_t FlattenedSize (const s1::codegen::StringsArrayPtr& strings)
{
  size_t totalSize = 0;
  for (size_t i = 0; i < strings->Size (); i++)
  {
    totalSize += strings->Get (i).size () + 1;
  }
  return totalSize;
}

/// Join the strings arrays with newlines.
static std::string FlattenStringArray (const s1::codegen::StringsArrayPtr& strings)
{
  std::string str;
  str.reserve (FlattenedSize (strings));
  for (size_t i = 0; i < strings->Size(); i++)
  {
    str.append (strings->Get (i));
    str.append ("\n");
  }
  return str;
}

namespace s1
{
  namespace api_impl
  {
    namespace
    {
      class ProgramLinesByteStream : public ByteStream
      {
      public:
        ProgramLinesByteStream (s1::Library* lib, const s1::codegen::StringsArrayPtr& lines);

        SizeResult Size () override;
        size_t Next (const char*& buf) override;
        CreateRestartedResult CreateRestarted () override;
      protected:
        s1::codegen::StringsArrayPtr lines;
        size_t currentLine = 0;
        bool nextIsNewline = false;
      };

      ProgramLinesByteStream::ProgramLinesByteStream (s1::Library* lib, const s1::codegen::StringsArrayPtr& lines)
        : ByteStream (lib), lines (lines) {}

      ByteStream::SizeResult ProgramLinesByteStream::Size ()
      {
        return SizeResult (S1_SUCCESS, FlattenedSize (lines));
      }

      size_t ProgramLinesByteStream::Next (const char*& buf)
      {
        static const char newline[] = "\n";

        if (nextIsNewline)
        {
          nextIsNewline = false;
          buf = newline;
          return 1;
        }

        if (currentLine >= lines->Size ()) return 0;
        const auto& line = lines->Get (currentLine);
        currentLine++;
        if (line.empty ())
        {
          // Empty data would mean "end of data", so return newline immediately
          buf = newline;
          return 1;
        }
        buf = line.data ();
        nextIsNewline = true;
        return line.size ();
      }

      ByteStream::CreateRestartedResult ProgramLinesByteStream::CreateRestarted ()
      {
        boost::intrusive_ptr<ByteStream> newStream (new ProgramLinesByteStream (GetLibrary(), lines));
        return std::make_pair (S1_SUCCESS, newStream->DowncastEvil<s1_ByteStream> ());
      }
    } // anonymous namespace

    //-----------------------------------------------------------------------

    struct CompiledProgram::FlatString : public boost::intrusive_ref_counter<FlatString>
    {
      std::string str;
    };

    CompiledProgram::CompiledProgram (s1::Library* lib, Compiler::Backend::ProgramPtr compiledProgram)
      : LibraryObject (lib), compiledProgram (compiledProgram)
    {}

    const char* CompiledProgram::GetString ()
    {
      if (!flatString)
      {
        flatString = new FlatString;
        flatString->str = FlattenStringArray (compiledProgram->GetProgramLines());
        compiledProgram->DiscardLines ();
      }
      return flatString->str.c_str();
    }

    boost::intrusive_ptr<ByteStream> CompiledProgram::CreateStream ()
    {
      boost::intrusive_ptr<ByteStream> stream;
      if (flatString)
      {
        // Create stream from flattened data
        intrusive_ptr_add_ref (flatString.get());
        stream = new DataByteStream (GetLibrary(),
                                      flatString->str.data(), flatString->str.size(),
                                      [](uintptr_t p, const char*){  intrusive_ptr_release (reinterpret_cast<FlatString*> (p)); },
                                      reinterpret_cast<uintptr_t> (flatString.get()));
      }
      else
      {
        // Create stream from strings list
        stream = new ProgramLinesByteStream (GetLibrary(), compiledProgram->GetProgramLines());
      }
      return stream;
    }
  } // namespace api_impl
} // namespace s1

const char* s1_compiledprogram_get_string (s1_CompiledProgram* program)
{
  S1_ASSERT_MSG(program, "NULL CompiledProgram", nullptr);
  auto program_impl (s1::EvilUpcast<s1::api_impl::CompiledProgram> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  return program_impl->Return (program_impl->Try ([=]() {
      return program_impl->GetString();
    }), nullptr);
}

s1_ByteStream* s1_compiledprogram_create_stream (s1_CompiledProgram* program)
{
  S1_ASSERT_MSG(program, "NULL CompiledProgram", nullptr);
  auto program_impl (s1::EvilUpcast<s1::api_impl::CompiledProgram> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  return program_impl->Return (program_impl->Try ([=]() {
      boost::intrusive_ptr<s1::api_impl::ByteStream> stream = program_impl->CreateStream();
      if (stream)
      {
        stream->AddRef ();
        return stream->DowncastEvil<s1_ByteStream> ();
      }
      return (s1_ByteStream*)nullptr;
    }), nullptr);
}
