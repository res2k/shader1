/**\file
 * S1 program object
 */
#ifndef __S1_PROGRAM_H__
#define __S1_PROGRAM_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"

#define S1TYPE_INFO_s1_Program   (s1_Program, S1TYPE_INFO_s1_LibraryObject)
S1TYPE_DECLARE(S1TYPE_INFO_s1_Program);

S1TYPE_DECLARE_FWD(s1_Options);

/**
 * Set program compilation options.
 * \param program Program object.
 * \param options Compilation options object.
 * \returns Whether setting the options succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 */
S1_API s1_bool s1_program_set_options (s1_Program* program, s1_Options* options);
/**
 * Get program compilation options.
 * \param program Program object.
 * \returns The program compilation options.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 * \remarks You will \em not get back the object passed to s1_program_set_options().
 */
S1_API s1_Options* s1_program_get_options (s1_Program* program);

/**
 * Set the entry function of a program.
 * \param program Program object.
 * \param name Entry function name.
 * \returns Whether changing the entry function succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 */
S1_API s1_bool s1_program_set_entry_function (s1_Program* program, const char* name);
/**
 * Get the entry function of a program.
 * \param program Program object.
 * \returns The program's entry function or \NULL in case if an error.
 * In that case the error status is saved in the library's
 * last error code.
 * \remarks The default entry function name is \c main.
 *   The returned pointer is valid as long as the program object is valid and the entry
 *   function name wasn't changed.
 */
S1_API const char* s1_program_get_entry_function (s1_Program* program);

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
 * \remarks Input variables are global variables and arguments to the
 *  entry function.
 */
S1_API s1_bool s1_program_set_input_frequency (s1_Program* program, const char* param, s1_InputFrequency freq);
/**
 * Query the variation frequency of a program input variable.
 * \param program Program object.
 * \param param Input parameter name.
 * \returns The variation frequency or S1_FREQ_INVALID in case of an error.
 * In that case the error status is saved in the library's
 * last error code.
 * \remarks If a variation frequency wasn't explicitly set for the
 *   given parameter a default frequency will be returned.
 */
S1_API s1_InputFrequency s1_program_get_input_frequency (s1_Program* program, const char* param);

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
 * \remarks Array input variables have a default size of 0.
 */
S1_API s1_bool s1_program_set_input_array_size (s1_Program* program, const char* param, size_t size);
/**
 * Get the size of an array program input variable.
 * \param program Program object.
 * \param param Array input parameter name.
 * \returns The input variable array size or <tt>(size_t)~0</tt> in case of an error.
 * In that case the error status is saved in the library's
 * last error code.
 */
S1_API size_t s1_program_get_input_array_size (s1_Program* program, const char* param);

//TODO: Lots of inspection/query functions

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    class Program : public S1_REBADGE(Program, s1_Program, LibraryObject)
    {
    public:
      typedef Ptr<Program> Pointer;

      /**
       * Set program compilation options.
       * \param options Compilation options object.
       * \returns Whether setting the options succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       */
      bool SetOptions (const CPtr<s1_Options, detail::Uncounted>& options)
      {
        return s1_program_set_options (Cpointer(), options);
      }
      /**
       * Get program compilation options.
       * \returns The program compilation options.
       *   The returned object will already have a reference, release the reference
       *   using s1_release().
       * In case of an error, \NULL is returned and the error status is saved in the library's
       * last error code.
       * \remarks You will \em not get back the object passed to s1_program_set_options().
       */
      CPtr<s1_Options> GetOptions ()
      {
        return CPtr<s1_Options> (s1_program_get_options (Cpointer()), CPtr<s1_Options>::TakeReference ());
      }

      /**
       * Set the entry function of a program.
       * \param name Entry function name.
       * \returns Whether changing the entry function succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       */
      bool SetEntryFunction (const char* name)
      {
        return s1_program_set_entry_function (Cpointer(), name);
      }

      /**
       * Get the entry function of a program.
       * \returns The program's entry function or \NULL in case if an error.
       * The pointer is valid as long as the program object is valid and the entry
       * function name wasn't changed.
       * In that case the error status is saved in the library's
       * last error code.
       * \remarks The default entry function name is \c main.
       */
      bool GetEntryFunction ()
      {
        return s1_program_get_entry_function (Cpointer());
      }

      /**
       * Set the variation frequency of a program input variable.
       * \param param Input parameter name.
       * \param freq Variation frequency.
       * \returns Whether changing the variation frequency succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       * \remarks Input variables are global variables and arguments to the
       *  entry function.
       */
      bool SetInputFrequency (const char* param, s1_InputFrequency freq)
      {
        return s1_program_set_input_frequency (Cpointer(), param, freq);
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
      s1_InputFrequency GetInputFrequency (const char* param)
      {
        return s1_program_get_input_frequency (Cpointer(), param);
      }

      /**
       * Set the size of an array program input variable.
       * \param param Array input parameter name.
       * \param size Size of array input variable.
       * \returns Whether changing the array size succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       * \remarks Array input variables have a default size of 0.
       */
      bool SetInputArraySize (const char* param, size_t size)
      {
        return s1_program_set_input_array_size (Cpointer(), param, size);
      }
      /**
       * Get the size of an array program input variable.
       * \param param Array input parameter name.
       * \returns The input variable array size or <tt>(size_t)~0</tt> in case of an error.
       * In that case the error status is saved in the library's
       * last error code.
       */
      size_t GetInputArraySize (const char* param)
      {
        return s1_program_get_input_array_size (Cpointer(), param);
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
