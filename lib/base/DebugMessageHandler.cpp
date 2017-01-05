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
#include "base/DebugMessageHandler.h"
#include "base/uc/UTF16Decoder.h"
#include "base/uc/UTF16Encoder.h"
#include "base/uc/UTF8Decoder.h"
#include "base/uc/UTF8Encoder.h"

#include <boost/container/small_vector.hpp>

#if defined (_WIN32)
#include <Windows.h>
#endif

namespace s1
{
  typedef boost::container::small_vector<wchar_t, 256> utf8_decoded_buf;

  static utf8_decoded_buf UTF8toWide (const char* str)
  {
    size_t strLen = std::char_traits<char>::length (str);
    utf8_decoded_buf buf_ws;
    buf_ws.reserve (strLen);
    const char* strPos = str;
    uc::UTF8Decoder decoder;
#if CXX_SIZEOF_WCHAR_T == 2
    uc::UTF16Encoder encoder;
#endif
    size_t outputPos = 0;
    while (strPos < str + strLen)
    {
      uc::Char32 nextChar;
      if (decoder (strPos, str + strLen, nextChar) != uc::UTF16Decoder::drSuccess)
      {
        break;
      }

#if CXX_SIZEOF_WCHAR_T == 2
      bool fullyEncoded = false;
      do
      {
        buf_ws.push_back (0);
        wchar_t* buf_ptr = buf_ws.data () + outputPos;
        auto encodeResult = encoder (nextChar,
                                     reinterpret_cast<uc::Char16*&> (buf_ptr),
                                     reinterpret_cast<uc::Char16*> (buf_ptr + (buf_ws.size () - outputPos)));
        fullyEncoded = encodeResult = uc::UTF16Encoder::erSuccess;
        outputPos = buf_ptr - buf_ws.data ();
        if (!fullyEncoded && (encodeResult != uc::UTF16Encoder::erOutputOverflow)) break;
      } while (!fullyEncoded);
#else
      buf_ws.push_back (static_cast<wchar_t> (nextChar));
#endif
    }
    buf_ws.push_back (0);
    return buf_ws;
  }

  // ------------------------------------------------------------------------

  void DebugMessageHandler::SetHandler (DebugMessageHandlerFunc handlerUTF8, uintptr_t context)
  {
    this->handlerUTF8 = handlerUTF8;
    this->handlerWide = nullptr;
    this->context = context;
  }

  void DebugMessageHandler::SetHandlerWS (DebugMessageHandlerWSFunc handlerWide, uintptr_t context)
  {
    this->handlerUTF8 = nullptr;
    this->handlerWide = handlerWide;
    this->context = context;
  }

  DebugMessageHandlerFunc DebugMessageHandler::GetHandler (uintptr_t* context) const
  {
    if (handlerUTF8)
    {
      if (context) *context = this->context;
      return handlerUTF8;
    }
    return nullptr;
  }

  DebugMessageHandlerWSFunc DebugMessageHandler::GetHandlerWS (uintptr_t* context) const
  {
    if (handlerWide)
    {
      if (context) *context = this->context;
      return handlerWide;
    }
    return nullptr;
  }

  bool DebugMessageHandler::PrintMessage (const uc::String& str) const
  {
    if (handlerWide)
    {
    #if CXX_SIZEOF_WCHAR_T == 2
      handlerWide (reinterpret_cast<const wchar_t*> (str.data ()), context);
    #elif CXX_SIZEOF_WCHAR_T == 4
      boost::container::small_vector<wchar_t, 256> buf;
      buf.reserve (str.size () + 1);
      String::CharacterIterator it (str);
      while (it.hasNext ())
      {
        buf.push_back (static_cast<wchar_t> (it.next32PostInc ()));
      }
      buf.push_back (0);
      handlerWide (buf.data (), context);
    #else
      return false;
    #endif
      return true;
    }
    else if (handlerUTF8)
    {
      std::string s_utf8;
      str.toUTF8String (s_utf8);
      handlerUTF8 (s_utf8.c_str (), context);
      return true;
    }
    return false;
  }

  bool DebugMessageHandler::PrintMessage (const wchar_t* str) const
  {
    if (handlerWide)
    {
      handlerWide (str, context);
      return true;
    }
    else if (handlerUTF8)
    {
      size_t strLen = std::char_traits<wchar_t>::length (str);
      boost::container::small_vector<char, 256> buf_utf8;
      buf_utf8.reserve (strLen);
      const wchar_t* strPos = str;
    #if CXX_SIZEOF_WCHAR_T == 2
      uc::UTF16Decoder decoder;
    #endif
      uc::UTF8Encoder encoder;
      size_t outputPos = 0;
      while (strPos < str + strLen)
      {
        uc::Char32 nextChar;
      #if CXX_SIZEOF_WCHAR_T == 2
        if (decoder (reinterpret_cast<const uc::Char16*&> (strPos),
                     reinterpret_cast<const uc::Char16*> (str + strLen), nextChar)
            != uc::UTF16Decoder::drSuccess)
        {
          break;
        }
      #else
        nextChar = static_cast<uc::Char32> (*str++);
      #endif

        bool fullyEncoded = false;
        do
        {
          buf_utf8.push_back (0);
          char* buf_ptr = buf_utf8.data () + outputPos;
          auto encodeResult = encoder (nextChar, buf_ptr, buf_ptr + (buf_utf8.size () - outputPos));
          fullyEncoded = encodeResult = uc::UTF8Encoder::erSuccess;
          outputPos = buf_ptr - buf_utf8.data ();
          if (!fullyEncoded && (encodeResult != uc::UTF8Encoder::erOutputOverflow)) break;
        } while (!fullyEncoded);
      }
      buf_utf8.push_back (0);
      handlerUTF8 (buf_utf8.data (), context);
      return true;
    }
    return false;
  }

  bool DebugMessageHandler::PrintMessage (const char* str) const
  {
    if (handlerWide)
    {
      auto buf_ws = UTF8toWide (str);
      handlerWide (buf_ws.data (), context);
      return true;
    }
    else if (handlerUTF8)
    {
      handlerUTF8 (str, context);
      return true;
    }
    return false;
  }

  namespace detail
  {
    GlobalDebugMessageHandler globalHandler;

    static void CleanupDebugMessageHandler (DebugMessageHandler* p) { /* do nothing */ }

    boost::thread_specific_ptr<DebugMessageHandler> threadHandler (&CleanupDebugMessageHandler);
  } // namespace detail

  void DefaultDebugMessageHandler::PrintMessage (const uc::String& str)
  {
  #if CXX_SIZEOF_WCHAR_T == 2
    PrintMessage (reinterpret_cast<const wchar_t*> (str.data ()));
  #else
    std::string s_utf8;
    str.toUTF8String (s_utf8);
    PrintMessage (s_utf8.data ());
#endif
  }

  void DefaultDebugMessageHandler::PrintMessage (const wchar_t* str)
  {
  #if defined (_WIN32)
    OutputDebugStringW (str);
  #endif
    std::wcerr << str << std::endl;
  }

  void DefaultDebugMessageHandler::PrintMessage (const char* str)
  {
  #if defined (_WIN32)
    auto buf_ws = UTF8toWide (str);
    PrintMessage (buf_ws.data ());
#else
    std::cerr << str << std::endl;
  #endif
  }
} // namespace s1
