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
 * String object
 */
#ifndef __S1_STRING_H__
#define __S1_STRING_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"
#include "s1/Ptr.h"
#include "s1/ResultCode.h"

#define S1TYPE_INFO_s1_String   (S1_TYPE_MAKE_NAME(String), S1TYPE_INFO_s1_LibraryObject)
/**
 * String object.
 * Contains Unicode strings and provides access to the string data in various
 * encodings.
 * Used by the API to both return string data and accept string data fromn the
 * user.
 * \remarks A String object may or may not be attached to a library; thus querying
 * the library (via s1_libraryobject_get_library) may legitimately return \NULL.
 * \createdby s1_create_string(), s1_create_string_independent()
 * \extends s1_Object
 */
S1TYPE_DECLARE(S1TYPE_INFO_s1_String);

//@{
/**
 * Create an independent string object.
 * \param newStrObj Pointer receiving the new string object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * \param string Contents of the string. Passing \NULL will result in an error.
 * \param invalidPos Optionally returns position past the (first) invalid input.
 * \returns If successful, the return code fulfills #S1_SUCCESSFUL.
 * In case of a decoding error a valid String object will be returned; however,
 * place holder characters will be present in the string. \a invalidPos will point
 * to the position of the (first) invalid input and an appropriate error
 * status is returned.
 * In case of other errors, \a newStrObj is set to \NULL is returned and the error
 * status is returned.
 */
S1_API(s1_ResultCode) s1_string_independent_create_u8 (s1_String** newStrObj, const char* string,
                                                       const char** invalidPos S1_ARG_DEFAULT (S1_NULL));
S1_API(s1_ResultCode) s1_string_independent_create_wcs (s1_String** newStrObj, const wchar_t* string,
                                                        const wchar_t** invalidPos S1_ARG_DEFAULT (S1_NULL));
S1_API(s1_ResultCode) s1_string_independent_create_u16 (s1_String** newStrObj, const s1_char16* string,
                                                        const s1_char16** invalidPos S1_ARG_DEFAULT (S1_NULL));
S1_API(s1_ResultCode) s1_string_independent_create_u32 (s1_String** newStrObj, const s1_char32* string,
                                                        const s1_char32** invalidPos S1_ARG_DEFAULT (S1_NULL));
//@}

/**
 * Get contents of a string object in UTF-8.
 * \param string String object to request contents of.
 * \returns Contents of the string object in UTF-8.
 * In case of an error, \NULL is returned.
 * \memberof s1_String
 */
S1_API(const char*) s1_string_u8 (s1_String* string);
/**
 * Get contents of a string object as a wide string.
 * \param string String object to request contents of.
 * \returns Contents of the string object as a wide string.
 * In case of an error, \NULL is returned.
 * \memberof s1_String
 */
S1_API(const wchar_t*) s1_string_wcs (s1_String* string);
/**
 * Get contents of a string object in UTF-16.
 * \param string String object to request contents of.
 * \returns Contents of the string object in UTF-16.
 * In case of an error, \NULL is returned.
 * \memberof s1_String
 */
S1_API(const s1_char16*) s1_string_u16 (s1_String* string);
/**
 * Get contents of a string object in UTF-32.
 * \param string String object to request contents of.
 * \returns Contents of the string object in UTF-32.
 * In case of an error, \NULL is returned.
 * \memberof s1_String
 */
