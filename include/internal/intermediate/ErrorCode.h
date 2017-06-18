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

#ifndef __INTERMEDIATE_ERRORCODE_H__
#define __INTERMEDIATE_ERRORCODE_H__

namespace s1
{
  namespace intermediate
  {
    /// Intermediate representation error codes
    enum ErrorCode : int
    {
      /* Note: some error codes represent incorrect/errorneous input,
         some represent wrong internal state/are triggered by bugs.
         The latter codes should probably be separated/have a special range */
      /// Operand types are incompatible with each other
      OperandTypesIncompatible,
      /// Operand types are not valid for the operation
      OperandTypesInvalid,
      /// Trying to assign to a variable of incompatible type
      AssignmentTypesIncompatible,
      /// Assignment target is not an L-value
      AssignmentTargetIsNotAnLValue,
      /// Invalid type cast
      InvalidTypeCast,
      /// No matching function overload
      NoMatchingFunctionOverload,
      /// Ambiguous function overload
      AmbiguousFunctionOverload,
      /// Actual parameter is not an L-value
      ActualParameterNotAnLValue,
      /// The types of the 'if' and 'else' expression of a ternary expression are incompatible
      TernaryExpressionTypesIncompatible,
      /// Too many arguments to type constructor
      TooManyTypeCtorArgs,
      /// Too few arguments to type constructor
      TooFewTypeCtorArgs,
      /// Invalid attribute
      InvalidAttribute,
      /// Value is not of an array type
      NotAnArray,
      /// Index type not an integer type
      IndexNotAnInteger,
      /// Swizzled expression is not an L-value
      SwizzledExpressionNotAnLValue,
      /// A component was used multiple times in an L-value swizzle
      MultipleUseOfComponentInLValueSwizzle,
      /// Array is not an L-value
      ArrayNotAnLValue,
      
      // Below: definitely internal
      /// Error parsing number string
      NumberParseError,
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_ERRORCODE_H__
