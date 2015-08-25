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

#ifndef __CODEGEN_STRINGSARRAY_H__
#define __CODEGEN_STRINGSARRAY_H__

#include "base/uc/String.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace s1
{
  namespace codegen
  {
    class StringsArray
    {
    protected:
      typedef std::vector<std::string> StrVector;
      StrVector strings;
    public:
      void AddString (const char* str);
      void AddString (const std::string& str);
      void AddString (const uc::String& str);
      void AddStrings (const StringsArray& other);
      void AddStrings (const StringsArray& other, unsigned int indent,
		       const char* indentStr = " ");
      
      const std::string& Get (size_t index) const;
      size_t Size() const;
    };
    typedef boost::shared_ptr<StringsArray> StringsArrayPtr;
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_STRINGSARRAY_H__
