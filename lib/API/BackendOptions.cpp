/*
    Shader1
    Copyright (c) 2015-2016 Frank Richter


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

#include "s1/BackendOptions.h"

#include "base/uc/String_optional.h"
#include "compiler/Backend.h"

static s1_bool s1_backendoptions_set_from_str_ucs (s1_BackendOptions* options, s1::uc::String_optional str)
{
  S1_ASSERT_MSG(options, "NULL BackendOptions", false);
  auto backend_options_impl (s1::EvilUpcast<s1::Compiler::Backend::Options> (options));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (backend_options_impl->GetDebugMessageHandler ());
  if (!str)
  {
    return backend_options_impl->ReturnErrorCode (S1_E_INVALID_ARG_N (0));
  }
  bool result = backend_options_impl->SetFromStr (*str);
  return backend_options_impl->ReturnErrorCode (result ? S1_SUCCESS : S1_E_FAILURE, result);
}

s1_bool s1_backendoptions_set_from_str (s1_BackendOptions* options, const char* str)
{
  return s1_backendoptions_set_from_str_ucs (options, s1::uc::make_String_optional (str));
}

s1_bool s1_backendoptions_set_from_str_ws (s1_BackendOptions* options, const wchar_t* str)
{
  return s1_backendoptions_set_from_str_ucs (options, s1::uc::make_String_optional (str));
}

s1_bool s1_backendoptions_set_from_str_u16 (s1_BackendOptions* options, const s1_char16* str)
{
  return s1_backendoptions_set_from_str_ucs (options, s1::uc::make_String_optional (str));
}

s1_bool s1_backendoptions_set_from_str_u32 (s1_BackendOptions* options, const s1_char32* str)
{
  return s1_backendoptions_set_from_str_ucs (options, s1::uc::make_String_optional (str));
}
