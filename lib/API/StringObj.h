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
 * Public API string object implementation
 */
#ifndef __S1_CXXAPI_STRINGOBJ_H__
#define __S1_CXXAPI_STRINGOBJ_H__

#include "s1/String.h"

#include "base/LibraryObject.h"

#include "StringWrapper.h"

#include <boost/intrusive_ptr.hpp>

namespace s1
{
  namespace api_impl
  {
    class String : public s1::LibraryObject
    {
      StringWrapper str;

      String (uc::String&& str, s1::Library* lib) : LibraryObject (lib), str (std::move (str)) {}
      String (const uc::String& str, s1::Library* lib) : LibraryObject (lib), str (str) {}
    public:
      //@
      typedef std::tuple<ResultCode, boost::intrusive_ptr<String>, size_t> CreateResultType;
      /**
       * Create a String instance from some input string.
       * \remarks May throw!
       */
      static CreateResultType Create (s1::Library* lib, uc::String&& str);
      static CreateResultType Create (s1::Library* lib, const uc::String& str);
      static CreateResultType Create (s1::Library* lib, const char* str, size_t len = (size_t)~0);
      static CreateResultType Create (s1::Library* lib, const s1_char16* str, size_t len = (size_t)~0);
      static CreateResultType Create (s1::Library* lib, const s1_char32* str, size_t len = (size_t)~0);
      static CreateResultType Create (s1::Library* lib, const wchar_t* str, size_t len = (size_t)~0);
      static CreateResultType Create (s1::Library* lib, cxxapi::StringArg str);
      //@}

      const uc::String& StrUCS () const { return str.GetUCS (); }

      const char* Str () const { return str.GetUTF8 (); }
      const s1_char16* StrU16 () const { return str.GetUTF16 (); }
      const s1_char32* StrU32 () const { return str.GetUTF32 (); }
      const wchar_t* StrWCS () const { return str.GetWCS (); }
    private:
      template<typename Ch>
      static CreateResultType CreateCommon (s1::Library* lib, const Ch* str, size_t len,
                                            uc::String::ConversionResult<Ch> (*convert)(const Ch*, size_t));
    };
  } // namespace api_impl
} // namespace s1

#endif // __S1_CXXAPI_STRINGOBJ_H__
