/*
    Shader1
    Copyright (c) 2010-2016 Frank Richter


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
 * Main library entry object
 */
#ifndef __S1_LIBRARY_H__
#define __S1_LIBRARY_H__

#include "s1/warn_off.h"

#include "s1/DebugMessageHandler.h"
#include "s1/Object.h"
#include "s1/Ptr.h"
#include "s1/ResultCode.h"
#include "s1/StringArg.h"

#define S1TYPE_INFO_s1_Library   (S1_TYPE_MAKE_NAME(Library), S1TYPE_INFO_s1_Object)
/**
 * Library object, generally serves as a factory
 * for all available object types.
 * Since libraries create all other object types it's the first
 * object that needs to be called in order to use the Shader1 API.
 * \createdby s1_create_library()
 * \extends s1_Object
 */
S1TYPE_DECLARE(S1TYPE_INFO_s1_Library);

/**
 * Create a library object. A library object generally serves as a factory
 * for all available object types.
 * \param out Pointer receiving the new library object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * \returns If successful, the return code fulfills #S1_SUCCESSFUL.
 *   Otherwise, the return code is an error code indicating why the
 *   library could not be created.
 */
S1_API(s1_ResultCode) s1_create_library (s1_Library** out);

/**
 * Return the last error code set by a function.
 * \param lib Library request error code from.
 * \memberof s1_Library
 */
// TODO: More docs on error handling, link
S1_API(s1_ResultCode) s1_library_get_last_error (s1_Library* lib);
/**
 * Reset the last error code (to #S1_SUCCESS).
 * \param lib Library to clear error code on.
 * \memberof s1_Library
 */
S1_API(void) s1_library_clear_last_error (s1_Library* lib);
/**
 * Return additional information for the last error code set by a function.
 * In some cases, additional information for an error result is available.
 * This function will return any such information, or \NULL if none is
 * available.
 * String is only guaranteed valid until the next S1 function call.
 * \param lib Library request error code from.
 * \memberof s1_Library
 */
S1_API(const char*) s1_library_get_last_error_info (s1_Library* lib);

/**
 * Set library debug message handler.
 * \param lib Library to set error handler on.
 * \param handler Handler function receiving UTF-8 strings.
 * \param userContext User-defined context value.
 * \memberof s1_Library
 * \sa \ref debug_message_handlers
 */
S1_API(void) s1_library_set_debug_message_handler (
  s1_Library* lib, s1_debug_message_handler_func handler, uintptr_t userContext);
/**
 * Set library debug message handler.
 * \param lib Library to set error handler on.
 * \param handler Handler function receiving wide strings.
 * \param userContext User-defined context value.
 * \memberof s1_Library
 * \sa \ref debug_message_handlers
 */
S1_API(void) s1_library_set_debug_message_handler_wcs (
  s1_Library* lib, s1_debug_message_handler_wcs_func handler, uintptr_t userContext);

/**
 * Obtain UTF-8 version of library debug message handler.
 * \param lib Library to get error handler from.
 * \param userContextPtr Optional argument to receive "user context" value.
 * \returns UTF-8 version of global debug message handler, or \NULL if none set
 *   (this includes a wide string version being set).
 * \memberof s1_Library
 * \sa \ref debug_message_handlers
 */
S1_API(s1_debug_message_handler_func) s1_library_get_debug_message_handler (
  s1_Library* lib, uintptr_t* userContextPtr S1_ARG_DEFAULT (S1_NULL));
/**
 * Obtain wide string version of global debug message handler.
 * \param lib Library to get error handler from.
 * \param userContextPtr Optional argument to receive "user context" value.
 * \returns wide string version of global debug message handler, or \NULL if none set
 *   (this includes an UTF-8 version being set).
 * \memberof s1_Library
 * \sa \ref debug_message_handlers
 */
S1_API(s1_debug_message_handler_wcs_func) s1_library_get_debug_message_handler_wcs (
  s1_Library* lib, uintptr_t* userContextPtr S1_ARG_DEFAULT (S1_NULL));


S1TYPE_DECLARE_FWD(Options);
/**
 * Create a compiler options objects.
 * \param lib Parent library.
 * \returns The new compiler options objects.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 * \memberof s1_Library
 */
S1_API(s1_Options*) s1_options_create (s1_Library* lib);

