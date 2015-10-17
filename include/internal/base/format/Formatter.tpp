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

#include <boost/call_traits.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/optional.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_signed.hpp>

#include "Sink.h"

namespace s1
{
  namespace format
  {
    namespace detail
    {
      /**
       * Parse an argument index from a string
       * @{ */
      boost::optional<unsigned long int> ParseArgumentIndex (const char* indexStrBegin, const char* indexStrEnd);
      boost::optional<unsigned long int> ParseArgumentIndex (const wchar_t* indexStrBegin, const wchar_t* indexStrEnd);
      /** @} */
    } // namespace detail

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
        static size_t FormattedSize (const ArgType& arg)
        {
          return arg.size();
        }
        static void Emit (SinkType& sink, const ArgType& arg)
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
        static size_t FormattedSize (const ArgType&) { return N-1; }
        static void Emit (SinkType& sink, const ArgType& arg)
        {
          sink.append (arg, N - 1 /* excluding NUL */);
        }
      };

      template<typename SinkType, typename CharType, size_t N>
      class ArgHelper<SinkType, CharType[N]>
      {
      public:
        static size_t FormattedSize (const CharType*) { return N-1; }
        static void Emit (SinkType& sink, const CharType* arg)
        {
          for (size_t i = 0; i < N; i++)
          {
            /* No explicit cast -
             * compiler time warnings/runtime bounds check errors are desired */
            typename SinkType::value_type c = arg[i];
            if (!c) break;
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
        static size_t FormattedSize (ArgType arg)
        {
          size_t len (0);
          while (*arg) { arg++; len++; }
          return len;
        }
        static void Emit (SinkType& sink, ArgType arg)
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
        static size_t FormattedSize (ArgType arg) { return IntDigitsHelper<T>::value; }
        static void Emit (SinkType& sink, ArgType arg)
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
      };
      // Unsigned integer
      template<typename SinkType>
      class ArgHelper<SinkType, unsigned int> : public ArgHelperInt<SinkType, unsigned int>
      {
      };

      //
      // Float specialization
      //
      template<typename T>
      class ArgHelperFloat
      {
      protected:
        typedef boost::container::static_vector<char, 32> string_type;
        static string_type ConvertValue (T value);
      public:
        static size_t FormattedSize (T)
        {
          return std::numeric_limits<T>::max_digits10 + 2;
        }
      };

      template<typename SinkType>
      class ArgHelper<SinkType, float> : public ArgHelperFloat<float>
      {
      public:
        static void Emit (SinkType& sink, float value)
        {
          string_type convertedStr (ConvertValue (value));
          sink.append (convertedStr.data(), convertedStr.size());
        }
      };

      // For FormatArgAccessHelper
      template<typename SinkType>
      class ArgHelper<SinkType, boost::none_t>
      {
        static void Throw ()
        {
          throw std::logic_error ("Invalid format argument index");
        }
      public:
        static size_t FormattedSize (const boost::none_t&)
        {
          Throw ();
          return 0;
        }
        static void Emit (SinkType& sink, const boost::none_t&)
        {
          Throw ();
        }
      };

      // Helper: get decayed tuple element type or none_t if index out of bounds
      template<std::size_t Index, typename Tuple>
      struct tuple_element_or_none;

      template<bool InBounds, std::size_t Index, typename Tuple>
      struct real_tuple_element_or_none
      {
        typedef boost::none_t type;
      };
      template<std::size_t Index, typename Tuple>
      struct real_tuple_element_or_none<true, Index, Tuple>
      {
        typedef typename std::tuple_element<Index, Tuple>::type type;
      };

      template<std::size_t Index, typename Tuple>
      struct tuple_element_or_none
      {
        typedef typename real_tuple_element_or_none<
          Index < std::tuple_size<Tuple>::value, Index, Tuple>::type type;
      };

      template<std::size_t Index, typename Tuple>
      struct decayed_tuple_element_or_none
      {
        typedef typename std::decay<typename tuple_element_or_none<Index, Tuple>::type>::type type;
      };

      // Helper: get tuple element or none if index out of bounds
      template<bool InBounds, std::size_t Index>
      struct real_tuple_get
      {
        template<typename Tuple>
        static inline const boost::none_t& get (const Tuple&)
        {
          return boost::none;
        }
      };

      template<std::size_t Index>
      struct real_tuple_get<true, Index>
      {
        template<typename Tuple>
        static inline typename tuple_element_or_none<Index, Tuple>::type const& get (const Tuple& t)
        {
          return std::get<Index> (t);
        }
      };

      template< std::size_t I, typename Tuple>
      static inline typename tuple_element_or_none<I, Tuple>::type const& tuple_get (const Tuple& t)
      {
        return real_tuple_get< I < std::tuple_size<Tuple>::value, I>::get (t);
      }

      // Helper: Access specific formatting args
      template<typename SinkType, typename ...Args>
      class FormatArgAccessHelper
      {
        typedef std::tuple<const Args&...> ArgsTuple;
        ArgsTuple args_tuple;
      public:
        FormatArgAccessHelper (const Args&... args)
          : args_tuple (args...)
        {}

      #define _ARG_HELPER(Arg)                                                      \
          ArgHelper<SinkType, decayed_tuple_element_or_none<Arg, ArgsTuple>::type>
      #define _ARG_FORMATTED_SIZE(Arg)                                              \
          return _ARG_HELPER(Arg)::FormattedSize (tuple_get<Arg> (args_tuple));
      #define _ARG_EMIT(Arg)                                                        \
          _ARG_HELPER(Arg)::Emit (sink, tuple_get<Arg> (args_tuple)); break;
      #define _DO_SWITCH_ARG(Z, N, Macro)                                           \
          case N: Macro (N); break;
      #define _SWITCH_ARG(MaxArg, Macro)                                            \
          BOOST_PP_REPEAT(MaxArg, _DO_SWITCH_ARG, Macro)

        size_t FormattedSize (size_t index) const
        {
          switch(index)
          {
            _SWITCH_ARG(FORMATTER_MAX_ARGS, _ARG_FORMATTED_SIZE)
          default:
            throw std::logic_error ("Invalid format argument index");
          }
          return 0;
        }

        void Emit (SinkType& sink, size_t index)
        {
          switch(index)
          {
            _SWITCH_ARG(FORMATTER_MAX_ARGS, _ARG_EMIT)
          default:
            throw std::logic_error ("Invalid format argument index");
          }
        }

      #undef _ARG_HELPER
      #undef _ARG_FORMATTED_SIZE
      #undef _ARG_EMIT
      #undef _DO_SWITCH_ARG
      #undef _SWITCH_ARG
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
              if (argEnd > p)
              {
                boost::optional<unsigned long int> index (detail::ParseArgumentIndex (p, argEnd));
                if (!index) throw std::logic_error ("Invalid format placeholder");
                parts.push_back (FormatPart (*index));
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

    template<typename FormatStringType>
    template<typename DestType, typename ...Args>
    void Formatter<FormatStringType>::operator() (DestType& dest, const Args&... a) const
    {
      static_assert (sizeof...(Args) <= FORMATTER_MAX_ARGS, "Unsupported formatter argument count");

      typedef Sink<DestType> SinkType;
      detail::FormatArgAccessHelper<SinkType, Args...> arg_access (a...);
      size_t output_size (0);
      for(const FormatPart& part : parts)
      {
        if (part.IsStringPart())
        {
          output_size += part.GetPartStringLen();
        }
        else
        {
          output_size += arg_access.FormattedSize (part.GetArgIndex ());
        }
      }
      SinkType sink (dest, output_size);
      for(const FormatPart& part : parts)
      {
        if (part.IsStringPart())
        {
          EmitPartString (sink, part);
        }
        else
        {
          arg_access.Emit (sink, part.GetArgIndex ());
        }
      }
    }

  } // namespace format
} // namespace s1

#endif // __BASE_FORMAT_FORMATTER_TPP__
