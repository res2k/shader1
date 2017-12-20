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

/**\file
 * S1 program object
 */
#ifndef __S1_PROGRAM_H__
#define __S1_PROGRAM_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"
#include "s1/StringArg.h"

#define S1TYPE_INFO_s1_Program   (S1_TYPE_MAKE_NAME(Program), S1TYPE_INFO_s1_LibraryObject)
/**
 * Intermediate program object, provides program information
 * (e.g. program inputs and outputs) and stores per-program settings (e.g.
 * parameter characteristics).
 * \createdby s1_program_create_from_string()
 * \extends s1_LibraryObject
 */
S1TYPE_DECLARE(S1TYPE_INFO_s1_Program);

S1TYPE_DECLARE_FWD(Options);

/**
 * Set program compilation options.
 * \param program Program object.
 * \param options Compilation options object.
 * \returns Whether setting the options succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 * \remarks - The options object can be modified without affecting the
 * programs it was assigned to.
 * Conversely, to change the compilation options of a program, you have
 * to set the options again.
 * - Changing the options will affect the reported available
 * input variables -- \ref input_vars_volatility.
 * \memberof s1_Program
 */
S1_API(s1_bool) s1_program_set_options (s1_Program* program, s1_Options* options);
/**
 * Get program compilation options.
 * \param program Program object.
 * \returns The program compilation options.
 * Does \em not add a reference to the returned object.
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 * \remarks You will \em not get back the object passed to s1_program_set_options().
 * \memberof s1_Program
 */
S1_API(s1_Options*) s1_program_get_options (s1_Program* program);

S1TYPE_DECLARE_FWD(ProgramDiagnostics);

/**
 * Get program diagnostics.
 * \param program Program object.
 * \returns The program diagnostics.
 * Does \em not add a reference to the returned object.
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 * \memberof s1_Program
 */
S1_API(s1_ProgramDiagnostics*) s1_program_get_diagnostics (s1_Program* program);

S1TYPE_DECLARE_FWD(String);

/**
 * Set the entry function of a program.
 * \param program Program object.
 * \param name Entry function name.
 * \returns Whether changing the entry function succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 * \remarks Changing the entry function will affect the reported available
 * input variables -- \ref input_vars_volatility.
 * \memberof s1_Program
 */
S1_API(s1_bool) s1_program_set_entry_function (s1_Program* program, s1_StringArg name);
/**
 * Get the entry function of a program.
 * \param program Program object.
 * \returns The program's entry function or \NULL in case if an error.
 * In that case the error status is saved in the library's
 * last error code.
 * Does \em not add a reference to the returned object.
 * \remarks The default entry function name is \c main.
 *   The returned pointer is valid as long as the program object is valid and the entry
 *   function name wasn't changed.
 * \memberof s1_Program
 */
S1_API(s1_String*) s1_program_get_entry_function (s1_Program* program);

/// Variation frequency of a program input variable
enum s1_InputFrequency
{
  /// "Invalid" frequency, used to return error states.
  S1_FREQ_INVALID = -1,
  /// Variable only changes between program invocations (uniform variable)
  S1_FREQ_UNIFORM = 0,
  /// Variable changes per vertex (i.e. vertex attribute)
  S1_FREQ_VERTEX = 1
};

/**
 * Set the variation frequency of a program input variable.
 * \param program Program object.
 * \param param Input parameter name.
 * \param freq Variation frequency.
 * \returns Whether changing the variation frequency succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 * \remarks - Input variables are global variables and arguments to the
 *  entry function.
 *  - Changing an input variation frequency will affect the reported available
 *  input variables -- \ref input_vars_volatility.
 * \memberof s1_Program
 */
S1_API(s1_bool) s1_program_set_input_frequency (s1_Program* program, s1_StringArg param, enum s1_InputFrequency freq);
/**
 * Query the variation frequency of a program input variable.
 * \param program Program object.
 * \param param Input parameter name.
 * \returns The variation frequency or S1_FREQ_INVALID in case of an error.
 * In that case the error status is saved in the library's
 * last error code.
 * \remarks If a variation frequency wasn't explicitly set for the
 *   given parameter a default frequency will be returned.
 * \memberof s1_Program
 */
S1_API(enum s1_InputFrequency) s1_program_get_input_frequency (s1_Program* program, s1_StringArg param);

/* TODO: Should probably distinguish between an upper bound size (actual runtime size conveyed through
 * a distinct input uniform in the compiled program) and a fixed array size (actual runtime size is given). */
/**
 * Set the size of an array program input variable.
 * \param program Program object.
 * \param param Array input parameter name.
 * \param size Size of array input variable.
 * \returns Whether changing the array size succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 * \remarks - Array input variables have a default size of 0.
 *  - Changing an input array size will affect the reported available
 *  input variables -- \ref input_vars_volatility.
 * \memberof s1_Program
 */