/**\page compat_level Program compatibility levels
 * Compability levels are for dealing with breaking changes as the
 * Shader1 language grows and evolves over time. 
 * As new features are added &ndash; or existing features receive
 * bug fixes &ndash; there's always the possibility that compatibility
 * issues arise. For example, a new language feature introduces a new
 * reserved keyword, breaking programs using the same token as a regular
 * identifier. Or a bug fix might cause semantics to change, but existing
 * third-party users have come to rely on the &ldquo;broken&rdquo; semantics
 * (and consequently, the fix does the reverse for them).
 * 
 * To address these issues, a &ldquo;compatibility level&rdquo; can be
 * provided when creating a program object. This compatibility level
 * controls available language features and semantics (e.g. a feature
 * introducing a new reserved keyword is not made available, making
 * it possible to continue using the keyword for other purposes).
 * 
 * The default compatibility level #S1_COMPATIBILITY_LATEST enables
 * features and behaviour supported by the version of Shader1 you're
 * currently building against. That means if you build against an old
 * version, but use a newer library version at runtime, you will still
 * obtain the behaviour you're expecting from the old version.
 * Only if \em building against a newer version will make the
 * new features available. (Conversely, if you build against a newer
 * version of Shader1 but need legacy behaviour, you must specify this
 * explicitly).
 */

/**\def S1_COMPATIBILITY_LATEST
 * \hideinitializer
 * Compability level: use highest level supported by the Shader1 version currently
 * building against.
 * \sa \ref compat_level
 */
#define S1_COMPATIBILITY_LATEST         0

S1TYPE_DECLARE_FWD(Program);
/**
 * Create a program object from a string.
 * \param lib Parent library.
 * \param source Program source code. Must be encoded in UTF-8.
 * \param sourceSize Program source code size.
 * \param compatLevel Program compatibility level.
 *    If not sure use #S1_COMPATIBILITY_LATEST here. \sa \ref compat_level
 * \returns A new program object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 * \memberof s1_Library
 */
S1_API(s1_Program*) s1_program_create_from_string (s1_Library* lib, const char* source,
                                                  size_t sourceSize,
                                                  unsigned int compatLevel S1_ARG_DEFAULT(S1_COMPATIBILITY_LATEST));

/**
 * Input stream function pointer.
 * \param userContext User context value provided along with the function pointer.
 * \param data Outputs pointer to the next block of available data.
 * \returns Size of the block returned in \a data. Return 0 if no other data is
 *   available.
 */
typedef size_t (*s1_stream_input_func)(uintptr_t userContext, const char** data);

S1TYPE_DECLARE_FWD(ByteStream);
/**
 * Create a program object from a stream.
 * \param lib Parent library.
 * \param stream Pointer to stream with source code. Source must be encoded in UTF-8.
 *    Create a stream using s1_byte_stream_create_from_data() or
 *    s1_byte_stream_create_from_callback().
 * \param compatLevel Program compatibility level.
 *    If not sure use #S1_COMPATIBILITY_LATEST here. \sa \ref compat_level
 * \returns A new program object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 * \warning If you implement \a streamFunc in C++ you should handle all exceptions!
 * \memberof s1_Library
 */
S1_API(s1_Program*) s1_program_create_from_stream (s1_Library* lib, s1_ByteStream* stream,
                                                   unsigned int compatLevel S1_ARG_DEFAULT(S1_COMPATIBILITY_LATEST));

/**
 * Create a program object from a stream.
 * \param lib Parent library.
 * \param streamFunc Function providing streaming input of source code. Source must be encoded in UTF-8.
 * \param userContext Context information for streaming input function.
 * \param compatLevel Program compatibility level.
 *    If not sure use #S1_COMPATIBILITY_LATEST here. \sa \ref compat_level
 * \returns A new program object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 * \warning If you implement \a streamFunc in C++ you should handle all exceptions!
 * \memberof s1_Library
 */
S1_API(s1_Program*) s1_program_create_from_stream_func (s1_Library* lib, s1_stream_input_func streamFunc,
                                                   uintptr_t userContext,
                                                   unsigned int compatLevel S1_ARG_DEFAULT(S1_COMPATIBILITY_LATEST));

S1TYPE_DECLARE_FWD(Backend);
/**
 * Create a backend object.
 * \param lib Parent library.
 * \param backend Name of the backend to create.
 *   Currently only <tt>"glsl"</tt> is supported.
 * \returns A new backend object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 * \memberof s1_Library
 */
S1_API(s1_Backend*) s1_backend_create (s1_Library* lib, s1_StringArg backend);

