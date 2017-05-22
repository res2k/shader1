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
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpConst.h"
#include "TypeImpl.h"

#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>

#include <limits>
#include <malloc.h>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::NumericExpressionImpl::NumericExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      const uc::String& valueStr)
       : ExpressionImpl (handler), valueStr (valueStr),
	 numberType (IntermediateGeneratorSemanticsHandler::DetectNumericType (valueStr))
    {
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::NumericExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> valueType =
	boost::static_pointer_cast<TypeImpl> (handler->CreateType (numberType));
	
      return valueType;
    }

    template<typename T>
    static T ParseNumber (int base, const uc::String& str,
                          uc::String::size_type begin,
                          uc::String::size_type end,
                          bool negative)
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
        else if ((digit >= 'a') && (digit < 'a' + (base - 10)))
        {
          digitVal = digit - 'a' + 10;
        }
        else if ((digit >= 'A') && (digit < 'A' + (base - 10)))
        {
          digitVal = digit - 'A' + 10;
        }
        else
          throw Exception (NumberParseError);

        // Over-/underflow check
        if (((std::numeric_limits<T>::max() / static_cast<T> (base)) < val)
            || ((std::numeric_limits<T>::min() / static_cast<T> (base)) > val))
        {
          throw Exception (NumberParseError);
        }

        if (negative)
        {
          // Underflow check
          if ((std::numeric_limits<T>::min() + static_cast<T> (digitVal)) > (val * static_cast<T> (base)))
          {
            throw Exception (NumberParseError);
          }
          val = (val * static_cast<T> (base)) - static_cast<T> (digitVal);
        }
        else
        {
          // Overflow check
          if ((std::numeric_limits<T>::max() - (val * static_cast<T> (base))) < static_cast<T> (digitVal))
          {
            throw Exception (NumberParseError);
          }
          val = (val * static_cast<T> (base)) + static_cast<T> (digitVal);
        }
      }
      return val;
    }

    template<typename T>
    static T ParseInteger (const uc::String& str)
    {
      if (str.isEmpty())
      {
        throw Exception (NumberParseError);
      }

      if (str.startsWith ("0x") || str.startsWith ("0X"))
      {
        return ParseNumber<T> (16, str, 2, str.length(), false);
      }
      else if (str.startsWith ("-"))
      {
        return ParseNumber<T> (10, str, 1, str.length(), true);
      }
      else
      {
        return ParseNumber<T> (10, str, 0, str.length(), false);
      }
    }

    static float ParseFloat (const uc::String& str)
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
          throw Exception (NumberParseError);
        }
        strAsc[i] = static_cast<char> (ch);
      }
      strAsc[strLen] = 0;
      // Actual parsing
      boost::optional<float> floatValue (
        boost::convert<float> (strAsc, boost::cnv::spirit ()));
      if (!floatValue)
      {
        throw Exception (NumberParseError);
      }
      return *floatValue;
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
      case Int:
        {
          // Parse number
          int n = ParseInteger<int> (valueStr);
          // Create actual sequence operation
          seqOp = SequenceOpPtr (new SequenceOpConst (destination, n));
        }
        break;
      case UInt:
        {
          // Parse number
          unsigned int n = ParseInteger<unsigned int> (valueStr);
          // Create actual sequence operation
          seqOp = SequenceOpPtr (new SequenceOpConst (destination, n));
        }
        break;
      case Float:
        {
          // Parse number
          float n = ParseFloat (valueStr);
          // Create actual sequence operation
          seqOp = SequenceOpPtr (new SequenceOpConst (destination, n));
        }
        break;
      case Void:
      case Bool:
        break;
      }
      assert (seqOp);
      block.GetSequenceBuilder()->AddOp (seqOp);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
