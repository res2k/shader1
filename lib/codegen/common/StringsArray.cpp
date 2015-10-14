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

#include "codegen/common/StringsArray.h"

#include <string>

namespace s1
{
  namespace codegen
  {
    void StringsArray::AddString (const char* str)
    {
      strings.emplace_back (str);
    }

    void StringsArray::AddString (const std::string& str)
    {
      strings.emplace_back (str);
    }

    void StringsArray::AddString(const uc::String& str)
    {
      std::string str_utf8;
      str.toUTF8String (str_utf8);
      AddString (str_utf8);
    }
    
    void StringsArray::AddStrings (const StringsArray& other)
    {
      for (StrVector::const_iterator it = other.strings.begin();
	   it != other.strings.end();
	   ++it)
      {
	strings.emplace_back (*it);
      }
    }
    
    void StringsArray::AddStrings (const StringsArray& other, unsigned int indent,
				   const char* indentStr_)
    {
      std::string indentStr;
      for (unsigned int i = 0; i < indent; i++)
	indentStr.append (indentStr_);
      
      for (StrVector::const_iterator it = other.strings.begin();
	   it != other.strings.end();
	   ++it)
      {
	std::string newStr (indentStr);
	newStr.append (*it);
	strings.emplace_back (std::move (newStr));
      }
    }
      
    const std::string& StringsArray::Get (size_t index) const
    {
      return strings[index];
    }
      
    size_t StringsArray::Size () const
    {
      return strings.size();
    }
  } // namespace codegen
} // namespace s1
