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
 * S1 compiled program object
 */
#ifndef __S1_COMPILEDPROGRAM_H__
#define __S1_COMPILEDPROGRAM_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"

#define S1TYPE_INFO_s1_CompiledProgram   (S1_TYPE_MAKE_NAME(CompiledProgram), S1TYPE_INFO_s1_LibraryObject)
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
S1_API(const char*) s1_compiledprogram_get_string (s1_CompiledProgram* program);

S1TYPE_DECLARE_FWD(ByteStream);
/**
 * Obtain the string of a generated program.
 * \param program The compile program object.
 * \returns The program string as a byte stream.
 *   If obtaining the string failed, \NULL is returned, and the error status
 *   is saved in the library's last error code.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * \memberof s1_CompiledProgram
 */
S1_API(s1_ByteStream*) s1_compiledprogram_create_stream (s1_CompiledProgram* program);

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
    class CompiledProgram : public LibraryObject
    {
    public:
      /// Smart pointer class for CompiledProgram instances.
      typedef Ptr<CompiledProgram> Pointer;

      /**
       * Obtain the string of a generated program.
       * \returns The program string.
       *   If obtaining the string failed, \nullptr is returned, and the error status
       *   is saved in the library's last error code.
       * \remarks The returned string is valid as long as the compiled
       *  program object exists.
       */
      const char* GetString ()
      {
        return s1_compiledprogram_get_string (this);
      }

      /**
       * Obtain the string of a generated program.
       * \returns The program string as a byte stream.
       *   If obtaining the string failed, \nullptr is returned, and the error status
       *   is saved in the library's last error code.
       */
      S1_RETURN_MOVE_REF_TYPE(ByteStream) CreateStream ()
      {
        return S1_RETURN_MOVE_REF(ByteStream,
                                  s1_compiledprogram_create_stream (this));
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
