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

#include "StringArg.h"

s1_bool s1_backendoptions_set_from_str (s1_BackendOptions* options, s1_StringArg str)
{
  S1_ASSERT_MSG(options, "NULL BackendOptions", false);
  auto backend_options_impl (s1::EvilUpcast<s1::Compiler::Backend::Options> (options));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (backend_options_impl->GetDebugMessageHandler ());

  return backend_options_impl->Return (backend_options_impl->Try (
    [=]() {
      bool result = backend_options_impl->SetFromStr (s1::api_impl::ResolveStringArg (str, 0));
      return s1::Result<s1_bool> (result, result ? S1_SUCCESS : S1_E_FAILURE);
    }), false);
}
