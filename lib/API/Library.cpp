/*
    Shader1
    Copyright (c) 2010-2016 Frank Richter


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

#include "s1/Library.h"

#include "base/DebugMessageHandler.h"
#include "base/Library.h"
#include "base/uc/String_optional.h"
#include "compiler/Backend.h"
#include "compiler/Options.h"

#include "Program.h"

#include <new>
#include <boost/algorithm/string/predicate.hpp>

s1_ResultCode s1_create_library (s1_Library** out)
{
  if (!out) return S1_E_INVALID_ARG_N(0);
  
  s1::Library* new_lib (new (std::nothrow) s1::Library);
  if (!new_lib) return S1_E_OUT_OF_MEMORY;
  new_lib->AddRef();
  *out = new_lib->DowncastEvil<s1_Library> ();
  return S1_SUCCESS;
}

s1_ResultCode s1_library_get_last_error (s1_Library* lib)
{
  S1_ASSERT_MSG(lib, "NULL Library",
                S1_MAKE_ERROR(S1_RESULT_COMP_BASE, 0xbad1));
  return s1::EvilUpcast<s1::Library> (lib)->GetLastError();
}

void s1_library_clear_last_error (s1_Library* lib)
{
  S1_ASSERT_MSG(lib, "NULL Library", S1_ASSERT_RET_VOID);
  s1::EvilUpcast<s1::Library> (lib)->SetLastError (S1_SUCCESS);
}

const char* s1_library_get_last_error_info (s1_Library* lib)
{
  S1_ASSERT_MSG(lib, "NULL Library", nullptr);
  return s1::EvilUpcast<s1::Library> (lib)->GetLastErrorInfo();
}

void s1_library_set_debug_message_handler (s1_Library* lib,
                                           s1_debug_message_handler_func handler,
                                           uintptr_t userContext)
{
  S1_ASSERT_MSG (lib, "NULL Library", S1_ASSERT_RET_VOID);
  auto libImpl = s1::EvilUpcast<s1::Library> (lib);
  libImpl->GetDebugMessageHandler ().SetHandler (handler, userContext);
}

void s1_library_set_debug_message_handler_ws (s1_Library* lib,
                                              s1_debug_message_handler_ws_func handler,
                                              uintptr_t userContext)
{
  S1_ASSERT_MSG (lib, "NULL Library", S1_ASSERT_RET_VOID);
  auto libImpl = s1::EvilUpcast<s1::Library> (lib);
  libImpl->GetDebugMessageHandler ().SetHandlerWS (handler, userContext);
}

s1_debug_message_handler_func s1_library_get_debug_message_handler (s1_Library* lib,
                                                                    uintptr_t* userContextPtr)
{
  S1_ASSERT_MSG (lib, "NULL Library", nullptr);
  auto libImpl = s1::EvilUpcast<s1::Library> (lib);
  return libImpl->GetDebugMessageHandler ().GetHandler (userContextPtr);
}

s1_debug_message_handler_ws_func s1_library_get_debug_message_handler_ws (s1_Library* lib,
                                                                          uintptr_t* userContextPtr)
{
  S1_ASSERT_MSG (lib, "NULL Library", nullptr);
  auto libImpl = s1::EvilUpcast<s1::Library> (lib);
  return libImpl->GetDebugMessageHandler ().GetHandlerWS (userContextPtr);
}


s1_Options* s1_options_create (s1_Library* obj)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::Library> (obj));
  
  return lib->Return (lib->Try (
    [=]() {
      s1::Compiler::OptionsPtr options (lib->GetCompiler().CreateOptions());
      options->AddRef();
      return options->DowncastEvil<s1_Options> ();
    }), nullptr);
}

s1_Program* s1_program_create_from_string (s1_Library* obj, const char* source,
                                           size_t sourceSize, unsigned int compatLevel)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::Library> (obj));
  
  if (!source)
  {
    lib->SetLastError (S1_E_INVALID_ARG_N (0));
    return nullptr;
  }
  
  if (compatLevel > S1_COMPATIBILITY_LATEST)
  {
    lib->SetLastError (S1_E_INCOMPATIBLE);
    return nullptr;
  }
  
  return lib->Return (lib->Try (
    [=]() {
      std::string sourceStr (source, sourceSize);
      boost::intrusive_ptr<s1::api_impl::Program> program (
        new s1::api_impl::Program (lib, lib->GetCompiler (), sourceStr));
      program->AddRef ();
      return program->DowncastEvil<s1_Program> ();
    }), nullptr);
}

static s1_Backend* s1_backend_create_ucs (s1_Library* obj, s1::uc::String_optional backend)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::Library> (obj));
  
  if (!backend)
  {
    lib->SetLastError (S1_E_INVALID_ARG_N (0));
    return nullptr;
  }
  s1::Compiler::SupportedBackend compiler_backend;
  if (boost::algorithm::equals (*backend, "cg"))
  {
    compiler_backend = s1::Compiler::beCg;
  }
  else if (boost::algorithm::equals (*backend, "glsl"))
  {
    compiler_backend = s1::Compiler::beGLSL;
  }
  else
  {
    lib->SetLastError (S1_E_UNKNOWN_BACKEND);
    return nullptr;
  }

  return lib->Return (lib->Try ([=]()
    {
      s1::Compiler::BackendPtr backend_obj (lib->GetCompiler().CreateBackend (compiler_backend));
      backend_obj->AddRef();
      return backend_obj->DowncastEvil<s1_Backend> ();
    }), nullptr);
}

s1_Backend* s1_backend_create (s1_Library* obj, const char* backend)
{
  return s1_backend_create_ucs (obj, s1::uc::make_String_optional (backend));
}

s1_Backend* s1_backend_create_ws (s1_Library* obj, const wchar_t* backend)
{
  return s1_backend_create_ucs (obj, s1::uc::make_String_optional (backend));
}

s1_Backend* s1_backend_create_u16 (s1_Library* obj, const s1_char16* backend)
{
  return s1_backend_create_ucs (obj, s1::uc::make_String_optional (backend));
}

s1_Backend* s1_backend_create_u32 (s1_Library* obj, const s1_char32* backend)
{
  return s1_backend_create_ucs (obj, s1::uc::make_String_optional (backend));
}
