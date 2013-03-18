/**\file
 * "Library" object
 */
#ifndef __BASE_LIBRARY_H__
#define __BASE_LIBRARY_H__

#include "base/Object.h"

#include <boost/intrusive_ptr.hpp>

namespace s1
{
  class Library : public Object
  {
  public:
    Library() {}
    
    // TODO: Store stuff like memory allocator, global options etc... here
  };
  typedef boost::intrusive_ptr<Library> LibraryPtr;
} // namespace s1

#endif // __BASE_LIBRARY_H__
