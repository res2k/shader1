/**\file
 * S1 compiler backend
 */
#ifndef __S1_BACKEND_H__
#define __S1_BACKEND_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"
#include "s1/Program.h"

#define S1TYPE_INFO_s1_Backend   (s1_Backend, S1TYPE_INFO_s1_LibraryObject)
/**
 * Compiler backend.
 * Used to compile a program object representing actual shader code.
 * \extends s1_LibraryObject
 * \sa s1_CompiledProgram
 * \createdby s1_backend_create()
 */
S1TYPE_DECLARE(S1TYPE_INFO_s1_Backend);

/// Target for backend program compilation
enum s1_CompileTarget
{
  /// Generate vertex program
  S1_TARGET_VP = 0,
  /// Generate fragment program
  S1_TARGET_FP = 1
};

S1TYPE_DECLARE_FWD(s1_CompiledProgram);
/**
 * Let a backend generate an actual shader code from a Shader1 program.
 * \param backend Backend that should generate the program.
 * \param program Program to generate code from.
 * \param target Targeted shader stage.
 * \returns A new compiled shader program or \NULL in case of an error.
 * The returned object will already have a reference, release the reference
 * using s1_release().
 * In that case the error status is saved in the library's
 * last error code.
 * \memberof s1_Backend
 */
S1_API s1_CompiledProgram* s1_backend_generate_program (s1_Backend* backend,
                                                        s1_Program* program,
                                                        enum s1_CompileTarget target);

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    /**
     * Compiler backend.
     * Used to compile a program object representing actual shader code.
     * \sa CompiledProgram
     * \createdby Library::CreateBackend()
     */
    class Backend : public S1_REBADGE(Backend, s1_Backend, LibraryObject)
    {
    public:
      /// Smart pointer class for Backend instances.
      typedef Ptr<Backend> Pointer;

      /**
       * Let a backend generate an actual shader code from a Shader1 program.
       * \param target Targeted shader stage.
       * \param program Program to generate code from.
       * \returns A new compiled shader program or \NULL in case of an error.
       * In that case the error status is saved in the library's
       * last error code.
       */
      CPtr<s1_CompiledProgram> GenerateProgram (const CPtr<s1_Program, ref_traits::Uncounted>& program,
                                                s1_CompileTarget target)
      {
        return CPtr<s1_CompiledProgram> (s1_backend_generate_program (Cpointer(), program, target),
                                         CPtr<s1_CompiledProgram>::TakeReference ());
      }
    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::Backend Backend;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)


#include "s1/warn_on.h"

#endif // __S1_BACKEND_H__