S1TYPE_DECLARE_FWD(String);
//@{
/**
 * Create a string object.
 * \param lib Parent library.
 * \param string Contents of the string. Passing \NULL will result in an error.
 * \param invalidPos Optionally returns position of the (first) invalid input.
 * \returns A new string object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of a decoding error a valid String object will be returned; however,
 * place holder characters will be present in the string. \a invalidPos will point
 * to the position of the (first) invalid input and an appropriate error
 * status is saved in the library's last error code.
 * In case of other errors, \NULL is returned and the error status is saved in
 * the library's last error code.
 * \memberof s1_Library
 */
S1_API(s1_String*) s1_string_create_u8 (s1_Library* lib, const char* string,
                                        const char** invalidPos S1_ARG_DEFAULT(S1_NULL));
S1_API(s1_String*) s1_string_create_wcs (s1_Library* lib, const wchar_t* string,
                                         const wchar_t** invalidPos S1_ARG_DEFAULT (S1_NULL));
S1_API(s1_String*) s1_string_create_u16 (s1_Library* lib, const s1_char16* string,
                                         const s1_char16** invalidPos S1_ARG_DEFAULT (S1_NULL));
S1_API(s1_String*) s1_string_create_u32 (s1_Library* lib, const s1_char32* string,
                                         const s1_char32** invalidPos S1_ARG_DEFAULT (S1_NULL));
//@}

/**
 * Create a string object.
 * \param lib Parent library.
 * \param string Contents of the string. Passing \NULL will result in an error.
 * \param invalidPos Optionally returns position of the (first) invalid input,
 *   in code units of the input string.
 * \returns A new string object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of a decoding error a valid String object will be returned; however,
 * place holder characters will be present in the string. \a invalidPos will point
 * to the position of the (first) invalid input and an appropriate error
 * status is saved in the library's last error code.
 * In case of other errors, \NULL is returned and the error status is saved in
 * the library's last error code.
 * \memberof s1_Library
 */
S1_API(s1_String*) s1_string_create (s1_Library* lib, s1_StringArg string,
                                     size_t* invalidPos S1_ARG_DEFAULT(S1_NULL));

/**
 * Byte stream cleanup function pointer.
 * \param cleanupContext Argument of the same name passed to s1_byte_stream_create_from_data().
 * \param data Pointer to data as passed to s1_byte_stream_create_from_data().
 */
typedef void (*s1_byte_stream_data_cleanup_func)(uintptr_t cleanupContext, const char* data);

/**
 * Create a byte stream object from raw data.
 * \param lib Parent library.
 * \param data Pointer to the data.
 * \param dataSize Size of the data.
 * \param cleanupFunc Optional function that is called when the byte stream data can be cleaned
 *   up.
 * \param cleanupContext Optional argument to cleanup function.
 * \remarks The buffer pointed to by \a data must be valid for the lifetime of the byte stream!
 * \memberof s1_Library
 */
S1_API(s1_ByteStream*) s1_byte_stream_create_from_data (s1_Library* lib, const char* data,
                                                        size_t dataSize,
                                                        s1_byte_stream_data_cleanup_func cleanupFunc S1_ARG_DEFAULT(S1_NULL),
                                                        uintptr_t cleanupContext S1_ARG_DEFAULT(0));

/**
 * Byte stream data streaming function pointer.
 * \param userContext User context value provided to s1_byte_stream_create_from_callback.
 * \param data Outputs pointer to the next block of available data.
 * \returns Size of the block returned in \a data. Return 0 if no other data is
 *   available.
 */
typedef size_t (*s1_byte_stream_data_func)(uintptr_t userContext, const char** data);
/**
 * Byte stream cleanup function pointer.
 * \param userContext Argument of the same name passed to s1_byte_stream_create_from_callback().
 */
typedef void (*s1_byte_stream_cleanup_func)(uintptr_t userContext);
/**
 * Byte stream "create restarted" function pointer.
 * \param userContext User context value provided to s1_byte_stream_create_from_callback.
 * \param stream Outputs pointer to the new s1_ByteStream instance that represents the
 *   same stream, but restarted.
 * \returns A return code to be set after s1_byte_stream_create_restarted().
 */
typedef s1_ResultCode (*s1_byte_stream_create_restarted_func)(uintptr_t userContext, s1_ByteStream** stream);

/**
 * Create a byte stream object with callback functions providing the data.
 * \param lib Parent library.
 * \param userContext Context information for streaming start function.
 * \param dataFunc Function to get next streamed data.
 * \param cleanupFunc Optional function that is called when the byte stream can be cleaned up.
 * \param createRestartedFunc Optional function that is called when a restarted copy of the
 *   stream is requested.
 * \param size Optional argument with the value returned by s1_byte_stream_size().
 * \memberof s1_Library
 */
