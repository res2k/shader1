/**\file
 * S1 compiler options
 */
#ifndef __S1_OPTIONS_H__
#define __S1_OPTIONS_H__

#include "s1/warn_off.h"

#include "s1/fwd/Options_type.h"

/// Optimization option constants
enum s1_Optimization
{
  /// Block inlining option
  S1_OPT_BLOCK_INLINING = 0,
  /// Dead code elimination option
  S1_OPT_DEAD_CODE_ELIMINATION = 1,
  /// Constant folding option
  S1_OPT_CONSTANT_FOLDING = 2
};

/**
 * Enable or disable an optimization option.
 * \param options Compiler options object.
 * \param opt Optimization option to enable or disable.
 * \param enable Whether to enable (\c true) or disable (\c false) the selected option.
 * \returns Whether changing the optimization flag succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 */
S1_API s1_bool s1_options_set_opt_flag (s1_Options* options, s1_Optimization opt, s1_bool enable);
/**
 * Query status of an optimization option.
 * \param options Compiler options object.
 * \param opt Optimization option to query.
 * \returns The optimization status.
 * Note: in case querying the optimization flag failed \c false is returned
 * as well. You can check whether an error occured or if the optimization is
 * actually disabled by inspecting the library's
 * last error code.
 */
S1_API s1_bool s1_options_get_opt_flag (s1_Options* options, s1_Optimization opt);

/**
 * Enable/disable all optimizations for some "level" of optimizations.
 * Currently two levels are supported: 0 -> no optimization, anything >0 -> full optimization.
 * \param options Compiler options object.
 * \param level Optimization level to set.
 * \returns Whether setting the optimization level succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 */
S1_API s1_bool s1_options_set_opt_level (s1_Options* options, int level);
    
/**
 * Parse an optimization flag from a string.
 * Useful for e.g. command line parsing.
 * \param options Compiler options object.
 * \param flagStr Optimization option string. Can be prefixed by <tt>no-</tt> in which
 *   case it's parsed as a "disable" option; otherwise, it's parsed as an "enable"
 *   option.
 * \param opt Receives parsed option.
 * \param flag Receives parsed option enable flag.
 * \returns Whether setting the optimization level succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 */
S1_API s1_bool s1_options_parse_opt_flag_str (s1_Options* options, const char* flagStr,
                                              s1_Optimization* opt, s1_bool* flag);
    
/**
 * Parse and set an optimization flag from a string.
 * Useful for e.g. command line parsing.
 * \param options Compiler options object.
 * \param flagStr Optimization option string. 
 *   Accepts the same strings as s1_options_parse_opt_flag_str().
 * \returns Whether setting the optimization level succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 */
S1_API s1_bool s1_options_set_opt_flag_from_str (s1_Options* options, const char* flagStr);


#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    class Options : public S1_REBADGE(Options, s1_Options, LibraryObject)
    {
    public:
      typedef Ptr<Options> Pointer;

      /**
       * Enable or disable an optimization option.
       * \param opt Optimization option to enable or disable.
       * \param enable Whether to enable (\c true) or disable (\c false) the selected option.
       * \returns Whether changing the optimization flag succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       */
      bool SetOptFlag (s1_Optimization opt, bool enable)
      {
        return s1_options_set_opt_flag (Cpointer(), opt, enable);
      }
      
      /**
       * Query status of an optimization option.
       * \param opt Optimization option to query.
       * \param enable Returns the optimization status.
       * \returns Whether querying the optimization flag succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       */
      bool GetOptFlag (s1_Optimization opt)
      {
        return s1_options_get_opt_flag (Cpointer(), opt);
      }

      /**
      * Enable/disable all optimizations for some "level" of optimizations.
      * Currently two levels are supported: 0 -> no optimization, anything >0 -> full optimization.
      * \param level Optimization level to set.
      * \returns Whether setting the optimization level succeeded.
      * In case of an error, the error status is saved in the library's
      * last error code.
      */
      bool SetOptLevel (int level)
      {
        return s1_options_set_opt_level (Cpointer(), level);
      }
          
      /**
       * Parse an optimization flag from a string.
       * Useful for e.g. command line parsing.
       * \param flagStr Optimization option string. Can be prefixed by <tt>no-</tt> in which
       *   case it's parsed as a "disable" option; otherwise, it's parsed as an "enable"
       *   option.
       * \param opt Receives parsed option.
       * \param flag Receives parsed option enable flag.
       * \returns Whether setting the optimization level succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       */
      bool ParseOptFlagStr (const char* flagStr, s1_Optimization& opt, bool& flag)
      {
        s1_bool ret_flag;
        bool ret (s1_options_parse_opt_flag_str (Cpointer(), flagStr, &opt, &ret_flag));
        if (!ret) return false;
        flag = ret_flag;
        return true;
      }
          
      /**
       * Parse and set an optimization flag from a string.
       * Useful for e.g. command line parsing.
       * \param flagStr Optimization option string. 
       *   Accepts the same strings as s1_options_parse_opt_flag_str().
       * \returns Whether setting the optimization level succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       */
      bool SetOptFlagFromStr (const char* flagStr)
      {
        return s1_options_set_opt_flag_from_str (Cpointer(), flagStr);
      }
    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::Options Options;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_OPTIONS_H__
