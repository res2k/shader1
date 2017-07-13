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
#include "base/ResultCode_internal.h"

#include "ByteStream.h"

namespace s1
{
  namespace api_impl
  {
    DataByteStream::Buffer::Buffer (const char* data, size_t dataSize,
                                    s1_byte_stream_data_cleanup_func cleanupFunc,
                                    uintptr_t cleanupContext)
      : data (data), dataSize (dataSize), cleanupFunc (cleanupFunc),
        cleanupContext (cleanupContext)
    {}

    DataByteStream::Buffer::~Buffer ()
    {
      if (cleanupFunc)
      {
        cleanupFunc (cleanupContext, data);
      }
    }

    //-----------------------------------------------------------------------

    DataByteStream::DataByteStream (s1::Library* lib, const char* data, size_t dataSize,
                                    s1_byte_stream_data_cleanup_func cleanupFunc,
                                    uintptr_t cleanupContext)
      : ByteStream (lib), buffer (new Buffer (data, dataSize, cleanupFunc, cleanupContext))
    {}

    DataByteStream::DataByteStream (s1::Library* lib, BufferPtr buffer)
      : ByteStream (lib), buffer (buffer)
    {}

    ByteStream::SizeResult DataByteStream::Size ()
    {
      return std::make_pair (S1_SUCCESS, buffer->dataSize);
    }

    size_t DataByteStream::Next (const char*& buf)
    {
      if (bufferRead)
      {
        buf = nullptr;
        return 0;
      }
      bufferRead = true;
      buf = buffer->data;
      return buffer->dataSize;
    }

    ByteStream::CreateRestartedResult DataByteStream::CreateRestarted ()
    {
      boost::intrusive_ptr<ByteStream> newStream (new DataByteStream (GetLibrary(), buffer));
      return std::make_pair (S1_SUCCESS, newStream->DowncastEvil<s1_ByteStream> ());
    }

    //-----------------------------------------------------------------------

    CallbackByteStream::CallbackByteStream (s1::Library* lib,
                                            uintptr_t userContext,
                                            s1_byte_stream_data_func dataFunc,
                                            s1_byte_stream_cleanup_func cleanupFunc,
                                            s1_byte_stream_create_restarted_func createRestartedFunc,
                                            size_t size)
      : ByteStream (lib), userContext (userContext), dataFunc (dataFunc),
        cleanupFunc (cleanupFunc), createRestartedFunc (createRestartedFunc),
        size (size)
    {}

    CallbackByteStream::~CallbackByteStream ()
    {
      if (cleanupFunc)
      {
        cleanupFunc (userContext);
      }
    }

    ByteStream::SizeResult CallbackByteStream::Size ()
    {
      return std::make_pair (S1_SUCCESS, size);
    }

    size_t CallbackByteStream::Next (const char*& buf)
    {
      return dataFunc (userContext, &buf);
    }

    ByteStream::CreateRestartedResult CallbackByteStream::CreateRestarted ()
    {
      if (!createRestartedFunc) return std::make_pair (S1_E_STREAM_NOT_RESTARTABLE, nullptr);

      s1_ByteStream* newStream = nullptr;
      s1_ResultCode code = createRestartedFunc (userContext, &newStream);
      CreateRestartedResult result = std::make_pair (static_cast<ResultCode> (code), newStream);
      if (newStream) newStream->Release ();
      return result;
    }
  } // namespace api_impl
} // namespace s1

size_t s1_byte_stream_size (s1_ByteStream* byteStream)
{
  S1_ASSERT_MSG(byteStream, "NULL ByteStream", S1_BYTE_STREAM_SIZE_UNKNOWN);
  auto byteStream_impl (s1::EvilUpcast<s1::api_impl::ByteStream> (byteStream));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (byteStream_impl->GetDebugMessageHandler ());

  return byteStream_impl->Return (byteStream_impl->Try (
    [=]() {
      s1::api_impl::ByteStream::SizeResult size = byteStream_impl->Size();
      return s1::Result<size_t> (size.second, size.first);
    }), S1_BYTE_STREAM_SIZE_UNKNOWN);
}

size_t s1_byte_stream_next (s1_ByteStream* byteStream, const char** data)
{
  S1_ASSERT_MSG(byteStream, "NULL ByteStream", S1_BYTE_STREAM_SIZE_UNKNOWN);
  auto byteStream_impl (s1::EvilUpcast<s1::api_impl::ByteStream> (byteStream));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (byteStream_impl->GetDebugMessageHandler ());

  if (!data) return byteStream_impl->ReturnErrorCode (S1_E_INVALID_ARG_N (0), 0);

  return byteStream_impl->Return (byteStream_impl->Try (
    [=]() -> s1::Result<size_t> {
      return byteStream_impl->Next (*data);
    }), 0);
}

s1_ByteStream* s1_byte_stream_create_restarted (s1_ByteStream* byteStream)
{
  S1_ASSERT_MSG(byteStream, "NULL ByteStream", nullptr);
  auto byteStream_impl (s1::EvilUpcast<s1::api_impl::ByteStream> (byteStream));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (byteStream_impl->GetDebugMessageHandler ());

  return byteStream_impl->Return (byteStream_impl->Try (
    [=]() {
      s1::api_impl::ByteStream::CreateRestartedResult restarted = byteStream_impl->CreateRestarted ();
      if (restarted.second)
      {
        restarted.second->AddRef ();
        return s1::Result<s1_ByteStream*> (restarted.second, restarted.first);
      }
      else
        return s1::Result<s1_ByteStream*> (restarted.first);
    }), nullptr);
}
