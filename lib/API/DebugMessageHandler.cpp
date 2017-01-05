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

#include "base/common.h"

#include "s1/DebugMessageHandler.h"

#include "base/DebugMessageHandler.h"

void s1_set_debug_message_handler (s1_debug_message_handler_func handler,
                                   uintptr_t userContext)
{
  s1::AccessGlobalDebugMessageHandler ([=](s1::DebugMessageHandler& dmh) { dmh.SetHandler (handler, userContext); });
}

void s1_set_debug_message_handler_ws (s1_debug_message_handler_ws_func handler,
                                      uintptr_t userContext)
{
  s1::AccessGlobalDebugMessageHandler ([=](s1::DebugMessageHandler& dmh) { dmh.SetHandlerWS (handler, userContext); });
}

s1_debug_message_handler_func s1_get_debug_message_handler (uintptr_t* userContextPtr)
{
  return s1::AccessGlobalDebugMessageHandler ([=](s1::DebugMessageHandler& dmh) { return dmh.GetHandler (userContextPtr); });
}

s1_debug_message_handler_ws_func s1_get_debug_message_handler_ws (uintptr_t* userContextPtr)
{
  return s1::AccessGlobalDebugMessageHandler ([=](s1::DebugMessageHandler& dmh) { return dmh.GetHandlerWS (userContextPtr); });
}

void s1_default_debug_message_handler (const char* message, uintptr_t userContext)
{
  s1::DefaultDebugMessageHandler::PrintMessage (message);
}

void s1_default_debug_message_handler_ws (const wchar_t* message, uintptr_t userContext)
{
  s1::DefaultDebugMessageHandler::PrintMessage (message);
}
