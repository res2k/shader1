/**\file
 * S1 compiler backend
 */
#ifndef __S1_BACKEND_H__
#define __S1_BACKEND_H__

#include "s1/warn_off.h"

#include "s1/Backend_type.h"
#include "s1/CompiledProgram_type.h"
#include "s1/Program.h"

/// Target for backend program compilation
enum s1_CompileTarget
{
  /// Generate vertex program
  S1_TARGET_VP = 0,
  /// Generate fragment program
  S1_TARGET_FP = 1
};

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
 */
S1_API s1_CompiledProgram* s1_backend_generate_program (s1_Backend* backend,
                                                        s1_Program* program,
                                                        s1_CompileTarget target);

#if defined(__cplusplus)
namespace s1
{
  namespace cxxapi
  {
    class Backend : public Rebadge<Backend, s1_Backend, LibraryObject>
    {
    public:
      typedef Ptr<Backend> Pointer;

      /**
       * Let a backend generate an actual shader code from a Shader1 program.
       * \param target Targeted shader stage.
       * \param program Program to generate code from.
       * \returns A new compiled shader program or \NULL in case of an error.
       * In that case the error status is saved in the library's
       * last error code.
       */
      CPtr<s1_CompiledProgram> GenerateProgram (const CPtr<s1_Program>& program,
                                                s1_CompileTarget target)
      {
        return CPtr<s1_CompiledProgram> (s1_backend_generate_program (Cpointer(), program, target),
                                         CPtr<s1_CompiledProgram>::TakeReference ());
      }
    };
  } // namespace cxxapi

#if !defined(S1_BUILD)
  typedef cxxapi::Backend Backend;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)


#include "s1/warn_on.h"

#endif // __S1_BACKEND_H__
