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

#include "base/common.h"

#include "NumericExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpConst.h"

#include <boost/convert.hpp>
#include "base/boost_convert_spirit.hpp"
#include <outcome/outcome.hpp>

#include <limits>
#include <malloc.h>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::NumericExpressionImpl::NumericExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      const uc::String& valueStr)
       : ExpressionImpl (handler, std::move (context)), valueStr (valueStr),
         numberType (semantics::Type::DetectNumericType (valueStr))
    {
    }
      
    semantics::TypePtr IntermediateGeneratorSemanticsHandler::NumericExpressionImpl::GetValueType()
    {
      return handler->CreateType (numberType);
    }

    template<typename T, T Base, bool Negative>
    static OUTCOME_V2_NAMESPACE::result<T, Error> ParseNumber (const uc::String& str,
                                                               uc::String::size_type begin,
                                                               uc::String::size_type end)
    {
      T val (0);
      for (uc::String::size_type i = begin; i < end; i++)
      {
        unsigned int digitVal = 0;
        uc::Char digit = str.data()[i];
        if ((digit >= '0') && (digit <= '9'))
        {
          digitVal = digit - '0';
        }
        else if ((digit >= 'a') && (digit < 'a' + (Base - 10)))
        {
          digitVal = digit - 'a' + 10;
        }
        else if ((digit >= 'A') && (digit < 'A' + (Base - 10)))
        {
          digitVal = digit - 'A' + 10;
        }
        else
          return OUTCOME_V2_NAMESPACE::failure (Error::NumberParseError);

        // Over-/underflow check
        if (((std::numeric_limits<T>::max() / Base) < val)
            || ((std::numeric_limits<T>::min() / Base) > val))
        {
          return OUTCOME_V2_NAMESPACE::failure (Error::NumberParseError);
        }

        if (Negative)
        {
          // Underflow check
          if ((std::numeric_limits<T>::min() + static_cast<T> (digitVal)) > (val * Base))
          {
            return OUTCOME_V2_NAMESPACE::failure (Error::NumberParseError);
          }
          val = (val * Base) - static_cast<T> (digitVal);
        }
        else
        {
          // Overflow check
          if ((std::numeric_limits<T>::max() - (val * Base)) < static_cast<T> (digitVal))
          {
            return OUTCOME_V2_NAMESPACE::failure (Error::NumberParseError);
          }
          val = (val * Base) + static_cast<T> (digitVal);
        }
      }
      return OUTCOME_V2_NAMESPACE::success (val);
    }

    template<typename T>
    static OUTCOME_V2_NAMESPACE::result<T, Error> ParseInteger (const uc::String& str)
    {
      if (str.isEmpty())
      {
        return OUTCOME_V2_NAMESPACE::failure (Error::NumberParseError);
      }

      if (str.startsWith ("0x") || str.startsWith ("0X"))
      {
        return ParseNumber<T, 16, false> (str, 2, str.length());
      }
      else if (str.startsWith ("-"))
      {
        return ParseNumber<T, 10, true> (str, 1, str.length());
      }
      else
      {
        return ParseNumber<T, 10, false> (str, 0, str.length());
      }
    }

    static OUTCOME_V2_NAMESPACE::result<float, Error> ParseFloat (const uc::String& str)
    {
      // Convert string to ASCII
      uc::String::size_type strLen = str.length();
      char* strAsc = (char*)alloca (strLen + 1);
      auto str_buf = str.data ();
      for (uc::String::size_type i = 0; i < strLen; i++)
      {
        uc::Char ch = str_buf[i];
        if (ch >= 128)
        {
          // Outside ASCII range, can't parse
          return OUTCOME_V2_NAMESPACE::failure (Error::NumberParseError);
        }
        strAsc[i] = static_cast<char> (ch);
      }
      strAsc[strLen] = 0;
      // Actual parsing
      boost::optional<float> floatValue (
        boost::convert<float> (strAsc, boost::cnv::spirit ()));
      if (!floatValue)
      {
        return OUTCOME_V2_NAMESPACE::failure (Error::NumberParseError);
      }
      return OUTCOME_V2_NAMESPACE::success (*floatValue);
    }

    RegisterPtr IntermediateGeneratorSemanticsHandler::NumericExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                             RegisterClassification classify,
                                                                                             bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      RegisterPtr destination (handler->AllocateRegister (*(block.GetSequenceBuilder()), GetValueType(), classify));
      
      // Mark 'destination' as constant or something?
      
      SequenceOpPtr seqOp;
      // Extract value from string
      switch (numberType)
      {
      case semantics::BaseType::Int:
        {
          // Parse number
          auto n = ParseInteger<int> (valueStr);
          if (!n) { ExpressionError (n.error()); return RegisterPtr(); }
          // Create actual sequence operation
          seqOp = SequenceOpPtr (new SequenceOpConst (destination, n.value()));
        }
        break;
      case semantics::BaseType::UInt:
        {
          // Parse number
          auto n = ParseInteger<unsigned int> (valueStr);
          if (!n) { ExpressionError (n.error()); return RegisterPtr(); }
          // Create actual sequence operation
          seqOp = SequenceOpPtr (new SequenceOpConst (destination, n.value()));
        }
        break;
      case semantics::BaseType::Float:
        {
          // Parse number
          auto n = ParseFloat (valueStr);
          if (!n) { ExpressionError (n.error()); return RegisterPtr(); }
          // Create actual sequence operation
          seqOp = SequenceOpPtr (new SequenceOpConst (destination, n.value()));
        }
        break;
      case semantics::BaseType::Invalid:
      case semantics::BaseType::Void:
      case semantics::BaseType::Bool:
        break;
      }
      assert (seqOp);
      block.GetSequenceBuilder()->AddOp (seqOp);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
