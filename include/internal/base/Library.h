/**\file
 * "Library" object
 */
#ifndef __BASE_LIBRARY_H__
#define __BASE_LIBRARY_H__

#include "base/Object.h"

#include "s1/Error.h"

#include <boost/intrusive_ptr.hpp>

namespace s1
{
  class Library : public Object
  {
    s1_ErrorCode lastError; // TODO: Store with thread affinity
  public:
    Library() : lastError (S1_SUCCESS) {}
    
    // TODO: Store stuff like memory allocator, global options etc... here
    s1_ErrorCode GetLastError () { return lastError; }
    void SetLastError (s1_ErrorCode code) { lastError = code; }
  };
  typedef boost::intrusive_ptr<Library> LibraryPtr;
} // namespace s1

#endif // __BASE_LIBRARY_H__
