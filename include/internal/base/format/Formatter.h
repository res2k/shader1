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
#include <boost/scoped_ptr.hpp>

#include <mutex>
#include <vector>

#include "FormatStringTraits.h"

#define FORMATTER_MAX_ARGS      BOOST_PP_INC(12) /* arbitrary, increase as necessary */

namespace s1
{
  namespace format
  {
    /**
     * Simple string formatter.
     * Takes format strings of type \a FormatStringType.
     * Placeholder style is \c {N} where \c N is the zero-based argument number.
     */
    template<typename FormatStringType = const char*>
    class Formatter
    {
    protected:
      typedef typename FormatStringTraits<FormatStringType>::CharType CharType;

      // A parsed format string part.
      struct FormatPart
      {
      private:
        /// Begin of format string part, or \c nullptr if index
        const CharType* strBegin;
        union
        {
          /// String part len
          size_t strLen;
          /// Argument index
          size_t argIndex;
        };
      public:
        FormatPart (const CharType* str, size_t len)
          : strBegin (str), strLen (len) {}
        FormatPart (size_t index)
          : strBegin (nullptr), argIndex (index) {}

        bool IsStringPart() const;
        const CharType* GetPartString() const;
        size_t GetPartStringLen() const;
        size_t GetArgIndex() const;
      };

      std::vector<FormatPart> parts;
      template<typename FormatIt>
      void ParseFormat (FormatIt fmtBegin, FormatIt fmtEnd);

      template<typename SinkType>
      void EmitPartString (SinkType& sink, const FormatPart& part) const;
    public:
      /**
       * Construct a formatter.
       * \param format Format string. Must be constant over the lifetime of the formatter!
       */
      Formatter (FormatStringType format);

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
        return std::move (dest);
      }
    };

    /**
     * Helper to declare a static formatter instance (for repeated use).
     */
    template<const char* const Format>
    class StaticFormatter
    {
      std::once_flag formatterInit;
      boost::scoped_ptr<Formatter<> > formatter;

      static void NewFormatter (StaticFormatter<Format>* this_, const char* format)
      {
        this_->formatter.reset (new Formatter<> (format));
        this_->formatter->CompactParsedFormat ();
      }

      const Formatter<>& GetFormatter (const char* format)
      {
        std::call_once (formatterInit, &NewFormatter, this, format);
        return *formatter;
      }
    public:
      /**
      * Actual formatting.
      * \param dest Destination string.
      * \param a Format arguments. Must have at least as many arguments as placeholders in
      *   the format string were used!
      */
      template<typename DestType, typename ...Args>
      void operator() (DestType& dest, const Args&... a)
      {
        GetFormatter (Format) (dest, a...);
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
        return GetFormatter (Format).to<DestType> (a...);
      }
    };
  } // namespace format

  /// Declare a StaticFormatter instance
  #define DECLARE_STATIC_FORMATTER(Name, Format)                                              \
    extern const char BOOST_PP_CAT(BOOST_PP_CAT(Name, _fmtstr), __LINE__)[] = Format;         \
    static format::StaticFormatter<BOOST_PP_CAT(BOOST_PP_CAT(Name, _fmtstr), __LINE__)> Name
} // namespace s1

#endif // __BASE_FORMAT_FORMATTER_H__
