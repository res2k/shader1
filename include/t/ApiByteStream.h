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

#include <cxxtest/TestSuite.h>

#include "s1/Library.h"
#include "s1/ByteStream.h"

#include <algorithm>
#include <functional>

#define E_STREAM_CUSTOM     S1_MAKE_ERROR(0xf, 9999)

class ApiByteStreamTestSuite : public CxxTest::TestSuite
{
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

    static size_t NextCallback (uintptr_t context, const char** data)
    {
      return reinterpret_cast<CallbackStream*> (context)->Next (*data);
    }
    static void CleanupCallback (uintptr_t context)
    {
      reinterpret_cast<CallbackStream*> (context)->Cleanup ();
    }
  };
public:
  void testCreateFromDataErrorCXX (void)
  {
    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);

    s1::ByteStream::Pointer stream = lib->CreateByteStreamFromData ((const char*)nullptr, 0);
    TS_ASSERT_DIFFERS(stream, (s1::ByteStream*)nullptr);
    TS_ASSERT(S1_SUCCESSFUL(lib->GetLastError()));

    stream = lib->CreateByteStreamFromData ((const char*)nullptr, 1);
    TS_ASSERT_EQUALS(stream, (s1::ByteStream*)nullptr);
    TS_ASSERT(S1_ERROR_EQUAL(lib->GetLastError(), S1_E_INVALID_ARG));
  }

  void testCreateFromDataCXX (void)
  {
    size_t testDataSize;
    const char* testData = TestData (testDataSize);

    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);

    s1::ByteStream::Pointer stream = lib->CreateByteStreamFromData (testData, testDataSize);
    TS_ASSERT_DIFFERS(stream, (s1::ByteStream*)nullptr);
    TS_ASSERT(S1_SUCCESSFUL(lib->GetLastError()));

    const char* testDataPos = testData;
    const char* block = nullptr;
    size_t blockSize;
    while ((blockSize = stream->Next (block)) != 0)
    {
      TS_ASSERT_DIFFERS(block, nullptr);
      size_t totalStreamed = testDataPos - testData;
      TS_ASSERT_LESS_THAN_EQUALS(blockSize, testDataSize - totalStreamed);
      
      size_t considerSize = std::min (blockSize, testDataSize - totalStreamed);
      TS_ASSERT_EQUALS(memcmp (block, testDataPos, considerSize), 0);
      testDataPos += considerSize;

      block = nullptr;
    }
  }

  void testCreateFromDataRestartCXX (void)
  {
    size_t testDataSize;
    const char* testData = TestData (testDataSize);

    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);

    s1::ByteStream::Pointer stream = lib->CreateByteStreamFromData (testData, testDataSize);
    TS_ASSERT_DIFFERS(stream, (s1::ByteStream*)nullptr);
    TS_ASSERT(S1_SUCCESSFUL(lib->GetLastError()));

    const char* block = nullptr;
    size_t blockSize;
    while ((blockSize = stream->Next (block)) != 0) {}

    s1::ByteStream::Pointer stream2 = stream->CreateRestarted ();
    TS_ASSERT(S1_SUCCESSFUL(lib->GetLastError()));

    const char* testDataPos = testData;
    while ((blockSize = stream2->Next (block)) != 0)
    {
      TS_ASSERT_DIFFERS(block, nullptr);
      size_t totalStreamed = testDataPos - testData;
      TS_ASSERT_LESS_THAN_EQUALS(blockSize, testDataSize - totalStreamed);
      
      size_t considerSize = std::min (blockSize, testDataSize - totalStreamed);
      TS_ASSERT_EQUALS(memcmp (block, testDataPos, considerSize), 0);
      testDataPos += considerSize;

      block = nullptr;
    }
  }

  void testCreateFromDataCleanupCXX (void)
  {
    size_t testDataSize;
    const char* testData = TestData (testDataSize);
    bool cleanupFlag = false;

    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);

    s1::ByteStream::Pointer stream =
      lib->CreateByteStreamFromData (testData, testDataSize,
                                     &SetCleanupFlag, reinterpret_cast<uintptr_t> (&cleanupFlag));
    TS_ASSERT_DIFFERS(stream, (s1::ByteStream*)nullptr);
    TS_ASSERT(S1_SUCCESSFUL(lib->GetLastError()));

    const char* testDataPos = testData;
    const char* block = nullptr;
    size_t blockSize;
    while ((blockSize = stream->Next (block)) != 0)
    {
      TS_ASSERT_DIFFERS(block, nullptr);
      size_t totalStreamed = testDataPos - testData;
      TS_ASSERT_LESS_THAN_EQUALS(blockSize, testDataSize - totalStreamed);
      
      size_t considerSize = std::min (blockSize, testDataSize - totalStreamed);
      TS_ASSERT_EQUALS(memcmp (block, testDataPos, considerSize), 0);
      testDataPos += considerSize;

      block = nullptr;
    }

    stream = nullptr;
    TS_ASSERT(cleanupFlag);
  }

  void testCreateFromCallbackErrorCXX (void)
  {
    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);

    s1::ByteStream::Pointer stream = lib->CreateByteStreamFromCallback (0u, nullptr, nullptr, nullptr);
    TS_ASSERT_EQUALS(stream, (s1::ByteStream*)nullptr);
    TS_ASSERT(S1_ERROR_EQUAL(lib->GetLastError(), S1_E_INVALID_ARG));
  }

  void testCreateFromCallbackCXX (void)
  {
    size_t testDataSize;
    const char* testData = TestData (testDataSize);

    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);

    for (size_t s = 1; s <= testDataSize; s++)
    {
      CallbackStream streamImpl (testData, testDataSize, s);

      s1::ByteStream::Pointer stream =
        lib->CreateByteStreamFromCallback (reinterpret_cast<uintptr_t> (&streamImpl),
                                           &CallbackStream::NextCallback,
                                           &CallbackStream::CleanupCallback);
      TS_ASSERT_DIFFERS(stream, (s1::ByteStream*)nullptr);
      TS_ASSERT(S1_SUCCESSFUL(lib->GetLastError()));

      const char* testDataPos = testData;
      const char* block = nullptr;
      size_t blockSize;
      while ((blockSize = stream->Next (block)) != 0)
      {
        TS_ASSERT_DIFFERS(block, nullptr);
        size_t totalStreamed = testDataPos - testData;
        TS_ASSERT_LESS_THAN_EQUALS(blockSize, testDataSize - totalStreamed);
        
        size_t considerSize = std::min (blockSize, testDataSize - totalStreamed);
        TS_ASSERT_EQUALS(memcmp (block, testDataPos, considerSize), 0);
        testDataPos += considerSize;

        block = nullptr;
      }

      s1::ByteStream::Pointer stream2 = stream->CreateRestarted ();
      TS_ASSERT_EQUALS(stream2, (s1::ByteStream*)nullptr);
      TS_ASSERT(S1_ERROR_EQUAL(lib->GetLastError(), S1_E_STREAM_NOT_RESTARTABLE));

      stream = nullptr;
      TS_ASSERT(streamImpl.cleanupCalled);
    }
  }
};
