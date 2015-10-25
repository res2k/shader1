/*
    Shader1
    Copyright (c) 2015 Frank Richter


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
 * S1 backend code generation options
 */
#ifndef __S1_BACKENDOPTIONS_H__
#define __S1_BACKENDOPTIONS_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"

#define S1TYPE_INFO_s1_BackendOptions   (S1_TYPE_MAKE_NAME(BackendOptions), S1TYPE_INFO_s1_LibraryObject)
/**
 * Backend code generation options.
 * Encapsulates code generation settings likely to be used across a number of
 * programs.
 * \createdby s1_backendoptions_create()
 * \extends s1_LibraryObject
 */
S1TYPE_DECLARE(S1TYPE_INFO_s1_BackendOptions);
    
/**
 * Parse and set a backend option from a string.
 * Useful for e.g. command line parsing.
 * \param options Backend options object.
 * \param flagStr Backend option string. 
 * \returns Whether setting the optimization level succeeded.
 * In case of an error, the error status is saved in the library's
 * last error code.
 * \memberof s1_BackendOptions
 */
S1_API(s1_bool) s1_backendoptions_set_from_str (s1_BackendOptions* options, const char* str);


#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    /**
     * Backend code generation options.
     * Encapsulates code generation settings likely to be used across a number of
     * programs.
     * \createdby s1::Backend::CreateBackendOptions()
     */
    class BackendOptions : public LibraryObject
    {
    public:
      /// Smart pointer class for Options instances.
      typedef Ptr<BackendOptions> Pointer;

      /**
       * Parse and set a backend option from a string.
       * Useful for e.g. command line parsing.
       * \param options Backend options object.
       * \param flagStr Backend option string. 
       * \returns Whether setting the optimization level succeeded.
       * In case of an error, the error status is saved in the library's
       * last error code.
       */
      bool SetFromStr (const char* str)
      {
        return s1_backendoptions_set_from_str (this, str) != 0;
      }
    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::BackendOptions BackendOptions;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_BACKENDOPTIONS_H__
