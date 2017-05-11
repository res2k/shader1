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
#include "ResultCodeHelper.h"
#include "StringArg.h"
#include "StringObj.h"
#include "StringWrapper.h"

#include <new>
#include <boost/algorithm/string/predicate.hpp>

namespace s1
{
  namespace api_impl
  {
    class Library : public s1::Library
    {
    protected:
      struct ApiPerThreadData : public PerThreadData
      {
        boost::optional<StringWrapper> lastErrorInfo;
      };
      PerThreadData* AllocPerThreadData () override { return new ApiPerThreadData; }
    public:
      const char* GetLastErrorInfo ()
      {
        auto& threadData = static_cast<ApiPerThreadData&> (GetPerThreadData());
        auto ucs_info = s1::Library::GetLastErrorInfo ();
        if (ucs_info)
        {
          threadData.lastErrorInfo = *ucs_info;
          return threadData.lastErrorInfo->GetUTF8 ();
        }
        else
        {
          threadData.lastErrorInfo = boost::none;
          return nullptr;
        }
      }
    };
  } // namespace api_impl
} // namespace s1

s1_ResultCode s1_create_library (s1_Library** out)
{
  if (!out) return S1_E_INVALID_ARG_N(0);
  
  try
  {
    s1::Library* new_lib (new (std::nothrow) s1::api_impl::Library);
    if (!new_lib) return S1_E_OUT_OF_MEMORY;
    new_lib->AddRef();
    *out = new_lib->DowncastEvil<s1_Library> ();
    return S1_SUCCESS;
  }
  catch (std::bad_alloc)
  {
    return S1_E_OUT_OF_MEMORY;
  }
  catch (std::exception)
  {
    return S1_E_FAILURE;
  }
}

s1_ResultCode s1_library_get_last_error (s1_Library* lib)
{
  S1_ASSERT_MSG(lib, "NULL Library",
                S1_MAKE_ERROR(S1_RESULT_COMP_BASE, 0xbad1));
  auto libImpl = s1::EvilUpcast<s1::api_impl::Library> (lib);
  s1::ScopedThreadDebugMessageHandler setMsgHandler (libImpl->GetDebugMessageHandler ());
  return libImpl->GetLastError();
}

void s1_library_clear_last_error (s1_Library* lib)
{
  S1_ASSERT_MSG(lib, "NULL Library", S1_ASSERT_RET_VOID);
  auto libImpl = s1::EvilUpcast<s1::api_impl::Library> (lib);
  s1::ScopedThreadDebugMessageHandler setMsgHandler (libImpl->GetDebugMessageHandler ());
  libImpl->SetLastError (S1_SUCCESS);
}

const char* s1_library_get_last_error_info (s1_Library* lib)
{
  S1_ASSERT_MSG(lib, "NULL Library", nullptr);
  auto libImpl = s1::EvilUpcast<s1::api_impl::Library> (lib);
  s1::ScopedThreadDebugMessageHandler setMsgHandler (libImpl->GetDebugMessageHandler ());
  return libImpl->GetLastErrorInfo();
}

void s1_library_set_debug_message_handler (s1_Library* lib,
                                           s1_debug_message_handler_func handler,
                                           uintptr_t userContext)
{
  S1_ASSERT_MSG (lib, "NULL Library", S1_ASSERT_RET_VOID);
  auto libImpl = s1::EvilUpcast<s1::api_impl::Library> (lib);
  libImpl->GetDebugMessageHandler ().SetHandler (handler, userContext);
}

void s1_library_set_debug_message_handler_wcs (s1_Library* lib,
                                               s1_debug_message_handler_wcs_func handler,
                                               uintptr_t userContext)
{
  S1_ASSERT_MSG (lib, "NULL Library", S1_ASSERT_RET_VOID);
  auto libImpl = s1::EvilUpcast<s1::api_impl::Library> (lib);
  libImpl->GetDebugMessageHandler ().SetHandlerWCS (handler, userContext);
}

s1_debug_message_handler_func s1_library_get_debug_message_handler (s1_Library* lib,
                                                                    uintptr_t* userContextPtr)
{
  S1_ASSERT_MSG (lib, "NULL Library", nullptr);
  auto libImpl = s1::EvilUpcast<s1::api_impl::Library> (lib);
  return libImpl->GetDebugMessageHandler ().GetHandler (userContextPtr);
}

s1_debug_message_handler_wcs_func s1_library_get_debug_message_handler_wcs (s1_Library* lib,
                                                                            uintptr_t* userContextPtr)
{
  S1_ASSERT_MSG (lib, "NULL Library", nullptr);
  auto libImpl = s1::EvilUpcast<s1::api_impl::Library> (lib);
  return libImpl->GetDebugMessageHandler ().GetHandlerWCS (userContextPtr);
}


