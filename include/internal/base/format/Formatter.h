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

#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>

#include <vector>

#include "FormatStringTraits.h"

#define FORMATTER_MAX_ARGS      BOOST_PP_INC(3) /* arbitrary, increase as necessary */

namespace s1
{
  namespace format
  {
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
      Formatter (FormatStringType format);

  #define _DECLARE_FORMATTER_OPERATOR(Z, ArgNum, Data)                         \
      template<typename DestType                                               \
        BOOST_PP_ENUM_TRAILING_PARAMS_Z(Z, ArgNum, typename A)>                \
      void operator() (DestType& dest                                          \
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(Z, ArgNum, const A, & a)) const;

      BOOST_PP_REPEAT(FORMATTER_MAX_ARGS, _DECLARE_FORMATTER_OPERATOR, _)

  #undef _DECLARE_FORMATTER_OPERATOR
    };
  } // namespace format
} // namespace s1

#endif // __BASE_FORMAT_FORMATTER_H__
