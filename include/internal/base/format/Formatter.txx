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
 * Simple string formatter (implementation).
 */
#ifndef __BASE_FORMAT_FORMATTER_TPP__
#define __BASE_FORMAT_FORMATTER_TPP__

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/type_traits/is_signed.hpp>

#include "Sink.h"

namespace s1
{
  namespace format
  {
    template<typename FormatStringType>
    bool Formatter<FormatStringType>::FormatPart::IsStringPart() const
    {
      return strBegin != nullptr;
    }

    template<typename FormatStringType>
    const typename Formatter<FormatStringType>::CharType* Formatter<FormatStringType>::FormatPart::GetPartString() const
    {
      assert (IsStringPart());
      return strBegin;
    }

    template<typename FormatStringType>
    size_t Formatter<FormatStringType>::FormatPart::GetPartStringLen() const
    {
      assert (IsStringPart());
      return strLen;
    }

    template<typename FormatStringType>
    size_t Formatter<FormatStringType>::FormatPart::GetArgIndex() const
    {
      assert (!IsStringPart());
      return argIndex;
    }

    //-----------------------------------------------------------------------

    template<typename FormatStringType>
    template<typename SinkType>
    void Formatter<FormatStringType>::EmitPartString (SinkType& sink, const FormatPart& part) const
    {
      sink.append (part.GetPartString(), part.GetPartStringLen());
    }

    namespace detail
    {
      // Generic version: leave undefined to catch missing specializations
      template<typename SinkType, typename ArgType>
      class ArgHelper;

      //
      // String specializations
      //

      // Direct copy to sink
      template<typename SinkType>
      class ArgHelper<SinkType, typename SinkType::DestType>
      {
      public:
        typedef typename SinkType::DestType ArgType;
        ArgHelper (const ArgType&) {}
        size_t FormattedSize (const ArgType& arg) const
        {
          return arg.size();
        }
        void Emit (SinkType& sink, const ArgType& arg)
        {
          sink.append (arg);
        }
      };

      // Constant string
      template<typename SinkType, size_t N>
      class ArgHelper<SinkType, typename SinkType::value_type [N]>
      {
      public:
        typedef typename SinkType::value_type ArgType[N];
        ArgHelper (const ArgType&) {}
        size_t FormattedSize (const ArgType&) const { return N-1; }
        void Emit (SinkType& sink, const ArgType& arg)
        {
          sink.append (arg, N - 1 /* excluding NUL */);
        }
      };

      template<typename SinkType, typename CharType, size_t N>
      class ArgHelper<SinkType, CharType[N]>
      {
      public:
        typedef CharType ArgType[N];
        ArgHelper (const ArgType&) {}
        size_t FormattedSize (const ArgType&) const { return N-1; }
        void Emit (SinkType& sink, const ArgType& arg)
        {
          for (size_t i = 0; i < N-1 /* excluding NUL */; i++)
          {
            /* No explicit cast -
             * compiler time warnings/runtime bounds check errors are desired */
            typename SinkType::value_type c = arg[i];
            sink.push_back (c);
          }
        }
      };

      // Pointer to string
      template<typename SinkType, typename CharType>
      class ArgHelper<SinkType, CharType const*>
      {
      public:
        typedef CharType const* ArgType;
        ArgHelper (const ArgType) {}
        size_t FormattedSize (ArgType arg) const
        {
          size_t len (0);
          while (*arg) { arg++; len++; }
          return len;
        }
        void Emit (SinkType& sink, ArgType arg)
        {
          while (*arg)
          {
            /* No explicit cast -
             * compiler time warnings/runtime bounds check errors are desired */
            typename SinkType::value_type c = *arg++;
            sink.push_back (c);
          }
        }
      };

      //
      // Integer specializations
      //

      template<typename T>
      struct IntDigitsHelper
      {
        enum
        {
          value = S1_APPROX_DIGITS(T) + boost::is_signed<T>::value
        };
      };
      template<typename T, bool IsSigned>
      struct MakeUnsignedHelper
      {
        static typename boost::make_unsigned<T>::type MakeUnsigned(T v, bool has_sign)
        {
          return has_sign ? -v : v;
        }
      };
      template<typename T>
      struct MakeUnsignedHelper<T, false>
      {
        static T MakeUnsigned(T v, bool)
        {
          return v;
        }
      };

      // General integer
      template<typename SinkType, typename T>
      class ArgHelperInt
      {
      public:
        typedef T ArgType;
        ArgHelperInt (const ArgType) {}
        size_t FormattedSize (ArgType arg) const { return IntDigitsHelper<T>::value; }
        void Emit (SinkType& sink, ArgType arg)
        {
          enum { num_digits = IntDigitsHelper<T>::value };
          typedef typename SinkType::value_type CharType;
          CharType buf[num_digits];
          CharType* buf_end = buf + num_digits;
          CharType* p = buf_end;

          bool has_sign (arg < 0);
          typename boost::make_unsigned<T>::type v = MakeUnsignedHelper<T, boost::is_signed<T>::value>::MakeUnsigned (arg, has_sign);
          do
          {
            --p;
            *p = '0' + (v % 10);
            v /= 10;
          }
          while ((v != 0) && (p != buf));

          if ((p > buf) && has_sign) { *--p = '-'; }

          sink.append (p, buf_end - p);
        }
      };
      // Signed integer
      template<typename SinkType>
      class ArgHelper<SinkType, int> : public ArgHelperInt<SinkType, int>
      {
        typedef ArgHelperInt<SinkType, int> Super;
      public:
        ArgHelper (int v) : Super (v) {}
      };
      // Unsigned integer
      template<typename SinkType>
      class ArgHelper<SinkType, unsigned int> : public ArgHelperInt<SinkType, unsigned int>
      {
        typedef ArgHelperInt<SinkType, unsigned int> Super;
      public:
        ArgHelper (unsigned int v) : Super (v) {}
      };

    } // namespace detail

