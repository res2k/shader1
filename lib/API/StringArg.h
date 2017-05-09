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
 * String argument resolution
 */
#ifndef __S1_CXXAPI_STRINGARG_H__
#define __S1_CXXAPI_STRINGARG_H__

#include "s1/StringArg.h"

#include "StringObj.h"

#include <boost/variant.hpp>

namespace s1
{
  namespace api_impl
  {
    /**
     * Calls an \c operator() of \a Visitor depending on the string passed in
     * \a arg.
     * \c operator() of \a Visitor is called with one of the following argument(s):
     * * <tt>const char*</tt>, <tt>const s1_char16*</tt>, <tt>const s1_char32*</tt>
     *   or <tt>const wchar_t*</tt> together with a \c size_t:
     *   Called if the string argument contains a pointer to an UTF-8, UTF-16,
     *   UTF-32 or wide character string. The \c size_t argument may specify the
     *   string length in code units, or may be \c (size_t)~0.
     * * <tt>cxxapi::String*</tt> if the argument contains a String object.
     * * \c ResultCode if the argument contains an error. The code is usually
     *   S1_E_STRING_TOO_LONG or S1_E_INVALID_ARG.
     * \a Visitor must provide a nested type definition \em result_type.
     * \returns The result of the called \c operator().
     */
    template<typename Visitor>
    typename Visitor::result_type VisitStringArg (s1::cxxapi::StringArg arg, Visitor&& v)
    {
      if (((arg.ts & _S1_SA_TS_SIZE_MASK) == _S1_SA_TS_SIZE_INVALID)
        && (arg.p == 0))
      {
        return v (S1_E_STRING_TOO_LONG);
      }
      else if ((arg.p != 0) && (arg.ts == UINTPTR_MAX))
      {
        return v (reinterpret_cast<cxxapi::String*> (arg.p));
      }
      else if (arg.p == 0)
      {
        return v (S1_E_INVALID_ARG);
      }
      else
      {
        uintptr_t type = arg.ts & _S1_SA_TS_TYPE_MASK;
        uintptr_t raw_size = (arg.ts & _S1_SA_TS_SIZE_MASK);
        size_t len = (raw_size == _S1_SA_TS_SIZE_AUTO) ? (size_t)~0 : (size_t)raw_size;
        switch (type)
        {
        case _S1_SA_TS_TYPE_UTF8:   return v (reinterpret_cast<const char*> (arg.p), len);
        case _S1_SA_TS_TYPE_UTF16:  return v (reinterpret_cast<const s1_char16*> (arg.p), len);
        case _S1_SA_TS_TYPE_UTF32:  return v (reinterpret_cast<const s1_char32*> (arg.p), len);
        case _S1_SA_TS_TYPE_WCS:    return v (reinterpret_cast<const wchar_t*> (arg.p), len);
        }
      }
      return v (S1_E_INVALID_ARG);
    }

    /**
     * Resolve a string argument to an internal type (or maybe error).
     */
    class ResolveStringArg
    {
      typedef boost::variant<ResultCode, uc::String, cxxapi::String*> string_data;

      class StringVisitor
      {
        int argNum;
      public:
        typedef string_data result_type;

        StringVisitor (int argNum) : argNum (argNum) {}

        string_data operator() (const char* s, size_t len) { return uc::String::fromUntrustedUTF (s, len); }
        string_data operator() (const s1_char16* s, size_t len) { return uc::String::fromUntrustedUTF (s, len); }
        string_data operator() (const s1_char32* s, size_t len) { return uc::String::fromUntrustedUTF (s, len); }
        string_data operator() (const wchar_t* s, size_t len) { return uc::String::fromUntrustedWS (s, len); }
        string_data operator() (cxxapi::String* s) { return s; }
        string_data operator() (ResultCode error)
        {
          switch (error)
          {
          case S1_E_STRING_TOO_LONG:  return S1_E_STRING_TOO_LONG_N(argNum);
          case S1_E_INVALID_ARG:      return S1_E_INVALID_ARG_N(argNum);
          }
          return error;
        }
      };

      string_data data;
    public:
      ResolveStringArg (s1::cxxapi::StringArg arg, int argNum)
      {
        data = VisitStringArg (arg, StringVisitor (argNum));
      }

