/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

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
     * Helper method to set a library error code and return an appropriate
     * \c s1_bool result.
     */
    inline s1_bool ReturnErrorCode (s1_ResultCode code)
    {
      return lib->ReturnErrorCode (code);
    }
    /**
     * Helper method to set the library error code to #S1_SUCCESS
     * and return the appropriate \c s1_bool result.
     */
    inline s1_bool ReturnSuccess ()
    {
      return ReturnErrorCode (S1_SUCCESS);
    }

    /**
     * Helper method to set a library error code and return an arbitrary result.
     */
    template<typename T>
    inline const T& ReturnErrorCode (s1_ResultCode code, const T& result)
    {
      return lib->ReturnErrorCode (code, result);
    }
    /**
     * Helper method to set the library error code to #S1_SUCCESS
     * and return an arbitrary result.
     */
    template<typename T>
    inline const T& ReturnSuccess (const T& result)
    {
      return ReturnErrorCode (S1_SUCCESS, result);
    }
    //@{
    /**
     * Helper method to return a Result<> value.
     * Sets error code from result and returns value from result or the default value.
     */
    template<typename T>
    inline const T& Return (const Result<T>& result, typename boost::call_traits<T>::param_type defaultVal)
    {
      return lib->Return (result, defaultVal);
    }
    template<typename T, typename U>
    inline T Return (Result<T>&& result, U&& defaultVal)
    {
      return lib->Return (std::move (result), std::forward<U> (defaultVal));
    }
    //@}

    /**
     * Helper method to execute a block of code, dealing with exceptions
     * thrown. Sets the appropriate last error code.
     */
    template<typename T, typename Func>
    inline Result<T> Try (Func func)
    {
      return lib->Try<T> (func);
    }
  }; 
} // namespace s1

#endif // __BASE_LIBRARYOBJECT_H__
