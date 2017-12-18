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
 * Stream of bytes object
 */
#ifndef __S1_BYTESTREAM_H__
#define __S1_BYTESTREAM_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"
#include "s1/Ptr.h"

#define S1TYPE_INFO_s1_ByteStream   (S1_TYPE_MAKE_NAME(ByteStream), S1TYPE_INFO_s1_LibraryObject)
/**
 * Stream of bytes object.
 * Is used both to return 'raw' byte data from methods and to pass raw byte data
 * into the library.
 * \createdby s1_byte_stream_create_from_data(), s1_byte_stream_create_from_callback(),
 *   s1_byte_stream_create_restarted()
 * \extends s1_LibraryObject
 */
S1TYPE_DECLARE(S1TYPE_INFO_s1_ByteStream);

/**\def S1_BYTE_STREAM_SIZE_UNKNOWN
 * s1_byte_stream_size() return value indicating the size of the stream is not
 * known.
 */
#define S1_BYTE_STREAM_SIZE_UNKNOWN       (~(size_t)0)

/**
 * Get size of byte stream.
 * \param byteStream Stream get size of.
 * \returns \em Unreliable size of the stream.
 *   The size of the stream may be underreported, overreported or entirely
 *   unknown (return value of #S1_BYTE_STREAM_SIZE_UNKNOWN).
 * \remarks Implementations may set a result code of #S1_S_STREAM_SIZE_PROBABLY_TOO_SMALL
 *   or #S1_S_STREAM_SIZE_PROBABLY_TOO_LARGE as a suggestion whether the reported size is
 *   likely smaller or larger than the actual size.
 * \memberof s1_ByteStream
 */
S1_API(size_t) s1_byte_stream_size (s1_ByteStream* byteStream);

/**
 * Get next block of byte data from stream.
 * \param byteStream Stream to get data of.
 * \param data Pointer to variable receiving start if next block of data.
 * \returns Size of the data block, or 0 if no further data is available.
 * \memberof s1_ByteStream
 */
S1_API(size_t) s1_byte_stream_next (s1_ByteStream* byteStream, const char** data);

/**
 * Create a clone of the byte stream in a state that allows reading the
 * data from the start.
 * \param byteStream Stream to clone.
 * \returns A new byte stream object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * A stream may be not be restartable in which case \NULL is returned.
 * \memberof s1_ByteStream
 */
S1_API(s1_ByteStream*) s1_byte_stream_create_restarted (s1_ByteStream* byteStream);

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    /**
     * Stream of bytes object.
     * Is used both to return 'raw' byte data from methods and to pass raw byte data
     * into the library.
     * \createdby s1::Library::CreateByteStreamFromData(),
     *   s1::Library::CreateByteStreamFromCallback()
     */
    class ByteStream : public LibraryObject
    {
    public:
      /// Smart pointer class for ByteStream instances.
      typedef Ptr<ByteStream> Pointer;

      /**
       * Get size of byte stream.
       * \returns \em Unreliable size of the stream.
       *   The size of the stream may be underreported, overreported or entirely
       *   unknown (return value of #S1_BYTE_STREAM_SIZE_UNKNOWN).
       * \remarks Implementations may set a result code of #S1_S_STREAM_SIZE_PROBABLY_TOO_SMALL
       *   or #S1_S_STREAM_SIZE_PROBABLY_TOO_LARGE as a suggestion whether the reported size is
       *   likely smaller or larger than the actual size.
       */
      size_t Size ()
      {
        return s1_byte_stream_size (this);
      }

      /**
       * Get next block of byte data from stream.
       * \param data Variable receiving start if next block of data.
       * \returns Size of the data block, or 0 if no further data is available.
       */
      size_t Next (const char*& data)
      {
        return s1_byte_stream_next (this, &data);
      }

      /**
       * Create a clone of the byte stream in a state that allows reading the
       * data from the start.
       * \returns A new byte stream object.
       * A stream may be not be restartable in which case \nullptr is returned.
       */
      S1_RETURN_MOVE_REF_TYPE(ByteStream) CreateRestarted ()
      {
        return S1_RETURN_MOVE_REF(ByteStream,
                                  s1_byte_stream_create_restarted (this));
      }
    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::ByteStream ByteStream;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_BYTESTREAM_H__
