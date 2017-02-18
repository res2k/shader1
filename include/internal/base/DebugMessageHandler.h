/*
    Shader1
    Copyright (c) 2017 Frank Richter


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
 * Debug message handler definitions and access
 */
#ifndef __S1_BASE_DEBUGMESSAGEHANDLER_H__
#define __S1_BASE_DEBUGMESSAGEHANDLER_H__

#include "s1/DebugMessageHandler.h"

#include "uc/String.h"

#include <mutex>

namespace s1
{
  /// Wrapper for debug message handler functions
  class DebugMessageHandler
  {
  public:
    //@{
    /// Set a handler function.
    void SetHandler (DebugMessageHandlerFunc handlerUTF8, uintptr_t context);
    void SetHandlerWCS (DebugMessageHandlerWCSFunc handlerWide, uintptr_t context);
    //@}

    //@{
    /// Get handler function.
    DebugMessageHandlerFunc GetHandler (uintptr_t* context) const;
    DebugMessageHandlerWCSFunc GetHandlerWCS (uintptr_t* context) const;
    //@}

    //@{
    /**
     * Print a message.
     * \returns Whether message was printed. (Returns \c false if no handler was set.)
     */
    bool PrintMessage (const uc::String& str) const;
    bool PrintMessage (const wchar_t* str) const;
    bool PrintMessage (const char* str) const;
    //@}
  private:
    DebugMessageHandlerFunc handlerUTF8 = nullptr;
    DebugMessageHandlerWCSFunc handlerWide = nullptr;
    uintptr_t context = 0;
  };

  /// Default message handling
  struct DefaultDebugMessageHandler
  {
    static void PrintMessage (const uc::String& str);
    static void PrintMessage (const wchar_t* str);
    static void PrintMessage (const char* str);
  };

  namespace detail
  {
    //@{
    /// Global debug message handler
    struct GlobalDebugMessageHandler
    {
      std::mutex mutex;
      DebugMessageHandler handler;
    };
    extern GlobalDebugMessageHandler globalHandler;
    //@}
  } // namespace detail

  /// Access the global debug message handler.
  template<typename F>
  static typename std::result_of<F (DebugMessageHandler&)>::type AccessGlobalDebugMessageHandler (F function)
  {
    std::lock_guard<std::mutex> lock_globalHandler (detail::globalHandler.mutex);
    return function (detail::globalHandler.handler);
  }

  namespace detail
  {
    /// Thread-specific debug message handler
    extern S1_THREAD_LOCAL DebugMessageHandler* threadHandler;
  } // namespace detail

  /// Helper: Sets a "scope" for a temporary thread-specific debug message handler
  class ScopedThreadDebugMessageHandler
  {
  public:
    ScopedThreadDebugMessageHandler (DebugMessageHandler& handler)
    {
      prevHandler = detail::threadHandler;
      detail::threadHandler = &handler;
    }
    ~ScopedThreadDebugMessageHandler ()
    {
      detail::threadHandler = prevHandler;
    }
  private:
    DebugMessageHandler* prevHandler;
  };

  namespace detail
  {
    template<typename T>
    static void PrintMessage (T msg)
    {
      // First, try thread-specific handler
      if (threadHandler)
      {
        if (threadHandler->PrintMessage (msg)) return;
      }
      // Next, the global handler
      if (AccessGlobalDebugMessageHandler ([=](const DebugMessageHandler& h) { return h.PrintMessage (msg); }))
        return;
      // Lastly, the default handler
      DefaultDebugMessageHandler::PrintMessage (msg);
    }
  } // namespace detail

  //@{
  /// Print a debug message.
  static inline void PrintDebugMessage (const uc::String& str)
  {
    detail::PrintMessage (str);
  }
  static inline void PrintDebugMessage (const wchar_t* str)
  {
    detail::PrintMessage (str);
  }
  static inline void PrintDebugMessage (const char* str)
  {
    detail::PrintMessage (str);
  }
  //@}

} // namespace s1

#endif // __S1_BASE_DEBUGMESSAGEHANDLER_H__
