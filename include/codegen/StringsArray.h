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
      std::vector<std::string> strings;
    public:
      void AddString (const std::string& str);
      
      const std::string& Get (size_t index) const;
    };
    typedef boost::shared_ptr<StringsArray> StringsArrayPtr;
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_STRINGSARRAY_H__
