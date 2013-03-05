#ifndef __CODEGEN_STRINGSARRAY_H__
#define __CODEGEN_STRINGSARRAY_H__

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
      void AddString (const std::string& str);
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
