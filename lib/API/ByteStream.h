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
 * Public API ByteStream object implementation
 */
#ifndef __S1_CXXAPI_BYTESTREAM_H__
#define __S1_CXXAPI_BYTESTREAM_H__

#include "s1/ByteStream.h"

#include "s1/Library.h"
#include "base/LibraryObject.h"

#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace s1
{
  namespace api_impl
  {
    /**
     * Abstract base class for ByteStream functions.
     */
    class ByteStream : public s1::LibraryObject
    {
    public:
      ByteStream (s1::Library* lib) : s1::LibraryObject (lib) {}

      typedef std::pair<ResultCode, size_t> SizeResult;
      virtual SizeResult Size () = 0;
      virtual size_t Next (const char*& buf) = 0;
      typedef std::pair<ResultCode, s1::cxxapi::ByteStream::Pointer> CreateRestartedResult;
      virtual CreateRestartedResult CreateRestarted () = 0;
    };

    /**
     * ByteStream implementation on a single block of data.
     */
    class DataByteStream : public ByteStream
    {
    public:
      DataByteStream (s1::Library* lib, const char* data, size_t dataSize,
                      s1_byte_stream_data_cleanup_func cleanupFunc, uintptr_t cleanupContext);

      SizeResult Size () override;
      size_t Next (const char*& buf) override;
      CreateRestartedResult CreateRestarted () override;
    protected:
      /// Buffer data. Shared internally to support CreateRestarted().
      struct Buffer : public boost::intrusive_ref_counter<Buffer>
      {
        const char* data;
        size_t dataSize;
        s1_byte_stream_data_cleanup_func cleanupFunc;
        uintptr_t cleanupContext;

        Buffer (const char* data, size_t dataSize,
                s1_byte_stream_data_cleanup_func cleanupFunc, uintptr_t cleanupContext);
        ~Buffer ();
      };
      typedef boost::intrusive_ptr<Buffer> BufferPtr;
      BufferPtr buffer;
      /// Whether we already read the buffer.
      bool bufferRead = false;

      DataByteStream (s1::Library* lib, BufferPtr buffer);
    };

    /**
     * ByteStream implementation with user-supplied callbacks.
     */
    class CallbackByteStream : public ByteStream
    {
    public:
      CallbackByteStream (s1::Library* lib,
                          uintptr_t userContext,
                          s1_byte_stream_data_func dataFunc,
                          s1_byte_stream_cleanup_func cleanupFunc,
                          s1_byte_stream_create_restarted_func createRestartedFunc,
                          size_t size);
      ~CallbackByteStream ();

      SizeResult Size () override;
      size_t Next (const char*& buf) override;
      CreateRestartedResult CreateRestarted () override;
    protected:
      uintptr_t userContext;
      s1_byte_stream_data_func dataFunc;
      s1_byte_stream_cleanup_func cleanupFunc;
      s1_byte_stream_create_restarted_func createRestartedFunc;
      size_t size;
    };
  } // namespace api_impl
} // namespace s1

#endif // __S1_CXXAPI_BYTESTREAM_H__