S1_API(const s1_char32*) s1_string_u32 (s1_String* string);

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    /**
     * String object.
     * Contains Unicode strings and provides access to the string data in various
     * encodings.
     * Used by the API to both return string data and accept string data fromn the
     * user.
     * \remarks A String object may or may not be attached to a library; thus querying
     * the library (via s1::LibraryObject::GetLibrary) may legitimately return \NULL.
     * \createdby s1::Library::CreateString(), s1::String::IndependentCreate
     */
    class String : public LibraryObject
    {
    public:
      /// Smart pointer class for Library instances.
      typedef Ptr<String> Pointer;

      //@{
      /**
       * Create an independent string object.
       * \param strObj Pointer receiving the new string object.
       * \param string Contents of the string. Passing \NULL will result in an error.
       * \returns If successful, the return code fulfills #S1_SUCCESSFUL.
       * In case of a decoding error a valid String object will be returned; however,
       * place holder characters will be present in the string and an appropriate error
       * status is returned.
       * In case of other errors, \a newStrObj is set to \NULL is returned and the error
       * status is returned.
       */
      static ResultCode IndependentCreate (Pointer& strObj, const char* string)
      {
        s1_String* p (0);
        ResultCode err = static_cast<ResultCode> (s1_string_independent_create_u8 (&p, string));
        strObj.take (p);
        return err;
      }
      static ResultCode IndependentCreate (Pointer& strObj, const wchar_t* string)
      {
        s1_String* p (0);
        ResultCode err = static_cast<ResultCode> (s1_string_independent_create_wcs (&p, string));
        strObj.take (p);
        return err;
      }
      static ResultCode IndependentCreate (Pointer& strObj, const s1_char16* string)
      {
        s1_String* p (0);
        ResultCode err = static_cast<ResultCode> (s1_string_independent_create_u16 (&p, string));
        strObj.take (p);
        return err;
      }
      static ResultCode IndependentCreate (Pointer& strObj, const s1_char32* string)
      {
        s1_String* p (0);
        ResultCode err = static_cast<ResultCode> (s1_string_independent_create_u32 (&p, string));
        strObj.take (p);
        return err;
      }
      //@}

      //@{
      /**
       * Create an independent string object.
       * \param strObj Pointer receiving the new string object.
       * \param string Contents of the string. Passing \NULL will result in an error.
       * \param invalidPos Returns position past the (first) invalid input, if any.
       * \returns If successful, the return code fulfills #S1_SUCCESSFUL.
       * In case of a decoding error a valid String object will be returned; however,
       * place holder characters will be present in the string. \a invalidPos will point
       * to the position of the (first) invalid input and an appropriate error
       * status is returned.
       * In case of other errors, \a newStrObj is set to \NULL is returned and the error
       * status is returned.
       */
      static ResultCode IndependentCreate (Pointer& strObj, const char* string, const char*& invalidPos)
      {
        s1_String* p (0);
        ResultCode err = static_cast<ResultCode> (s1_string_independent_create_u8 (&p, string, &invalidPos));
        strObj.take (p);
        return err;
      }
      static ResultCode IndependentCreate (Pointer& strObj, const wchar_t* string, const wchar_t*& invalidPos)
      {
        s1_String* p (0);
        ResultCode err = static_cast<ResultCode> (s1_string_independent_create_wcs (&p, string, &invalidPos));
        strObj.take (p);
        return err;
      }
      static ResultCode IndependentCreate (Pointer& strObj, const s1_char16* string, const s1_char16*& invalidPos)
      {
        s1_String* p (0);
        ResultCode err = static_cast<ResultCode> (s1_string_independent_create_u16 (&p, string, &invalidPos));
        strObj.take (p);
        return err;
      }
      static ResultCode IndependentCreate (Pointer& strObj, const s1_char32* string, const s1_char32*& invalidPos)
      {
        s1_String* p (0);
        ResultCode err = static_cast<ResultCode> (s1_string_independent_create_u32 (&p, string, &invalidPos));
        strObj.take (p);
        return err;
      }
      //@}

      /**
       * Get contents of the string object in UTF-8.
       * \returns Contents of the string object in UTF-8.
       * In case of an error, \NULL is returned.
       */
      const char* U8 ()
      {
        return s1_string_u8 (this);
      }
      /**
      * Get contents of the string object as a wide string.
      * \returns Contents of the string object as a wide string.
      * In case of an error, \NULL is returned.
      */
      const wchar_t* WCS ()
      {
        return s1_string_wcs (this);
      }
      /**
      * Get contents of the string object in UTF-16.
      * \returns Contents of the string object in UTF-16.
      * In case of an error, \NULL is returned.
      */
      const s1_char16* U16 ()
      {
        return s1_string_u16 (this);
      }
      /**
       * Get contents of the string object in UTF-32.
       * \returns Contents of the string object in UTF-32.
       * In case of an error, \NULL is returned.
       */
      const s1_char32* U32 ()
      {
        return s1_string_u32 (this);
      }
    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::String String;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_STRING_H__