S1_API(s1_ByteStream*) s1_byte_stream_create_from_callback (s1_Library* lib,
                                                            uintptr_t userContext,
                                                            s1_byte_stream_data_func dataFunc,
                                                            s1_byte_stream_cleanup_func cleanupFunc S1_ARG_DEFAULT(S1_NULL),
                                                            s1_byte_stream_create_restarted_func createRestartedFunc S1_ARG_DEFAULT(S1_NULL),
                                                            size_t size S1_ARG_DEFAULT(~(size_t)0));

#if defined(__cplusplus)
#include "s1/cxxapi_detail.h"
#include <assert.h>

namespace s1
{
  S1_NS_CXXAPI_BEGIN
    /**
     * Library object, generally serves as a factory
     * for all available object types.t
     * object that needs to be called in order to use the Shader1 API.
     * Since libraries create all other object types it's the firs
     * \createdby s1::Library::Create()
     */
    class Library : public Object
    {
    protected:
      template<typename StreamFunc>
      static size_t StreamFuncWrapper (uintptr_t funcObj, const char** data)
      {
        try
        {
          return (*reinterpret_cast<StreamFunc*> (funcObj))(*data);
        }
        _S1_DEFAULT_CATCH_UNHANDLED_EXCEPTIONS("stream callback")
        return 0;
      }

      template<typename CleanupFunc>
      static void CleanupDataFuncWrapper (uintptr_t funcObj, const char* data)
      {
        try
        {
          CleanupFunc* cleanup = reinterpret_cast<CleanupFunc*> (funcObj);
          (*cleanup)(data);
          delete cleanup;
          return;
        }
        _S1_DEFAULT_CATCH_UNHANDLED_EXCEPTIONS("data stream cleanup")
      }

      template<typename DataFunc, typename CleanupFunc, typename CreateRestartedFunc>
      struct ByteStreamCallbacks
      {
        DataFunc data;
        CleanupFunc cleanup;
        CreateRestartedFunc createRestarted;

      #if defined(S1_HAVE_RVALUES)
        ByteStreamCallbacks (DataFunc&& data, CleanupFunc&& cleanup, CreateRestartedFunc&& createRestarted)
          : data (std::move (data)), cleanup (std::move (cleanup)), createRestarted (std::move (createRestarted))
        {}
      #else
        ByteStreamCallbacks (DataFunc data, CleanupFunc cleanup, CreateRestartedFunc createRestarted)
          : data (data), cleanup (cleanup), createRestarted (createRestarted)
        {}
      #endif

        static size_t Data (uintptr_t callbacks, const char** data)
        {
          try
          {
            return (reinterpret_cast<ByteStreamCallbacks*> (callbacks))->data (*data);
          }
          _S1_DEFAULT_CATCH_UNHANDLED_EXCEPTIONS("callback stream data")
          return 0;
        }
        static void Cleanup (uintptr_t callbacks)
        {
          try
          {
            ByteStreamCallbacks* callbacks_p = reinterpret_cast<ByteStreamCallbacks*> (callbacks);
            callbacks_p->cleanup ();
            delete callbacks_p;
            return;
          }
          _S1_DEFAULT_CATCH_UNHANDLED_EXCEPTIONS("callback stream cleanup")
        }
        static s1_ResultCode CreateRestarted (uintptr_t callbacks, ByteStream** restartedStream)
        {
          try
          {
            return (reinterpret_cast<ByteStreamCallbacks*> (callbacks))->createRestarted (*restartedStream);
          }
          _S1_DEFAULT_CATCH_UNHANDLED_EXCEPTIONS ("callback stream create restarted")
          return S1_E_STREAM_NOT_RESTARTABLE;
        }
      };

      template<typename T> static uintptr_t WrapperArg (T& arg)
      { return reinterpret_cast<uintptr_t> (&arg); }
      template<typename T> static uintptr_t WrapperArg (T* arg)
      { return reinterpret_cast<uintptr_t> (arg); }
    public:
      /// Smart pointer class for Library instances.
      typedef Ptr<Library> Pointer;

      /**
       * Create a library object. A library object generally serves as a factory
       * for all available object types.
       * \param lib Reference to a smart pointer receiving the new library object.
       *   Receives \NULL in case of error.
       * \returns If successful, the return code fulfills #S1_SUCCESSFUL.
       *   Otherwise, the return code is an error code indicating why the
       *   library could not be created.
       */
      static ResultCode Create (Pointer& lib)
      {
        s1_Library* p (0);
        ResultCode err = static_cast<ResultCode> (s1_create_library (&p));
        lib.take (p);
        return err;
      }
      
