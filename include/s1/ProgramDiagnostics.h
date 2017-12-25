/*
    Shader1
    Copyright (c) 2017 Frank Richter


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
 * S1 program diagnostics information container object
 */
#ifndef S1_PROGRAMDIAGNOSTICS_H_
#define S1_PROGRAMDIAGNOSTICS_H_

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"
#include "s1/StringArg.h"

#define S1TYPE_INFO_s1_ProgramDiagnostics   (S1_TYPE_MAKE_NAME(ProgramDiagnostics), S1TYPE_INFO_s1_LibraryObject)
/**
 * Program diagnostics information container object.
 * Provides facilities to query program compilation diagnostics (ie errors and
 * warnings).
 * \extends s1_LibraryObject
 */
S1TYPE_DECLARE(S1TYPE_INFO_s1_ProgramDiagnostics);

/// Type of a diagnostic information
enum s1_DiagnosticClass
{
  /// Invalid class
  S1_DIAGNOSTIC_INVALID = -1,
  /// Compilation error
  S1_DIAGNOSTIC_ERROR = 0,
  /// Compilation warning
  S1_DIAGNOSTIC_WARNING
};

/**
 * Get number of individual diagnostic information in a container.
 * \param diagnostics Program diagnostics object.
 * \returns Number of individual diagnostic information in container.
 * In case of an error, the error status is saved in the library's
 * last error code, and the return value is 0.
 * \memberof s1_ProgramDiagnostics
 */
S1_API(size_t) s1_programdiagnostics_get_count (s1_ProgramDiagnostics* diagnostics);

/**
 * Get "class" for an individual diagnostic information.
 * \param diagnostics Program diagnostics object.
 * \param diagnosticIndex Index of individual diagnostic information.
 * \returns The "class" of the individual diagnostic information in container.
 * In case of an error, the error status is saved in the library's
 * last error code, and the return value is S1_DIAGNOSTIC_INVALID.
 * \memberof s1_ProgramDiagnostics
 */
S1_API(enum s1_DiagnosticClass) s1_programdiagnostics_get_class (
  s1_ProgramDiagnostics* diagnostics, size_t diagnosticIndex);

/**
 * Get an ID string for an individual diagnostic information.
 * \param diagnostics Program diagnostics object.
 * \param diagnosticIndex Index of individual diagnostic information.
 * \returns The ID string of the individual diagnostic information in container.
 * The string is encoded in ASCII; it's intended for programmatic identifaction
 * of an individual diagnostic information, but it's somewhat suitable for
 * human consumption as well.
 * In case of an error, the error status is saved in the library's
 * last error code, and the return value is \NULL.
 * \memberof s1_ProgramDiagnostics
 */
S1_API(const char*) s1_programdiagnostics_get_id (s1_ProgramDiagnostics* diagnostics,
  size_t diagnosticIndex);

/**
 * Obtain descriptive string for an individual diagnostic information.
 * \param diagnostics Program diagnostics object.
 * \param diagnosticIndex Index of individual diagnostic information.
 * \returns A new string object containing a description of the individual
 *   diagnostic information in container. The string is suitable for human
 *   consumption.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 *   In case of an error, the error status is saved in the library's
 *   last error code, and the return value is \NULL.
 * \memberof s1_ProgramDiagnostics
 */
S1_API(s1_String*) s1_programdiagnostics_create_description (s1_ProgramDiagnostics* diagnostics,
  size_t diagnosticIndex);

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    /**
     * Intermediate program object, provides program information
     * (e.g. program inputs and outputs) and stores per-program settings (e.g.
     * parameter characteristics).
     * \createdby s1::Library::CreateProgramFromString()
     */
    class ProgramDiagnostics : public LibraryObject
    {
    public:
      /// Smart pointer class for ProgramDiagnostics instances.
      typedef Ptr<ProgramDiagnostics> Pointer;

      /**
       * Get number of individual diagnostic information in a container.
       * \returns Number of individual diagnostic information in container.
       * In case of an error, the error status is saved in the library's
       * last error code, and the return value is 0.
       */
      size_t GetCount ()
      {
        return s1_programdiagnostics_get_count (this);
      }

      /**
       * Get "class" for an individual diagnostic information.
       * \param diagnosticIndex Index of individual diagnostic information.
       * \returns The "class" of the individual diagnostic information in container.
       * In case of an error, the error status is saved in the library's
       * last error code, and the return value is S1_DIAGNOSTIC_INVALID.
       */
      s1_DiagnosticClass GetClass (size_t diagnosticIndex)
      {
        return s1_programdiagnostics_get_class (this, diagnosticIndex);
      }

      /**
       * Get an ID string for an individual diagnostic information.
       * \param diagnosticIndex Index of individual diagnostic information.
       * \returns The ID string of the individual diagnostic information in container.
       * The string is encoded in ASCII; it's intended for programmatic identifaction
       * of an individual diagnostic information, but it's somewhat suitable for
       * human consumption as well.
       * In case of an error, the error status is saved in the library's
       * last error code, and the return value is \nullptr.
       */
      const char* GetID (size_t diagnosticIndex)
      {
        return s1_programdiagnostics_get_id (this, diagnosticIndex);
      }

      /**
       * Obtain descriptive string for an individual diagnostic information.
       * \param diagnosticIndex Index of individual diagnostic information.
       * \returns A new string object containing a description of the individual
       *   diagnostic information in container. The string is suitable for human
       *   consumption.
       *   In case of an error, the error status is saved in the library's
       *   last error code, and the return value is \nullptr.
       */
      S1_RETURN_MOVE_REF_TYPE(String) CreateDescription (size_t diagnosticIndex)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_programdiagnostics_create_description (this, diagnosticIndex));
      }
    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::ProgramDiagnostics ProgramDiagnostics;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // S1_PROGRAMDIAGNOSTICS_H_
