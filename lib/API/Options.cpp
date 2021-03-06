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
#include "s1/ResultCode_defs_compileroptions.h"

#include "s1/Options.h"

#include "base/uc/String_optional.h"
#include "compiler/Options.h"

#include "StringArg.h"

s1_bool s1_options_set_opt_flag (s1_Options* options, s1_Optimization opt, s1_bool enable)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (options_impl->GetDebugMessageHandler ());
  if (int (opt) >= int (s1::Compiler::Options::numOptimizations))
  {
    return options_impl->ReturnErrorCode (S1_E_INVALID_OPTIMIZATION);
  }
  options_impl->SetOptimizationFlag ((s1::Compiler::Options::Optimization)opt, enable != 0);
  return options_impl->ReturnSuccess();
}

s1_bool s1_options_get_opt_flag (s1_Options* options, s1_Optimization opt)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (options_impl->GetDebugMessageHandler ());
  if (int (opt) >= int (s1::Compiler::Options::numOptimizations))
  {
    return options_impl->ReturnErrorCode (S1_E_INVALID_OPTIMIZATION);
  }
  bool enabled = options_impl->GetOptimizationFlag ((s1::Compiler::Options::Optimization)opt);
  return options_impl->ReturnSuccess (enabled);
}

s1_bool s1_options_set_opt_level (s1_Options* options, int level)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (options_impl->GetDebugMessageHandler ());
  if (level < 0)
  {
    return options_impl->ReturnErrorCode (S1_E_INVALID_ARG_N(0));
  }
  options_impl->SetOptimizationLevel (level);
  return options_impl->ReturnSuccess();
}
    
s1_bool s1_options_parse_opt_flag_str (s1_Options* options, s1_StringArg flagStr,
                                       s1_Optimization* opt, s1_bool* flag)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (options_impl->GetDebugMessageHandler ());
  if (!opt)
  {
    return options_impl->ReturnErrorCode (S1_E_INVALID_ARG_N(1));
  }
  if (!flag)
  {
    return options_impl->ReturnErrorCode (S1_E_INVALID_ARG_N(2));
  }

  return options_impl->Return (options_impl->Try (
    [=]() -> s1::Result<bool> {
      s1::Compiler::Options::Optimization parsed_opt;
      bool parsed_flag;
      if (!options_impl->ParseOptimizationFlagString (s1::api_impl::ResolveStringArg (flagStr, 0),
                                                      parsed_opt, parsed_flag))
      {
        return S1_E_INVALID_OPTIMIZATION;
      }
      *opt = (s1_Optimization)parsed_opt;
      *flag = parsed_flag;
      return true;
    }), false);
}

s1_bool s1_options_set_opt_flag_from_str (s1_Options* options, s1_StringArg flagStr)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (options_impl->GetDebugMessageHandler ());

  return options_impl->Return (options_impl->Try (
    [=]() -> s1::Result<bool> {
      if (!options_impl->SetOptimizationFlagFromStr (s1::api_impl::ResolveStringArg (flagStr, 0)))
      {
        return S1_E_INVALID_OPTIMIZATION;
      }
      return true;
    }), false);
}
