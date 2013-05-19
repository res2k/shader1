/**\file
 * S1 compiled program object
 */
#ifndef __S1_COMPILEDPROGRAM_H__
#define __S1_COMPILEDPROGRAM_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"

#define S1TYPE_INFO_s1_CompiledProgram   (s1_CompiledProgram, S1TYPE_INFO_s1_LibraryObject)
/**
 * Compiled program object.
 * Produced by a backend, encapsulates a program compiled to a specific target
 * shader language and shading stage.
 * \createdby s1_backend_generate_program().
 * \extends s1_LibraryObject
 */
S1TYPE_DECLARE(S1TYPE_INFO_s1_CompiledProgram);

/**
 * Obtain the string of a generated program.
 * \param program The compile program object.
 * \returns The program string.
 *   If obtaining the string failed, \NULL is returned, and the error status
 *   is saved in the library's last error code.
 * \remarks The returned string is valid as long as the compiled
 *  program object exists.
 * \memberof s1_CompiledProgram
 */
S1_API const char* s1_compiledprogram_get_string (s1_CompiledProgram* program);

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    /**
     * Compiled program object.
     * Produced by a backend, encapsulates a program compiled to a specific target
     * shader language and shading stage.
     * \createdby s1::Backend::GenerateProgram().
     */
    class CompiledProgram : public S1_REBADGE(CompiledProgram, s1_CompiledProgram, LibraryObject)
    {
    public:
      /// Smart pointer class for CompiledProgram instances.
      typedef Ptr<CompiledProgram> Pointer;

      /**
       * Obtain the string of a generated program.
       * \returns The program string.
       *   If obtaining the string failed, \NULL is returned, and the error status
       *   is saved in the library's last error code.
       * \remarks The returned string is valid as long as the compiled
       *  program object exists.
       */
      const char* GetString ()
      {
        return s1_compiledprogram_get_string (Cpointer());;
      }

    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::CompiledProgram CompiledProgram;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_COMPILEDPROGRAM_H__
