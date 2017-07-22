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

#include "base/DebugMessageHandler.h"
#include "base/dynamic_tls.h"
#include "base/Exception.h"
#include "base/Object.h"
#include "base/Result.h"
#include "compiler/Compiler.h"

#include "s1/ResultCode_defs_base.h"

#include <boost/intrusive_ptr.hpp>

#include <type_traits>

namespace s1
{
  namespace detail
  {
    template<typename T>
    struct deduce_try_result
    {
      typedef Result<T> type;
    };

    template<typename T>
    struct deduce_try_result<Result<T>>
    {
      typedef Result<T> type;
    };
  } // namespace detail

  class Library : public Object
  {
  protected:
    struct PerThreadData
    {
      s1_ResultCode lastError = S1_SUCCESS;
      boost::optional<uc::String> errorInfo;

      virtual ~PerThreadData() {}
    };
    dynamic_tls::Handle perThreadSlot = dynamic_tls::InvalidHandle;

    /**
     * Allocation function for per-thread data.
     * Virtual so derived classed can also derive PerThreadData.
     */
    virtual PerThreadData* AllocPerThreadData () { return new PerThreadData; }
    /// Get per-thread data for current thread.
    PerThreadData& GetPerThreadData ()
    {
      auto data_p = reinterpret_cast<PerThreadData*> (dynamic_tls::GetValue (perThreadSlot));
      if (!data_p)
      {
        data_p = AllocPerThreadData ();
        dynamic_tls::SetValue (perThreadSlot, data_p);
      }
      return *data_p;
    }

    /// Message handler settings
    DebugMessageHandler messageHandler;
    /// The internal factory object
    Compiler compiler;  // TODO: Can probably be removed
  public:
    Library() : compiler (this)
    {
      perThreadSlot =
        dynamic_tls::Alloc ([](void* data, uintptr_t){ delete reinterpret_cast<PerThreadData*> (data); }, 0);
    }
    ~Library()
    {
      if (perThreadSlot != dynamic_tls::InvalidHandle)
      {
        dynamic_tls::Free (perThreadSlot,
          [](void* data, uintptr_t){ delete reinterpret_cast<PerThreadData*> (data); }, 0);
      }
    }
    
    // TODO: Store stuff like memory allocator, global options etc... here
    s1_ResultCode GetLastError () { return GetPerThreadData().lastError; }
    const uc::String* GetLastErrorInfo ()
    {
      auto errorInfo = GetPerThreadData().errorInfo;
      return errorInfo ? &(*errorInfo) : nullptr;
    }
    void SetLastError (s1_ResultCode code, const uc::String* info = nullptr)
    {
      auto& threadData = GetPerThreadData();
      threadData.lastError = code;
      if (info)
      {
        threadData.errorInfo = *info;
      }
      else
      {
        threadData.errorInfo = boost::none;
      }
    }
    
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
    inline T&& ReturnErrorCode (s1_ResultCode code, T&& result)
    {
      SetLastError (code);
      return std::forward<T> (result);
    }
    /**
     * Helper method to set the library error code to #S1_SUCCESS
     * and return an arbitrary result.
     */
    template<typename T>
    inline T&& ReturnSuccess (T&& result)
    {
      return ReturnErrorCode (S1_SUCCESS, std::forward<T> (result));
    }
    //@{
    /**
     * Helper method to return a Result<> value.
     * Sets error code from result and returns value from result or the default value.
     */
    template<typename T>
    inline const T& Return (const Result<T>& result, const T& defaultVal)
    {
      SetLastError (result.code(), result.errorInfo());
      return result.get_value_or (defaultVal);
    }
    template<typename T, typename U>
    inline T Return (Result<T>&& result, U&& defaultVal)
    {
      SetLastError (result.code(), result.errorInfo());
      return result.value_or (std::forward<U> (defaultVal));
    }
    //@}

    /**
     * Helper method to execute a block of code, dealing with exceptions
     * thrown. Sets the appropriate last error code.
     */
    template<typename Func>
    static inline typename detail::deduce_try_result<typename std::result_of<Func()>::type>::type Try (Func func)
    {
      try
      {
        return func ();
      }
      catch (std::bad_alloc)
      {
        return S1_E_OUT_OF_MEMORY;
      }
      catch (Exception& e)
      {
        return e.code ();
      }
      catch (std::exception& e)
      {
        return { S1_E_FAILURE, uc::String::fromLocal8Bit (e.what ()) };
      }
      return S1_E_FAILURE;
    }

    /// Debug message handler access
    DebugMessageHandler& GetDebugMessageHandler () { return messageHandler; }
  };
  typedef boost::intrusive_ptr<Library> LibraryPtr;
} // namespace s1

#endif // __BASE_LIBRARY_H__