      /**
       * Return the last error code set by a function.
      */
      s1::ResultCode GetLastError()
      {
        return static_cast<ResultCode> (s1_library_get_last_error (this));
      }
      /**
       * Reset the last error code (to #S1_SUCCESS).
       */
      void ClearLastError ()
      {
        s1_library_clear_last_error (this);
      }
      /**
       * Return additional information for the last error code set by a function.
       * In some cases, additional information for an error result is available.
       * This function will return any such information, or \NULL if none is
       * available.
       * String is only guaranteed valid until the next S1 function call.
       */
      const char* GetLastErrorInfo ()
      {
        return s1_library_get_last_error_info (this);
      }

      /**
       * Set library debug message handler.
       * \param handler Handler function receiving UTF-8 strings.
       * \param userContext User-defined context value.
       * \sa \ref debug_message_handlers
       */
      void SetDebugMessageHandler (s1_debug_message_handler_func handler, uintptr_t userContext)
      {
        return s1_library_set_debug_message_handler (this, handler, userContext);
      }
      /**
       * Set library debug message handler.
       * \param handler Handler function receiving wide strings.
       * \param userContext User-defined context value.
       * \sa \ref debug_message_handlers
       */
      void SetDebugMessageHandlerWCS (s1_debug_message_handler_wcs_func handler, uintptr_t userContext)
      {
        return s1_library_set_debug_message_handler_wcs (this, handler, userContext);
      }

      /**
       * Obtain UTF-8 version of library debug message handler.
       * \returns UTF-8 version of global debug message handler, or \NULL if none set
       *   (this includes a wide string version being set).
       * \sa \ref debug_message_handlers
       */
      s1_debug_message_handler_func GetDebugMessageHandler ()
      {
        return s1_library_get_debug_message_handler (this);
      }
      /**
      * Obtain UTF-8 version of library debug message handler.
      * \param userContext Argument to receive "user context" value.
      * \returns UTF-8 version of global debug message handler, or \NULL if none set
      *   (this includes a wide string version being set).
       * \sa \ref debug_message_handlers
      */
      s1_debug_message_handler_func GetDebugMessageHandler (uintptr_t& userContext)
      {
        return s1_library_get_debug_message_handler (this, &userContext);
      }

      /**
       * Obtain wide string version of global debug message handler.
       * \returns wide string version of global debug message handler, or \NULL if none set
       *   (this includes an UTF-8 version being set).
       * \sa \ref debug_message_handlers
       */
      s1_debug_message_handler_wcs_func GetDebugMessageHandlerWCS ()
      {
        return s1_library_get_debug_message_handler_wcs (this);
      }
      /**
       * Obtain wide string version of global debug message handler.
       * \param userContext Argument to receive "user context" value.
       * \returns wide string version of global debug message handler, or \NULL if none set
       *   (this includes an UTF-8 version being set).
       * \sa \ref debug_message_handlers
       */
      s1_debug_message_handler_wcs_func GetDebugMessageHandlerWCS (uintptr_t& userContext)
      {
        return s1_library_get_debug_message_handler_wcs (this, &userContext);
      }

      /**
       * Create a compiler options objects.
       * \returns The new compiler options objects.
       * In case of an error, \NULL is returned and the error status is saved in the library's
       * last error code.
      */
      S1_RETURN_MOVE_REF_TYPE(Options) CreateOptions ()
      {
        return S1_RETURN_MOVE_REF(Options, s1_options_create (this));
      }
      
      /**
       * Create a program object from a string.
       * \param source Program source code. Must be encoded in UTF-8.
       * \param sourceSize Program source code size.
       * \param compatLevel Program compatibility level.
       *    If not sure use #S1_COMPATIBILITY_LATEST here. \sa \ref compat_level
       * \returns A new program object.
       * In case of an error, \NULL is returned and the error status is saved in the library's
       * last error code.
       */
      S1_RETURN_MOVE_REF_TYPE(Program) CreateProgramFromString (const char* source,
                                                                    size_t sourceSize,
                                                                    unsigned int compatLevel = S1_COMPATIBILITY_LATEST)
      {
        return S1_RETURN_MOVE_REF(Program,
          s1_program_create_from_string (this, source, sourceSize, compatLevel));
      }

