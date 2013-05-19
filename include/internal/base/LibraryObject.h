/**\file
 * Base class for objects created by a library.
 */
#ifndef __BASE_LIBRARYOBJECT_H__
#define __BASE_LIBRARYOBJECT_H__

#include "s1/LibraryObject.h"
#include "base/Library.h"

namespace s1
{
  class LibraryObject : public Object
  {
    LibraryPtr lib;
  public:
    LibraryObject (Library* lib) : lib (lib) {}
    
    Library* GetLibrary() { return lib.get(); }
    
    /**
     * Helper method to to set a library error code and return an appropriate
     * \c s1_bool result.
     */
    inline s1_bool ReturnErrorCode (s1_ResultCode code)
    {
      GetLibrary()->SetLastError (code);
      return S1_SUCCESSFUL(code);
    }
    /**
     * Helper method to to set the library error code to #S1_SUCCESS
     * and return the appropriate \c s1_bool result.
     */
    inline s1_bool ReturnSuccess ()
    {
      return ReturnErrorCode (S1_SUCCESS);
    }

    /**
     * Helper method to to set a library error code and return an arbitrary result.
     */
    template<typename T>
    inline const T& ReturnErrorCode (s1_ResultCode code, const T& result)
    {
      GetLibrary()->SetLastError (code);
      return result;
    }
    /**
     * Helper method to to set the library error code to #S1_SUCCESS
     * and return an arbitrary result.
     */
    template<typename T>
    inline const T& ReturnSuccess (const T& result)
    {
      return ReturnErrorCode (S1_SUCCESS, result);
    }
  }; 
} // namespace s1

#endif // __BASE_LIBRARYOBJECT_H__
