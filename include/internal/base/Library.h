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
 * "Library" object
 */
#ifndef __BASE_LIBRARY_H__
#define __BASE_LIBRARY_H__

#include "base/Object.h"
#include "compiler/Compiler.h"

#include "s1/Error.h"

#include <boost/call_traits.hpp>
#include <boost/intrusive_ptr.hpp>

#include <type_traits>

namespace s1
{
  class Library : public Object
  {
    s1_ResultCode lastError; // TODO: Store with thread affinity
    /// The internal factory object
    Compiler compiler;  // TODO: Can probably be removed
  public:
    Library() : lastError (S1_SUCCESS), compiler (this) {}
    
    // TODO: Store stuff like memory allocator, global options etc... here
    s1_ResultCode GetLastError () { return lastError; }
    void SetLastError (s1_ResultCode code) { lastError = code; }
    
    Compiler& GetCompiler() { return compiler; }

    /**
     * Helper method to set a library error code and return an appropriate
     * \c s1_bool result.
     */
    inline s1_bool ReturnErrorCode (s1_ResultCode code)
    {
      SetLastError (code);
      return S1_SUCCESSFUL(code);
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
      SetLastError (code);
      return result;
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

    /**
     * Helper method to execute a block of code, dealing with exceptions
     * thrown. Sets the appropriate last error code.
     */
    template<typename Func>
    inline typename std::result_of<Func()>::type Try (
      Func func, typename boost::call_traits <typename std::result_of<Func()>::type>::param_type default)
    {
      try
      {
        auto result = func ();
        SetLastError (S1_SUCCESS);
        return std::move (result);
      }
      catch (std::bad_alloc)
      {
        SetLastError (S1_E_OUT_OF_MEMORY);
      }
      catch (...)
      {
        SetLastError (S1_E_FAILURE);
      }
      return default;
    }
  };
  typedef boost::intrusive_ptr<Library> LibraryPtr;
} // namespace s1

#endif // __BASE_LIBRARY_H__
