/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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
 * S1 compiler backend
 */
#ifndef __S1_BACKEND_H__
#define __S1_BACKEND_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"
#include "s1/Program.h"

#define S1TYPE_INFO_s1_Backend   (S1_TYPE_MAKE_NAME(Backend), S1TYPE_INFO_s1_LibraryObject)
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
  S1_TARGET_FP = 1,
  /**
   * Generate a program from the input before splitting.
   * \remarks Generally, &ldquo;unsplit&rdquo; programs generated by a backend
   *   are not actually useable. However, this type of output might be useful
   *   for purposes of debugging and troubleshooting.
   */
  S1_TARGET_UNSPLIT = 0x100
};

S1TYPE_DECLARE_FWD(BackendOptions);
S1TYPE_DECLARE_FWD(CompiledProgram);
/**
 * Let a backend generate an actual shader code from a Shader1 program.
 * \param backend Backend that should generate the program.
 * \param program Program to generate code from.
 * \param target Targeted shader stage.
 * \param options Optional backend options.
 * \returns A new compiled shader program or \NULL in case of an error.
 * The returned object will already have a reference, release the reference
 * using s1_release().
 * In that case the error status is saved in the library's
 * last error code.
 * \memberof s1_Backend
 */
S1_API(s1_CompiledProgram*) s1_backend_generate_program (s1_Backend* backend,
                                                         s1_Program* program,
                                                         enum s1_CompileTarget target,
                                                         s1_BackendOptions* options S1_ARG_DEFAULT(0));

/**
* Create a backend options object.
* \param backend Backend this options object will be used with.
* \returns A new backend options object.
*   The returned object will already have a reference, release the reference
*   using s1_release().
* In case of an error, \NULL is returned and the error status is saved in the library's
* last error code.
* \memberof s1_Backend
*/
S1_API(s1_BackendOptions*) s1_backendoptions_create (s1_Backend* backend);

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
    class Backend : public LibraryObject
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
      S1_RETURN_MOVE_REF_TYPE(CompiledProgram) GenerateProgram (Program* program, s1_CompileTarget target,
                                                                    BackendOptions* options = 0)
      {
        return S1_RETURN_MOVE_REF (CompiledProgram,
          s1_backend_generate_program (this, program, target, options));
      }

      /**
       * Create a backend options object.
       * \returns A new backend options object.
       * In case of an error, \NULL is returned and the error status is saved in the library's
       * last error code.
       */
      S1_RETURN_MOVE_REF_TYPE(BackendOptions) CreateBackendOptions ()
      {
        return S1_RETURN_MOVE_REF (BackendOptions,
                                       s1_backendoptions_create (this));
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