    template<typename FormatStringType>
    template<typename FormatIt>
    void Formatter<FormatStringType>::ParseFormat (FormatIt fmtBegin, FormatIt fmtEnd)
    {
      const CharType* p = fmtBegin;
      const CharType* partStart = p;
      while (p != fmtEnd)
      {
        if (*p == '{')
        {
          p++;
          if (*p)
          {
            if (*p == '{')
            {
              // '{{' -> emit a single '{' in output
              parts.push_back (FormatPart (partStart, p - partStart));
              p++;
              partStart = p;
              continue;
            }
            else
            {
              // Part up to '{'
              if ((p - 1) > partStart)
              {
                parts.push_back (FormatPart (partStart, p - 1 - partStart));
              }
              // Parse format argument
              const CharType* argEnd = p;
              while (*argEnd && (*argEnd != '}')) argEnd++;
              // Parse index
              // TODO: Get rid of lexical_cast<>? (pulls in iostreams...)
              if (argEnd > p)
              {
                size_t index = boost::lexical_cast<size_t> (p, argEnd - p);
                parts.push_back (FormatPart (index));
              }
              // else: assert or so?
              // Continue after '}'
              p = argEnd;
              if (*p) p++;
              partStart = p;
              continue;
            }
          }
        }
        else if (*p == '}')
        {
          p++;
          parts.push_back (FormatPart (partStart, p - partStart));
          /* Collapse '}}' as to a single '}' for symmetry with '}'
           * (but accept lone '}' as well) */
          if (*p && *p == '}')
            p++;
          partStart = p;
          continue;
        }
        p++;
      }

      if (p > partStart)
        parts.push_back (FormatPart (partStart, p - partStart));
    }

    template<typename FormatStringType>
    Formatter<FormatStringType>::Formatter (FormatStringType format)
    {
      ParseFormat (format, format + std::char_traits<CharType>::length (format));
    }

#define _ARGHELPER_EMIT(Z, Arg, Data)                                        \
    detail::ArgHelper<SinkType, BOOST_PP_CAT(A, Arg)>                        \
      BOOST_PP_CAT(ah, Arg) (BOOST_PP_CAT(a, Arg));

#define _DO_SWITCH_FORMATTER_INDEX(Z, N, Macro)                              \
    case N: Macro (N); break;
#define _SWITCH_FORMATTER_INDEX(Z, MaxArg, Macro)                            \
    BOOST_PP_REPEAT_ ## Z(MaxArg, _DO_SWITCH_FORMATTER_INDEX, Macro)

#define _FORMATTER_INDEX_EMIT(Arg)                                           \
    BOOST_PP_CAT(ah, Arg).Emit (sink, BOOST_PP_CAT(a, Arg))
#define _FORMATTER_INDEX_ADD_SIZE(Arg)                                       \
    output_size += BOOST_PP_CAT(ah, Arg).FormattedSize (BOOST_PP_CAT(a, Arg))

#define _DEFINE_FORMATTER_OPERATOR(Z, ArgNum, Data)                          \
    template<typename FormatStringType>                                      \
    template<typename DestType                                               \
      BOOST_PP_ENUM_TRAILING_PARAMS_Z(Z, ArgNum, typename A)>                \
    void Formatter<FormatStringType>::operator() (DestType& dest             \
      BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(Z, ArgNum, const A, & a)) const \
    {                                                                        \
      typedef Sink<DestType> SinkType;                                       \
      BOOST_PP_REPEAT_ ## Z(ArgNum, _ARGHELPER_EMIT, _)                      \
      size_t output_size (0);                                                \
      BOOST_FOREACH(const FormatPart& part, parts)                           \
      {                                                                      \
        if (part.IsStringPart())                                             \
        {                                                                    \
          output_size += part.GetPartStringLen();                            \
        }                                                                    \
        else                                                                 \
        {                                                                    \
          switch(part.GetArgIndex())                                         \
          {                                                                  \
          _SWITCH_FORMATTER_INDEX(Z, ArgNum, _FORMATTER_INDEX_ADD_SIZE)      \
          default:                                                           \
            assert(false); /* Invalid format index */                        \
          }                                                                  \
        }                                                                    \
      }                                                                      \
      SinkType sink (dest, output_size);                                     \
      BOOST_FOREACH(const FormatPart& part, parts)                           \
      {                                                                      \
        if (part.IsStringPart())                                             \
        {                                                                    \
          EmitPartString (sink, part);                                       \
        }                                                                    \
        else                                                                 \
        {                                                                    \
          switch(part.GetArgIndex())                                         \
          {                                                                  \
          _SWITCH_FORMATTER_INDEX(Z, ArgNum, _FORMATTER_INDEX_EMIT)          \
          default:                                                           \
            assert(false); /* Invalid format index */                        \
          }                                                                  \
        }                                                                    \
      }                                                                      \
    }

  #ifdef _MSC_VER
    #pragma warning (push)
    #pragma warning (disable: 4065) // Disable warning in zero-arg formatter
  #endif

    BOOST_PP_REPEAT(FORMATTER_MAX_ARGS, _DEFINE_FORMATTER_OPERATOR, _)

  #ifdef _MSC_VER
    #pragma warning (pop)
  #endif

#undef _DEFINE_FORMATTER_OPERATOR
  } // namespace format
} // namespace s1

#endif // __BASE_FORMAT_FORMATTER_TPP__
