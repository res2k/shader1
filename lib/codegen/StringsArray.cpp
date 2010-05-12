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
