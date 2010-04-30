#include <boost/functional/hash.hpp>

#include "intermediate/RegisterID.h"

namespace s1
{
  namespace intermediate
  {
    std::size_t RegisterID::hash_value () const
    {
      size_t seed = 0;
      boost::hash_combine (seed, bank);
      boost::hash_combine (seed, num);
      return seed;
    }
  } // namespace intermediate
} // namespace s1