s1_Options* s1_options_create (s1_Library* obj)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::api_impl::Library> (obj));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (lib->GetDebugMessageHandler ());

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
  s1::Library* lib (s1::EvilUpcast<s1::api_impl::Library> (obj));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (lib->GetDebugMessageHandler ());

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

s1_Program* s1_program_create_from_stream (s1_Library* obj, s1_stream_input_func streamFunc,
                                           uintptr_t userContext, unsigned int compatLevel)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::api_impl::Library> (obj));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (lib->GetDebugMessageHandler ());

  if (!streamFunc)
  {
    lib->SetLastError (S1_E_INVALID_ARG_N (0));
    return nullptr;
  }

  if (compatLevel > S1_COMPATIBILITY_LATEST)
  {
    lib->SetLastError (S1_E_INCOMPATIBLE);
    return nullptr;
  }

  auto wrapStreamFunc =
    [=](const char*& data) -> size_t
    { return streamFunc (userContext, &data); };
  return lib->Return (lib->Try (
    [=]() {
      boost::intrusive_ptr<s1::api_impl::Program> program (
        new s1::api_impl::Program (lib, lib->GetCompiler (), wrapStreamFunc));
      program->AddRef ();
      return program->DowncastEvil<s1_Program> ();
    }), nullptr);
}

s1_Backend* s1_backend_create (s1_Library* obj, s1_StringArg backendStr)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::api_impl::Library> (obj));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (lib->GetDebugMessageHandler ());

  return lib->Return (lib->Try (
    [=]() -> s1::Result<s1_Backend*> {
      s1::api_impl::ResolveStringArg backend (backendStr, 0);
      s1::Compiler::SupportedBackend compiler_backend;
      if (boost::algorithm::equals (backend.GetString(), "glsl"))
      {
        compiler_backend = s1::Compiler::beGLSL;
      }
      else
      {
        return S1_E_UNKNOWN_BACKEND;
      }

      s1::Compiler::BackendPtr backend_obj (lib->GetCompiler().CreateBackend (compiler_backend));
      backend_obj->AddRef();
      return backend_obj->DowncastEvil<s1_Backend> ();
    }), nullptr);
}

s1_String* s1_string_create (s1_Library* obj, s1_StringArg string, size_t* invalidPos)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::api_impl::Library> (obj));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (lib->GetDebugMessageHandler ());

  return lib->Return (lib->Try (
    [=]() -> s1::Result<s1_String*> {
      boost::intrusive_ptr<s1::api_impl::String> newString;
      s1::ResultCode createResult = s1::api_impl::String::Create (newString, lib, string, invalidPos);
      createResult = static_cast<s1::ResultCode> (s1::detail::ChangeResultCodeArgumentIndex (createResult, 0));
      if (!newString) return createResult;
      newString->AddRef ();
      return s1::Result<s1_String*> (newString->DowncastEvil<s1_String> (), createResult);
    }), nullptr);
}

template<typename Ch>
static s1_String* s1_string_create_internal (s1_Library* obj, const Ch* string, const Ch** invalidPos)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::api_impl::Library> (obj));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (lib->GetDebugMessageHandler ());

  if (!string) return lib->ReturnErrorCode (S1_E_INVALID_ARG_N (0), nullptr);

  return lib->Return (lib->Try (
    [=]() -> s1::Result<s1_String*> {
      boost::intrusive_ptr<s1::api_impl::String> newString;
      s1::ResultCode createResult = s1::api_impl::String::Create (newString, lib, string, invalidPos);
      if (!newString) return createResult;
      newString->AddRef ();
      return s1::Result<s1_String*> (newString->DowncastEvil<s1_String> (), createResult);
    }), nullptr);
}

s1_String* s1_string_create_u8 (s1_Library* obj, const char* string, const char** invalidPos)
{
  return s1_string_create_internal (obj, string, invalidPos);
}

s1_String* s1_string_create_u16 (s1_Library* obj, const s1_char16* string, const s1_char16** invalidPos)
{
  return s1_string_create_internal (obj, string, invalidPos);
}

s1_String* s1_string_create_u32 (s1_Library* obj, const s1_char32* string, const s1_char32** invalidPos)
{
  return s1_string_create_internal (obj, string, invalidPos);
}

s1_String* s1_string_create_wcs (s1_Library* obj, const wchar_t* string, const wchar_t** invalidPos)
{
  return s1_string_create_internal (obj, string, invalidPos);
}