      /**
       * Create a program object from a stream.
       * \param stream Pointer to stream with source code. Source must be encoded in UTF-8.
       *    Create a stream using CreateByteStreamFromData() or CreateByteStreamFromCallback().
       * \param compatLevel Program compatibility level.
       *    If not sure use #S1_COMPATIBILITY_LATEST here. \sa \ref compat_level
       * \returns A new program object.
       * In case of an error, \NULL is returned and the error status is saved in the library's
       * last error code.
       * \warning You should handle all exceptions in streamFunc!
       */
      S1_RETURN_MOVE_REF_TYPE(Program) CreateProgramFromStream (ByteStream* stream,
                                                                unsigned int compatLevel = S1_COMPATIBILITY_LATEST)
      {
        return S1_RETURN_MOVE_REF(Program,
                                  s1_program_create_from_stream (this, stream, compatLevel));
      }

      /**
       * Create a program object from a stream.
       * \param streamFunc Function providing streaming input of source code. Source must be encoded in UTF-8.
       * \param userContext Context information for streaming input function.
       * \param compatLevel Program compatibility level.
       *    If not sure use #S1_COMPATIBILITY_LATEST here. \sa \ref compat_level
       * \returns A new program object.
       * In case of an error, \NULL is returned and the error status is saved in the library's
       * last error code.
       * \warning You should handle all exceptions in streamFunc!
       */
      S1_RETURN_MOVE_REF_TYPE(Program) CreateProgramFromStreamFunc (s1_stream_input_func streamFunc,
                                                                uintptr_t userContext,
                                                                unsigned int compatLevel = S1_COMPATIBILITY_LATEST)
      {
        return S1_RETURN_MOVE_REF(Program,
                                  s1_program_create_from_stream_func (this, streamFunc, userContext, compatLevel));
      }

      /**
       * Create a program object from a stream.
       * \param streamFunc Functor providing streaming input of source code. Source must be encoded in UTF-8.
       *   The functor must provide an <tt>size_t operator()(const char*&)</tt>, writing a pointer to the
       *   next block of data into the reference argument and returning the size of that data, or 0 if
       *   no additional data is available.
       * \param compatLevel Program compatibility level.
       *    If not sure use #S1_COMPATIBILITY_LATEST here. \sa \ref compat_level
       * \returns A new program object.
       * In case of an error, \NULL is returned and the error status is saved in the library's
       * last error code.
       */
      template<typename StreamFunc>
      S1_RETURN_MOVE_REF_TYPE(Program) CreateProgramFromStreamFunc (StreamFunc streamFunc,
                                                                unsigned int compatLevel = S1_COMPATIBILITY_LATEST)
      {
        return S1_RETURN_MOVE_REF(Program,
                                  s1_program_create_from_stream_func (this,
                                                                 &StreamFuncWrapper<typename detail::remove_pointer<StreamFunc>::type>,
                                                                 WrapperArg (streamFunc), compatLevel));
      }

      /**
       * Create a backend object.
       * \param backend Name of the backend to create.
       *   Currently only <tt>"glsl"</tt> is supported.
       * \returns A new backend object.
       * In case of an error, \NULL is returned and the error status is saved in the library's
       * last error code.
       */
      S1_RETURN_MOVE_REF_TYPE(Backend) CreateBackend (StringArg backend)
      {
        return S1_RETURN_MOVE_REF(Backend,
          s1_backend_create (this, backend));
      }

