/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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
 * Simple string formatter.
 */
#ifndef __BASE_FORMAT_FORMATTER_H__
#define __BASE_FORMAT_FORMATTER_H__

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/inc.hpp>

#include <atomic>
#include <vector>

#include "../string_view.h"

#define FORMATTER_MAX_ARGS      BOOST_PP_INC(12) /* arbitrary, increase as necessary */

namespace s1
{
  namespace format
  {
    /**
     * Simple string formatter.
     * Takes format strings of type const char*.
     * Placeholder style is \c {N} where \c N is the zero-based argument number.
     */
    class Formatter
    {
    protected:
      // A parsed format string part.
      struct FormatPart
      {
      private:
        /// Begin of format string part, or \c nullptr if index
        const char* strBegin;
        union
        {
          /// String part len
          size_t strLen;
          /// Argument index
          size_t argIndex;
        };
      public:
        FormatPart (const char* str, size_t len)
          : strBegin (str), strLen (len) {}
        FormatPart (size_t index)
          : strBegin (nullptr), argIndex (index) {}

        bool IsStringPart() const;
        const char* GetPartString() const;
        size_t GetPartStringLen() const;
        size_t GetArgIndex() const;
      };

      std::vector<FormatPart> parts;
      void ParseFormat (string_view format);

      template<typename SinkType>
      void EmitPartString (SinkType& sink, const FormatPart& part) const;
    public:
      /**
       * Construct a formatter.
       * \param format Format string. Must be constant over the lifetime of the formatter!
       */
      Formatter (string_view format);

      /**
       * Helper function to reduce the storage of the internal parsed format representation.
       * Useful if the format is retained for multiple use.
       */
      void CompactParsedFormat () { parts.shrink_to_fit (); }

      /**
       * Actual formatting.
       * \param dest Destination string.
       * \param a Format arguments. Must have at least as many arguments as placeholders in
       *   the format string were used!
       */
      template<typename DestType, typename ...Args>
      void operator() (DestType& dest, const Args&... a) const;

      /**
       * Actual formatting.
       * \param a Format arguments. Must have at least as many arguments as placeholders in
       *   the format string were used!
       * \returns Destination string.
       */
      template<typename DestType, typename ...Args>
      DestType to (const Args&... a) const
      {
        DestType dest;
        operator() (dest, a...);
        return dest;
      }
    };

    /**
     * Helper to declare a static formatter instance (for repeated use).
     */
    template<const char* const Format>
    class StaticFormatter
    {
      std::atomic<Formatter*> formatter;

      const Formatter& GetFormatter (const char* format)
      {
        Formatter* fmtPtr = formatter.load (std::memory_order_acquire);
        if (!fmtPtr)
        {
          fmtPtr = new Formatter (format);
          fmtPtr->CompactParsedFormat ();
          Formatter* expectedPtr = nullptr;
          if (!formatter.compare_exchange_strong (expectedPtr, fmtPtr))
          {
            delete fmtPtr;
            fmtPtr = expectedPtr;
          }
        }
        return *fmtPtr;
      }
      const Formatter& GetFormatter ()
      {
        return GetFormatter (Format);
      }
    public:
      StaticFormatter () : formatter (nullptr) {}
      ~StaticFormatter ()
      {
        if (auto fmtPtr = formatter.load (std::memory_order_acquire))
          delete fmtPtr;
      }

      /**
      * Actual formatting.
      * \param dest Destination string.
      * \param a Format arguments. Must have at least as many arguments as placeholders in
      *   the format string were used!
      */
      template<typename DestType, typename ...Args>
      void operator() (DestType& dest, const Args&... a)
      {
        GetFormatter() (dest, a...);
      }

      /**
       * Actual formatting.
       * \param a Format arguments. Must have at least as many arguments as placeholders in
       *   the format string were used!
       * \returns Destination string.
       */
      template<typename DestType, typename ...Args>
      DestType to (const Args&... a)
      {
        return GetFormatter().template to<DestType> (a...);
      }
    };
  } // namespace format

  /// Declare a StaticFormatter instance
  #define DECLARE_STATIC_FORMATTER(Name, Format)                                              \
    extern const char BOOST_PP_CAT(BOOST_PP_CAT(Name, _fmtstr), __LINE__)[] = Format;         \
    static format::StaticFormatter<BOOST_PP_CAT(BOOST_PP_CAT(Name, _fmtstr), __LINE__)> Name
} // namespace s1

#endif // __BASE_FORMAT_FORMATTER_H__
