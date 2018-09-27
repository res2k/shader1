/*
    Shader1
    Copyright (c) 2017-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <boost/test/unit_test.hpp>

#include "s1/Library.h"
#include "s1/ByteStream.h"

#include <algorithm>
#include <functional>

#define E_STREAM_CUSTOM     S1_MAKE_ERROR(0xf, 9999)

BOOST_AUTO_TEST_SUITE(ApiByteStreamCXX11)

static const char* TestData (size_t& dataSize)
{
  static const char data[] = "The Quick Brown Fox Jumps Over The Lazy Dog";
  dataSize = sizeof (data);
  return data;
}

static void SetCleanupFlag (uintptr_t context, const char*)
{
  *(reinterpret_cast<bool*> (context)) = true;
}

struct CallbackStream
{
  const char* dataPointer;
  size_t remainingData;
  size_t chunkSize;
  bool cleanupCalled = false;

  CallbackStream (const char* dataPointer, size_t remainingData, size_t chunkSize)
    : dataPointer (dataPointer), remainingData (remainingData), chunkSize (chunkSize)
  {}

  size_t Next (const char*& data)
  {
    data = dataPointer;
    size_t thisChunk = std::min (remainingData, chunkSize);
    dataPointer += thisChunk;
    remainingData -= thisChunk;
    return thisChunk;
  }
  void Cleanup ()
  {
    cleanupCalled = true;
  }
  s1_ResultCode CreateRestarted (s1::ByteStream*& newStream)
  {
    newStream = nullptr;
    return E_STREAM_CUSTOM;
  }
};

BOOST_AUTO_TEST_CASE(CreateFromCallbackCXX2)
{
  size_t testDataSize;
  const char* testData = TestData (testDataSize);

  s1::Library::Pointer lib;
  s1_ResultCode err = s1::Library::Create (lib);
  BOOST_CHECK(S1_SUCCESSFUL(err));
  BOOST_CHECK_NE(lib, (s1::Library*)nullptr);

  for (size_t s = 1; s <= testDataSize; s++)
  {
    CallbackStream streamImpl (testData, testDataSize, s);

    s1::ByteStream::Pointer stream =
      lib->CreateByteStreamFromCallback (std::bind (&CallbackStream::Next, &streamImpl, std::placeholders::_1),
                                          std::bind (&CallbackStream::Cleanup, &streamImpl),
                                          std::bind (&CallbackStream::CreateRestarted, &streamImpl, std::placeholders::_1));
    BOOST_CHECK_NE(stream, (s1::ByteStream*)nullptr);
    BOOST_CHECK(S1_SUCCESSFUL(lib->GetLastError()));

    const char* testDataPos = testData;
    const char* block = nullptr;
    size_t blockSize;
    while ((blockSize = stream->Next (block)) != 0)
    {
      BOOST_CHECK_NE(block, nullptr);
      size_t totalStreamed = testDataPos - testData;
      BOOST_CHECK_LE(blockSize, testDataSize - totalStreamed);
      
      size_t considerSize = std::min (blockSize, testDataSize - totalStreamed);
      BOOST_CHECK_EQUAL(memcmp (block, testDataPos, considerSize), 0);
      testDataPos += considerSize;

      block = nullptr;
    }

    s1::ByteStream::Pointer stream2 = stream->CreateRestarted ();
    BOOST_CHECK_EQUAL(stream2, (s1::ByteStream*)nullptr);
    BOOST_CHECK(S1_ERROR_EQUAL(lib->GetLastError(), E_STREAM_CUSTOM));

    stream = nullptr;
    BOOST_CHECK(streamImpl.cleanupCalled);
  }
}

BOOST_AUTO_TEST_SUITE_END()
