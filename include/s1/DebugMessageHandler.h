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
 * User-provided debug message handlers
 */
#ifndef __S1_DEBUGMESSAGEHANDLER_H__
#define __S1_DEBUGMESSAGEHANDLER_H__

#include "defs.h"

/**\name Debug message handlers
 * Debug message handlers are used to relay messages related to internal
 * verifications and consistency checks.
 *
 * Debug message handlers can be associated with s1_Library objects or
 * set globally. The library-associated handler has precedence; the global
 * message handler is used if no library-associated handler is set or no
 * library can be sensibly determined.
 *
 * Only one debug message handler can be set globally.
 * One debug message handler can be set per s1_Library (but each s1_Library
 * can have it's own).
 *
 * For convenience, debug message handlers can be set in both UTF-8 and
 * wide string variants. However, only one of these can be active at a
 * time. Setting one handler will "unset" the other.
 * @{ */
/**
 * Debug message handler callback function (UTF-8 variant).
 * \param message Message string.
 * \param userContext User-defined context value as passed to s1_set_debug_message_handler().
 */
typedef void (*s1_debug_message_handler_func)(const char* message, uintptr_t userContext);
/**
 * Debug message handler callback function (wide string variant).
 * \param message Message string.
 * \param userContext User-defined context value as passed to s1_set_debug_message_handler().
 */
typedef void (*s1_debug_message_handler_ws_func)(const wchar_t* message, uintptr_t userContext);

//@{
/**
 * Set global debug message handler.
 * \param handler Handler function receiving UTF-8 strings.
 * \param userContext User-defined context value.
 */
S1_API(void) s1_set_debug_message_handler (s1_debug_message_handler_func handler,
                                           uintptr_t userContext);
/**
 * Set global debug message handler.
 * \param handler Handler function receiving wide strings.
 * \param userContext User-defined context value.
 */
S1_API(void) s1_set_debug_message_handler_ws (s1_debug_message_handler_ws_func handler,
                                              uintptr_t userContext);

/**
 * Obtain UTF-8 variant of global debug message handler.
 * \param userContextPtr Optional argument to receive "user context" value.
 * \returns UTF-8 variant of global debug message handler, or \NULL if none set
 *   (this includes a wide string variant being set).
 */
S1_API(s1_debug_message_handler_func) s1_get_debug_message_handler (uintptr_t* userContextPtr S1_ARG_DEFAULT (0));
/**
 * Obtain wide string variant of global debug message handler.
 * \param userContextPtr Optional argument to receive "user context" value.
 * \returns wide string variant of global debug message handler, or \NULL if none set
 *   (this includes an UTF-8 variant being set).
 */
S1_API(s1_debug_message_handler_ws_func) s1_get_debug_message_handler_ws (uintptr_t* userContextPtr S1_ARG_DEFAULT (0));

/**
 * Default debug message handler (UTF-8 variant).
 * \param message Message string.
 * \param userContext Ignored.
 * \remarks Prints \a message to \c stderr. On Windows, additionally outputs to
 *   OutputDebugString().
 */
S1_API(void) s1_default_debug_message_handler (const char* message, uintptr_t userContext);
/**
* Default debug message handler (wide string variant).
* \param message Message string.
* \param userContext Ignored.
* \remarks Prints \a message to \c stderr. On Windows, additionally outputs to
*   OutputDebugString().
*/
S1_API (void) s1_default_debug_message_handler_ws (const wchar_t* message, uintptr_t userContext);

/// Debug message handler variants.
enum s1_DebugMessageHandlerVariant
{
  /// UTF-8 variant
  S1_DMH_UTF8 = 0,
  /// Wide string variant
  S1_DMH_WIDE_STRING = 1
};

/// Obtain preferred message handler for current platform.
static inline s1_DebugMessageHandlerVariant s1_get_preferred_debug_message_handler ()
{
#if defined(_WIN32)
  return S1_DMH_WIDE_STRING;
#else
  return S1_DMH_UTF8;
#endif
}
//@}

#if defined(__cplusplus)
namespace s1
{
  typedef s1_debug_message_handler_func DebugMessageHandlerFunc;
  typedef s1_debug_message_handler_ws_func DebugMessageHandlerWSFunc;

  /// \copydoc s1_set_debug_message_handler
  static inline void SetDebugMessageHandler (DebugMessageHandlerFunc handler,
                                             uintptr_t userContext)
  {
    return s1_set_debug_message_handler (handler, userContext);
  }
  /// \copydoc s1_set_debug_message_handler_ws
  static inline void SetDebugMessageHandlerWS (DebugMessageHandlerWSFunc handler,
                                               uintptr_t userContext)
  {
    return s1_set_debug_message_handler_ws (handler, userContext);
  }

  //@{
  /// \copydoc s1_get_debug_message_handler
  static inline DebugMessageHandlerFunc GetDebugMessageHandler ()
  {
    return s1_get_debug_message_handler ();
  }
  static inline DebugMessageHandlerFunc GetDebugMessageHandler (uintptr_t& userContext)
  {
    return s1_get_debug_message_handler (&userContext);
  }
  //@}
  //@{
  /// \copydoc s1_get_debug_message_handler_ws
  static inline DebugMessageHandlerWSFunc GetDebugMessageHandlerWS ()
  {
    return s1_get_debug_message_handler_ws ();
  }
  static inline DebugMessageHandlerWSFunc GetDebugMessageHandlerWS (uintptr_t& userContext)
  {
    return s1_get_debug_message_handler_ws (&userContext);
  }
  //@}

  /// \copydoc s1_default_debug_message_handler
  static inline void DefaultDebugMessageHandler (const char* message, uintptr_t userContext)
  {
    s1_default_debug_message_handler (message, userContext);
  }
  /// \copydoc s1_default_debug_message_handler_ws
  static inline void DefaultDebugMessageHandlerWS (const wchar_t* message, uintptr_t userContext)
  {
    s1_default_debug_message_handler_ws (message, userContext);
  }

  typedef s1_DebugMessageHandlerVariant DebugMessageHandlerVariant;

  /// \copydoc s1_get_preferred_debug_message_handler
  static inline DebugMessageHandlerVariant GetPreferredDebugMessageHandler ()
  {
    return s1_get_preferred_debug_message_handler ();
  }
}
#endif // defined(__cplusplus)

/** @} */

#endif // __S1_DEBUGMESSAGEHANDLER_H__
