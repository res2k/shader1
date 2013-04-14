#include "base/common.h"

#include "s1/Options.h"

#include "compiler/Options.h"

s1_bool s1_options_set_opt_flag (s1_Options* options, s1_Optimization opt, s1_bool enable)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  if (int (opt) >= int (s1::Compiler::Options::numOptimizations))
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_OPTIMIZATION);
    return false;
  }
  options_impl->SetOptimizationFlag ((s1::Compiler::Options::Optimization)opt, enable);
  return true;
}

s1_bool s1_options_get_opt_flag (s1_Options* options, s1_Optimization opt, s1_bool* enabled)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  if (int (opt) >= int (s1::Compiler::Options::numOptimizations))
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_OPTIMIZATION);
    return false;
  }
  if (!enabled)
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_ARG_N(1));
    return false;
  }
  *enabled = options_impl->GetOptimizationFlag ((s1::Compiler::Options::Optimization)opt);
  return true;
}

s1_bool s1_options_set_opt_level (s1_Options* options, int level)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  if (level < 0)
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_ARG_N(0));
    return false;
  }
  options_impl->SetOptimizationLevel (level);
  return true;
}
    
s1_bool s1_options_parse_opt_flag_str (s1_Options* options, const char* flagStr,
                                       s1_Optimization* opt, s1_bool* flag)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  if (!flagStr)
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_ARG_N(0));
    return false;
  }
  if (!opt)
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_ARG_N(1));
    return false;
  }
  if (!flag)
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_ARG_N(2));
    return false;
  }
  s1::Compiler::Options::Optimization parsed_opt;
  bool parsed_flag;
  if (!options_impl->ParseOptimizationFlagString (flagStr, parsed_opt, parsed_flag))
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_OPTIMIZATION);
    return false;
  }
  *opt = (s1_Optimization)parsed_opt;
  *flag = parsed_flag;
  return true;
}

s1_bool s1_options_set_opt_flag_from_str (s1_Options* options, const char* flagStr)
{
  S1_ASSERT_MSG(options, "NULL Options", false);
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));
  if (!flagStr)
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_ARG_N(0));
    return false;
  }
  if (!options_impl->SetOptimizationFlagFromStr (flagStr))
  {
    options_impl->GetLibrary()->SetLastError (S1_E_INVALID_OPTIMIZATION);
    return false;
  }
  return true;
}
