#ifndef __INTERMEDIATE_ERRORCODE_H__
#define __INTERMEDIATE_ERRORCODE_H__

namespace s1
{
  namespace intermediate
  {
    /// Intermediate representation error codes
    enum ErrorCode
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
      
      // Below: definitely internal
      /// Error parsing number string
      NumberParseError,
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_ERRORCODE_H__