      /// Return resolution error code, if any
      ResultCode GetError() const
      {
        if (data.which() == 0)
        {
          return *(boost::get<ResultCode> (&data));
        }
        else
        {
          return S1_SUCCESS;
        }
      }

      //@{
      /**
       * Get string value as an uc::String reference.
       * \remarks Throws.
       */
      const uc::String& GetString() const
      {
        struct visitor : public boost::static_visitor<const uc::String&>
        {
          const uc::String& operator()(ResultCode err)
          {
            throw s1::Exception (err);
          }
          const uc::String& operator()(const uc::String& s)
          {
            return s;
          }
          const uc::String& operator()(cxxapi::String* s)
          {
            return s1::EvilUpcast<s1::api_impl::String> (s)->StrUCS();
          }
        };
        visitor v;
        return boost::apply_visitor (v, data);
      }
      operator const uc::String& () const { return GetString(); }
      //@}

      //@{
      /**
       * Get string value as an optional reference to an uc::String.
       * \remarks Does not throw.
       */
      boost::optional<const uc::String&> GetOptionalStringRef(ResultCode* out_err = nullptr) const
      {
        struct visitor : public boost::static_visitor<boost::optional<const uc::String&>>
        {
          ResultCode* err_ptr;
          visitor (ResultCode* err_ptr) : err_ptr (err_ptr) {}

          boost::optional<const uc::String&> operator()(ResultCode err)
          {
            if (err_ptr) *err_ptr = err;
            return boost::none;
          }
          boost::optional<const uc::String&> operator()(const uc::String& s)
          {
            if (err_ptr) *err_ptr = S1_SUCCESS;
            return s;
          }
          boost::optional<const uc::String&> operator()(cxxapi::String* s)
          {
            if (err_ptr) *err_ptr = S1_SUCCESS;
            return s1::EvilUpcast<s1::api_impl::String> (s)->StrUCS();
          }
        };
        visitor v (out_err);
        return boost::apply_visitor (v, data);
      }
      operator boost::optional<const uc::String&>() const { return GetOptionalStringRef(); }
      //@}

      //@{
      /**
       * Get string value as an uc::String_optional.
       * \remarks Does not throw.
       */
      uc::String_optional GetString_optional(ResultCode* out_err = nullptr) const
      {
        struct visitor : public boost::static_visitor<uc::String_optional>
        {
          ResultCode* err_ptr;
          visitor (ResultCode* err_ptr) : err_ptr (err_ptr) {}

          uc::String_optional operator()(ResultCode err)
          {
            if (err_ptr) *err_ptr = err;
            return boost::none;
          }
          uc::String_optional operator()(const uc::String& s)
          {
            if (err_ptr) *err_ptr = S1_SUCCESS;
            return s;
          }
          uc::String_optional operator()(cxxapi::String* s)
          {
            if (err_ptr) *err_ptr = S1_SUCCESS;
            return s1::EvilUpcast<s1::api_impl::String> (s)->StrUCS();
          }
        };
        visitor v (out_err);
        return boost::apply_visitor (v, data);
      }
      operator uc::String_optional () const { return GetString_optional(); }
      //@}

      //@{
      /**
      * Get string object pointer (or \NULL if not pointing to one).
      * \remarks Does not throw.
      */
      api_impl::String* GetStringObj(ResultCode* out_err = nullptr) const
      {
        struct visitor : public boost::static_visitor<api_impl::String*>
        {
          ResultCode* err_ptr;
          visitor (ResultCode* err_ptr) : err_ptr (err_ptr) {}

          api_impl::String* operator()(ResultCode err)
          {
            if (err_ptr) *err_ptr = err;
            return nullptr;
          }
          api_impl::String* operator()(const uc::String& s)
          {
            if (err_ptr) *err_ptr = S1_E_FAILURE;
            return nullptr;
          }
          api_impl::String* operator()(cxxapi::String* s)
          {
            if (err_ptr) *err_ptr = S1_SUCCESS;
            return s1::EvilUpcast<s1::api_impl::String> (s);
          }
        };
        visitor v (out_err);
        return boost::apply_visitor (v, data);
      }
      operator api_impl::String* () const { return GetStringObj(); }
      //@}
    };
  } // namespace api_impl
} // namespace s1

#endif // __S1_CXXAPI_STRINGARG_H__