S1_API(s1_bool) s1_program_set_input_array_size (s1_Program* program, s1_StringArg param, size_t size);
/**
 * Get the size of an array program input variable.
 * \param program Program object.
 * \param param Array input parameter name.
 * \returns The input variable array size or <tt>(size_t)~0</tt> in case of an error.
 * In that case the error status is saved in the library's
 * last error code.
 * \memberof s1_Program
 */
S1_API(size_t) s1_program_get_input_array_size (s1_Program* program, s1_StringArg param);

//TODO: Lots of inspection/query functions

/**\page input_vars_volatility Volatility of input variable properties
 * Various program settings affect which input variables are actually used by
 * the program -- as a typical example, due to optimizations a variable may end
 * up being unused.
 * 
 * Keep this is mind when querying available input variables and their
 * properties -- though the set of variables will stay the same, their
 * disposition may change (e.g. suddenly become unused) if you change
 * program settings between queries.
 */

#if defined(__cplusplus)
namespace s1
{
  typedef s1_InputFrequency InputFrequency;

  S1_NS_CXXAPI_BEGIN
    /**
     * Intermediate program object, provides program information
     * (e.g. program inputs and outputs) and stores per-program settings (e.g.
     * parameter characteristics).
     * \createdby s1::Library::CreateProgramFromString()
     */
    class Program : public LibraryObject
    {
    public:
      /// Smart pointer class for Program instances.
      typedef Ptr<Program> Pointer;

      /**
       * Set program compilation options.
       * \param options Compilation options object.
       * \returns Whether setting the options succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       * \remarks - The options object can be modified without affecting the
       * programs it was assigned to.
       * Conversely, to change the compilation options of a program, you have
       * to set the options again.
       * - Changing the options will affect the reported available
       * input variables -- \ref input_vars_volatility.
       */
      bool SetOptions (Options* options)
      {
        return s1_program_set_options (this, options) != 0;
      }
      /**
       * Get program compilation options.
       * \returns The program compilation options.
       * In case of an error, \nullptr is returned and the error status is saved in the library's
       * last error code.
       * \remarks You will \em not get back the object passed to s1_program_set_options().
       */
      Options* GetOptions ()
      {
        return s1_program_get_options (this);
      }

      /**
       * Get program diagnostics.
       * \returns The program diagnostics.
       * In case of an error, \nullptr is returned and the error status is saved in the library's
       * last error code.
       */
      ProgramDiagnostics* GetDiagnostics ()
      {
        return s1_program_get_diagnostics (this);
      }

      /**
       * Set the entry function of a program.
       * \param name Entry function name.
       * \returns Whether changing the entry function succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       * \remarks Changing the entry function will affect the reported available
       * input variables -- \ref input_vars_volatility.
       */
      bool SetEntryFunction (StringArg name)
      {
        return s1_program_set_entry_function (this, name) != 0;
      }

      /**
       * Get the entry function of a program.
       * \returns The program's entry function or \nullptr in case if an error.
       * Does \em not add a reference to the returned object.
       * In that case the error status is saved in the library's
       * last error code.
       * \remarks The default entry function name is \c main.
       */
      String* GetEntryFunction ()
      {
        return s1_program_get_entry_function (this);
      }

      /**
       * Set the variation frequency of a program input variable.
       * \param param Input parameter name.
       * \param freq Variation frequency.
       * \returns Whether changing the variation frequency succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       * \remarks - Input variables are global variables and arguments to the
       *  entry function.
       *  - Changing an input variation frequency will affect the reported available
       *  input variables -- \ref input_vars_volatility.
       */
      bool SetInputFrequency (StringArg param, InputFrequency freq)
      {
        return s1_program_set_input_frequency (this, param, freq) != 0;
      }

      /**
       * Query the variation frequency of a program input variable.
       * \param param Input parameter name.
       * \returns The variation frequency or S1_FREQ_INVALID in case of an error.
       * In that case the error status is saved in the library's
       * last error code.
       * \remarks If a variation frequency wasn't explicitly set for the
       *   given parameter a default frequency will be returned.
       */
      InputFrequency GetInputFrequency (StringArg param)
      {
        return s1_program_get_input_frequency (this, param);
      }

      /**
       * Set the size of an array program input variable.
       * \param param Array input parameter name.
       * \param size Size of array input variable.
       * \returns Whether changing the array size succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       * \remarks - Array input variables have a default size of 0.
       * - Changing an input array size will affect the reported available
       * input variables -- \ref input_vars_volatility.
       */
      bool SetInputArraySize (StringArg param, size_t size)
      {
        return s1_program_set_input_array_size (this, param, size) != 0;
      }
      /**
       * Get the size of an array program input variable.
       * \param param Array input parameter name.
       * \returns The input variable array size or <tt>(size_t)~0</tt> in case of an error.
       * In that case the error status is saved in the library's
       * last error code.
       */
      size_t GetInputArraySize (StringArg param)
      {
        return s1_program_get_input_array_size (this, param);
      }

    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::Program Program;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_PROGRAM_H__
