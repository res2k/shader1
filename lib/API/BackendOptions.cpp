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

#include "base/common.h"

#include "s1/BackendOptions.h"

#include "compiler/Backend.h"

s1_bool s1_backendoptions_set_from_str (s1_BackendOptions* options, const char* str)
{
  S1_ASSERT_MSG(options, "NULL BackendOptions", false);
  auto backend_options_impl (s1::EvilUpcast<s1::Compiler::Backend::Options> (options));
  bool result = backend_options_impl->SetFromStr (str);
  return backend_options_impl->ReturnErrorCode (result ? S1_SUCCESS : S1_E_FAILURE, result);
}