      //@{
      /** Create a string object.
       * \param string Contents of the string. Passing \NULL will result in an error.
       * \returns A new string object.
       * In case of a decoding error a valid String object will be returned; however,
       * place holder characters will be present in the string. An appropriate error
       * status is saved in the library's last error code.
       * In case of other errors, \NULL is returned and the error status is saved in
       * the library's last error code.
       */
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (const char* string)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create_u8 (this, string));
      }
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (const wchar_t* string)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create_wcs (this, string));
      }
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (const s1_char16* string)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create_u16 (this, string));
      }
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (const s1_char32* string)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create_u32 (this, string));
      }
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (StringArg string)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create (this, string));
      }
      //@}
      //@{
      /** Create a string object.
       * \param string Contents of the string. Passing \NULL will result in an error.
       * \param invalidPos Returns position of the (first) invalid input, if any.
       * \returns A new string object.
       * In case of a decoding error a valid String object will be returned; however,
       * place holder characters will be present in the string. \a invalidPos will point
       * to the position of the (first) invalid input and an appropriate error
       * status is saved in the library's last error code.
       * In case of other errors, \NULL is returned and the error status is saved in
       * the library's last error code.
       */
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (const char* string, const char*& invalidPos)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create_u8 (this, string, &invalidPos));
      }
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (const wchar_t* string, const wchar_t*& invalidPos)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create_wcs (this, string, &invalidPos));
      }
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (const s1_char16* string, const s1_char16*& invalidPos)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create_u16 (this, string, &invalidPos));
      }
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (const s1_char32* string, const s1_char32*& invalidPos)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create_u32 (this, string, &invalidPos));
      }
      //@}
      /**
       * Create a string object.
       * \param string Contents of the string. Passing \NULL will result in an error.
       * \param invalidPos Returns position of the (first) invalid input, if any,
       *   in code units of the input string.
       * \returns A new string object.
       * In case of a decoding error a valid String object will be returned; however,
       * place holder characters will be present in the string. \a invalidPos will point
       * to the position of the (first) invalid input and an appropriate error
       * status is saved in the library's last error code.
       * In case of other errors, \NULL is returned and the error status is saved in
       * the library's last error code.
       */
      S1_RETURN_MOVE_REF_TYPE(String) CreateString (StringArg string, size_t& invalidPos)
      {
        return S1_RETURN_MOVE_REF(String,
                                  s1_string_create (this, string, &invalidPos));
      }

      /**
       * Create a byte stream object from raw data.
       * \param data Pointer to the data.
       * \param dataSize Size of the data.
       * \param cleanupFunc Optional function that is called when the byte stream data can be
       *   cleaned up.
       * \param cleanupContext Optional argument to cleanup function.
       * \remarks The buffer pointed to by \a data must be valid for the lifetime of the byte
       *   stream!
       */
      S1_RETURN_MOVE_REF_TYPE(ByteStream) CreateByteStreamFromData (const char* data,
                                                                    size_t dataSize,
                                                                    s1_byte_stream_data_cleanup_func cleanupFunc = S1_NULL,
                                                                    uintptr_t cleanupContext = 0)
      {
        return S1_RETURN_MOVE_REF(ByteStream,
                                  s1_byte_stream_create_from_data (this, data, dataSize, cleanupFunc, cleanupContext));
      }
      /**
       * Create a byte stream object from raw data.
       * \param data Pointer to the data.
       * \param dataSize Size of the data.
       * \param cleanupFunc Functor to clean up the streamed data.
       *   The functor must provide a <tt>void operator()(const char*)</tt>, cleaning up the
       *   data when the byte stream is destroyed.
       * \remarks The buffer pointed to by \a data must be valid for the lifetime of the byte
       *   stream!
       */
      template<typename CleanupFunc>
      S1_RETURN_MOVE_REF_TYPE(ByteStream) CreateByteStreamFromData (const char* data, size_t dataSize,
                                                                    CleanupFunc cleanupFunc)
      {
        // The functor persist in the object, so always make a copy
      #if defined(S1_HAVE_RVALUES)
        CleanupFunc* cleanup_p = new CleanupFunc (std::move (cleanupFunc));
      #else
        CleanupFunc* cleanup_p = new CleanupFunc (cleanupFunc);
      #endif
        return S1_RETURN_MOVE_REF(ByteStream,
                                  s1_byte_stream_create_from_data (this, data, dataSize,
                                                                   &CleanupDataFuncWrapper<CleanupFunc>,
                                                                   reinterpret_cast<uintptr_t> (cleanup_p)));
      }

      /**
       * Create a byte stream object with callback functions providing the data.
       * \param userContext Context information for streaming start function.
       * \param dataFunc Function to get next streamed data.
       * \param cleanupFunc Optional function that is called when the byte stream can be
       *   cleaned up.
       * \param createRestartedFunc Optional function that is called when a restarted copy of
       *   the stream is requested.
       * \param size Optional argument with the value returned by s1_byte_stream_size().
       */
      S1_RETURN_MOVE_REF_TYPE(ByteStream) CreateByteStreamFromCallback (uintptr_t userContext,
                                                                        s1_byte_stream_data_func dataFunc,
                                                                        s1_byte_stream_cleanup_func cleanupFunc = S1_NULL,
                                                                        s1_byte_stream_create_restarted_func createRestartedFunc = S1_NULL,
                                                                        size_t size = ~(size_t)0)
      {
        return S1_RETURN_MOVE_REF(ByteStream,
                                  s1_byte_stream_create_from_callback (this, userContext,
                                                                       dataFunc,
                                                                       cleanupFunc,
                                                                       createRestartedFunc,
                                                                       size));
      }

      /**
       * Create a byte stream object with callback functions providing the data.
       * \param dataFunc Functor providing stream data.
       *   The functor must provide an <tt>size_t operator()(const char*&)</tt>, writing a
       *   pointer to the next block of data into the reference argument and returning the
       *   size of that data, or 0 if no additional data is available.
       * \param size Optional argument with the value returned by s1_byte_stream_size().
       */
      template<typename DataFunc>
      S1_RETURN_MOVE_REF_TYPE(ByteStream) CreateByteStreamFromCallback (DataFunc dataFunc,
                                                                        size_t size = ~(size_t)0)
      {
        return S1_RETURN_MOVE_REF(ByteStream,
                                  s1_byte_stream_create_from_callback (this,
                                                                       WrapperArg (dataFunc),
                                                                       &StreamFuncWrapper<typename detail::remove_pointer<DataFunc>::type>,
                                                                       nullptr,
                                                                       nullptr,
                                                                       size));
      }

      /**
       * Create a byte stream object with callback functions providing the data.
       * \param dataFunc Functor providing stream data.
       *   The functor must provide an <tt>size_t operator()(const char*&)</tt>, writing a
       *   pointer to the next block of data into the reference argument and returning the
       *   size of that data, or 0 if no additional data is available.
       * \param cleanupFunc Functor providing cleanup.
       *   The functor must provide an <tt>void operator()()</tt> that performs any required
       *   cleanup of the streamed data.
       * \param size Optional argument with the value returned by s1_byte_stream_size().
       */
      template<typename DataFunc, typename CleanupFunc>
      S1_RETURN_MOVE_REF_TYPE(ByteStream) CreateByteStreamFromCallback (DataFunc dataFunc,
                                                                        CleanupFunc cleanupFunc,
                                                                        size_t size = ~(size_t)0)
      {
        typedef ByteStreamCallbacks<DataFunc, CleanupFunc, s1_ResultCode (*)(s1_ByteStream** stream)> Callbacks;
        // The functors persist in the object, so always make copies
        Callbacks* callbacks = new Callbacks (
        #if defined(S1_HAVE_RVALUES)
          std::move (dataFunc), std::move (cleanupFunc), nullptr
        #else
          dataFunc, cleanupFunc, S1_NULL
        #endif
          );
        return S1_RETURN_MOVE_REF(ByteStream,
                                  s1_byte_stream_create_from_callback (this, reinterpret_cast<uintptr_t> (callbacks),
                                                                       &Callbacks::Data,
                                                                       &Callbacks::Cleanup,
                                                                       nullptr,
                                                                       size));
      }

      /**
       * Create a byte stream object with callback functions providing the data.
       * \param dataFunc Functor providing stream data.
       *   The functor must provide an <tt>size_t operator()(const char*&)</tt>, writing a
       *   pointer to the next block of data into the reference argument and returning the
       *   size of that data, or 0 if no additional data is available.
       * \param cleanupFunc Functor providing cleanup.
       *   The functor must provide an <tt>void operator()()</tt> that performs any required
       *   cleanup of the streamed data.
       * \param createRestartedFunc Functor providing a restarted copy of the stream.
       *   The functor must provide an <tt>s1_ResultCode operator()(ByteStream*&)</tt>, writing
       *   a pointer to new ByteStream instance that allows reading the data from the start and
       *   returning a return code to be set after ByteStream::CreateRestarted().
       * \param size Optional argument with the value returned by s1_byte_stream_size().
       */
      template<typename DataFunc, typename CleanupFunc, typename CreateRestartedFunc>
      S1_RETURN_MOVE_REF_TYPE(ByteStream) CreateByteStreamFromCallback (DataFunc dataFunc,
                                                                        CleanupFunc cleanupFunc,
                                                                        CreateRestartedFunc createRestartedFunc,
                                                                        size_t size = ~(size_t)0)
      {
        typedef ByteStreamCallbacks<DataFunc, CleanupFunc, CreateRestartedFunc> Callbacks;
        // The functors persist in the object, so always make copies
        Callbacks* callbacks = new Callbacks (
        #if defined(S1_HAVE_RVALUES)
          std::move (dataFunc), std::move (cleanupFunc), std::move (createRestartedFunc)
        #else
          dataFunc, cleanupFunc, createRestartedFunc
        #endif
          );
        return S1_RETURN_MOVE_REF(ByteStream,
                                  s1_byte_stream_create_from_callback (this, reinterpret_cast<uintptr_t> (callbacks),
                                                                       &Callbacks::Data,
                                                                       &Callbacks::Cleanup,
                                                                       &Callbacks::CreateRestarted,
                                                                       size));
      }
    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::Library Library;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_LIBRARY_H__
