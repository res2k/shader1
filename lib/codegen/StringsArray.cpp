#include "codegen/StringsArray.h"

#include <string>

namespace s1
{
  namespace codegen
  {
    void StringsArray::AddString (const std::string& str)
    {
      strings.push_back (str);
    }
    
    void StringsArray::AddStrings (const StringsArray& other)
    {
      for (StrVector::const_iterator it = other.strings.begin();
	   it != other.strings.end();
	   ++it)
      {
	strings.push_back (*it);
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
	strings.push_back (newStr);
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
