#ifndef __HASH_UNICODESTRING_H__
#define __HASH_UNICODESTRING_H__

#include "unordered_map"
#include <unicode/unistr.h>

namespace std
{
  namespace tr1
  {
    template<>
    class hash<UnicodeString>
    {
    public:
      size_t operator() (const UnicodeString& s) const
      { return s.hashCode(); }
    };
  }
}

// Boost hash function
inline std::size_t hash_value (const UnicodeString& s)
{
  return s.hashCode();
}

#endif // __HASH_UNICODESTRING_H__
