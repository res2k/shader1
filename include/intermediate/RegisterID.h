#ifndef __INTERMEDIATE_REGISTERID_H__
#define __INTERMEDIATE_REGISTERID_H__

#include <stdlib.h>
#include <boost/unordered_set.hpp>

namespace s1
{
  namespace intermediate
  {
    class Sequence;
    
    class RegisterID
    {
    protected:
      friend class Sequence;
      
      unsigned int bank;
      unsigned int num;
      
      RegisterID (unsigned int bank, unsigned int num) : bank (bank), num (num) {}
    public:
      RegisterID () : bank (~0), num (~0) {}
      
      bool IsValid() const
      { return (bank != (unsigned int)~0) && (num != (unsigned int)~0); }
      
      bool operator== (const RegisterID& other) const
      { return (bank == other.bank) && (num == other.num); }
      bool operator!= (const RegisterID& other) const
      { return (bank != other.bank) || (num != other.num); }
      
      size_t hash_value () const;
    };
    
    inline size_t hash_value (const RegisterID& id)
    { return id.hash_value(); }
    
    typedef boost::unordered_set<RegisterID> RegisterIDSet;
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_REGISTERID_H__
