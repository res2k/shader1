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
 * Result code type and check macro definitions
 */
#ifndef __S1_RESULTCODE_H__
#define __S1_RESULTCODE_H__

/// Result code type
typedef unsigned int s1_ResultCode;

/**\def S1_ERROR_FLAG
 * \hideinitializer
 * Flag set on result codes to indicate a failure code.
 * \remarks It's better to use #S1_SUCCESSFUL or #S1_FAILED to check
 * for succeess resp. failure.
 */
#define S1_ERROR_FLAG                           0x80000000u
/// \internal Shift value for error extra data
#define _S1_ERROR_EXTRA_SHIFT                   20
/// \internal Mask value for error extra data
#define _S1_ERROR_EXTRA_MASK                    0xff
/// \internal Combine a result code and extra data to produce a failure code
#define _S1_MAKE_ERROR_X(Code, Extra)           ((Code) | ((Extra) << _S1_ERROR_EXTRA_SHIFT) | S1_ERROR_FLAG)
/// \internal Combine a result code and extra data to produce a success code
#define _S1_MAKE_SUCCESS_X(Code, Extra)         ((Code) | ((Extra) << _S1_ERROR_EXTRA_SHIFT))
/// \internal Produce a failure code (without extra data)
#define _S1_MAKE_ERROR(Code)                    _S1_MAKE_ERROR_X(Code, 0u)
/// \internal Produce a success code (without extra data)
#define _S1_MAKE_SUCCESS(Code)                  _S1_MAKE_SUCCESS_X(Code, 0u)
/// \internal Shift value for result component
#define _S1_RESULT_COMP_SHIFT                   16
/// \internal Mask value for result component
#define _S1_RESULT_COMP_MASK                    0xf
/// \internal Combine a result code and a component
#define _S1_CODE_COMBINE(Comp, Code)            (((Comp) << _S1_RESULT_COMP_SHIFT) | (Code))
/**\def S1_MAKE_ERROR(Comp, Code)
 * \hideinitializer
 * Produce an error code for a component
 */
#define S1_MAKE_ERROR(Comp, Code)               (_S1_MAKE_ERROR(_S1_CODE_COMBINE(Comp, Code ## u)))
/**\def S1_MAKE_SUCCESS(Comp, Code)
 * \hideinitializer
 * Produce a success code for a component
 */
#define S1_MAKE_SUCCESS(Comp, Code)             (_S1_MAKE_SUCCESS(_S1_CODE_COMBINE(Comp, Code ## u)))
/**\def S1_MAKE_ERROR_X(Comp, Code, Extra)
 * \hideinitializer
 * Produce an error code for a component carrying extra data
 */
#define S1_MAKE_ERROR_X(Comp, Code, Extra)      (_S1_MAKE_ERROR_X(_S1_CODE_COMBINE(Comp, Code ## u), Extra))
/**\def S1_MAKE_SUCCESS_X(Comp, Code, Extra)
 * \hideinitializer
 * Produce a success code for a component carrying extra data
 */
#define S1_MAKE_SUCCESS_X(Comp, Code, Extra)    (_S1_MAKE_SUCCESS_X(_S1_CODE_COMBINE(Comp, Code ## u), Extra))

/**\name Result code inspection
 * @{ */
/**\def S1_SUCCESSFUL(Code)
 * \hideinitializer
 * Check if a result code is a success code.
 */
#define S1_SUCCESSFUL(Code)                     (((Code) & S1_ERROR_FLAG) == 0)
/**\def S1_FAILED(Code)
 * \hideinitializer
 * Check if a result code is a failure code.
 */
#define S1_FAILED(Code)                         (((Code) & S1_ERROR_FLAG) != 0)
/**\def S1_WARNING(Code)
 * \hideinitializer
 * Check if a result code is a warning code (ie it's a success code,
 * but it's not #S1_SUCCESS).
 */
#define S1_WARNING(Code)                        (S1_SUCCESSFUL(Code) && ((Code) != S1_SUCCESS))

/**\def S1_GET_COMP(Code)
 * \hideinitializer
 * Extract the component from a result code.
 */
#define S1_GET_COMP(Code)                       (((Code) >> _S1_RESULT_COMP_SHIFT) & _S1_RESULT_COMP_MASK)
/**\def S1_GET_EXTRA(Code)
 * \hideinitializer
 * Extract the extra data from a result code.
 */
#define S1_GET_EXTRA(Code)                      (((Code) >> _S1_RESULT_COMP_EXTRA) & _S1_RESULT_COMP_EXTRA)
/**\def S1_CLEAR_EXTRA(Code)
 * \hideinitializer
 * Return the result code with extra data cleared.
 */
#define S1_CLEAR_EXTRA(Code)                    ((Code) & ~(_S1_ERROR_EXTRA_MASK << _S1_ERROR_EXTRA_SHIFT))

/**\def S1_ERROR_EQUAL(Code1, Code2)
 * \hideinitializer
 * Check whether two result codes are equal, ignoring any extra data.
 */
#define S1_ERROR_EQUAL(Code1, Code2)            (S1_CLEAR_EXTRA(Code1) == S1_CLEAR_EXTRA(Code2))
/** @} */

// Actual result code definitions, generated from XML
#include "s1/ResultCode_defs.h"

#if defined(__cplusplus)
namespace s1
{
  /// Result code type
  typedef s1_ResultCode ResultCode;
}
#endif // defined(__cplusplus)

#endif // __S1_RESULTCODE_H__
