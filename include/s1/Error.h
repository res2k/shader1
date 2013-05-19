/**\file
 * Result code definitions
 */
#ifndef __S1_ERROR_H__
#define __S1_ERROR_H__

/// Result code type
typedef unsigned int s1_ErrorCode;

/**\def S1_ERROR_FLAG
 * \hideinitializer
 * Flag set on error codes to indicate a failure code.
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
/// \internal Shift value for error component
#define _S1_ERROR_COMP_SHIFT                    16
/// \internal Mask value for error component
#define _S1_ERROR_COMP_MASK                     0xf
/// \internal Combine a result code and a component
#define _S1_CODE_COMBINE(Comp, Code)            (((Comp) << _S1_ERROR_COMP_SHIFT) | (Code))
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

/**\name Error code inspection
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
 * Check if a result code is a warning code (i.e. it's a success code,
 * but it's not #S1_SUCCESS).
 */
#define S1_WARNING(Code)                        (S1_SUCCESSFUL(Code) && ((Code) != S1_SUCCESS))

/**\def S1_GET_COMP(Code)
 * \hideinitializer
 * Extract the component from a result code.
 */
#define S1_GET_COMP(Code)                       (((Code) >> _S1_ERROR_COMP_SHIFT) & _S1_ERROR_COMP_MASK)
/**\def S1_GET_EXTRA(Code)
 * \hideinitializer
 * Extract the extra data from a result code.
 */
#define S1_GET_EXTRA(Code)                      (((Code) >> _S1_ERROR_COMP_EXTRA) & _S1_ERROR_COMP_EXTRA)
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

/**\name General result codes
 * @{ */
/**\def S1_SUCCESS
 * \hideinitializer
 * Result code for an unmitigated success.
 * \remarks Since an operation might succeed with the result code \em not being
 * S1_SUCCESS you should always use #S1_SUCCESSFUL to check a result code.
 */
#define S1_SUCCESS                              _S1_MAKE_SUCCESS(0)

/**\def S1_ERROR_COMP_BASE
 * \hideinitializer
 * Error component: &lsquo;base&rsquo; errors
 */
#define S1_ERROR_COMP_BASE                      0u
/**\def S1_E_FAILURE
 * \hideinitializer
 * Generic failure.
 */
#define S1_E_FAILURE                            S1_MAKE_ERROR(S1_ERROR_COMP_BASE, 0)
/**\def S1_E_OUT_OF_MEMORY
 * \hideinitializer
 * Out of memory.
 */
#define S1_E_OUT_OF_MEMORY                      S1_MAKE_ERROR(S1_ERROR_COMP_BASE, 1)
/**\def S1_E_INVALID_ARG
 * \hideinitializer
 * Some argument was invalid (e.g. a pointer was \NULL).
 * \remarks You need to use the #S1_ERROR_EQUAL flag to check for this error code.
 */
#define S1_E_INVALID_ARG                        S1_MAKE_ERROR(S1_ERROR_COMP_BASE, 2)
/**\def S1_E_INVALID_ARG_N(N)
 * \hideinitializer
 * A specific argument was invalid (e.g. a pointer was \NULL).
 * \a N is the argument number and can be extracted using #S1_GET_EXTRA. The first argument
 * has index 0. But note that for functions taking an object instance as the first argument that
 * argument is \em not included when counting the invalid argument number.
 * (Note that passing an invalid object instance instead typically results in a crash.)
 */
#define S1_E_INVALID_ARG_N(N)                   S1_MAKE_ERROR_X(S1_ERROR_COMP_BASE, 2, (N ## u) + 1)
/** @} */

/**\name Compiler options result codes
 * @{ */
/**\def S1_ERROR_COMP_COMPILEROPTIONS
 * \hideinitializer
 * Error component: compiler options
 */
#define S1_ERROR_COMP_COMPILEROPTIONS           1u
/**\def S1_E_INVALID_OPTIMIZATION
 * \hideinitializer
 * The optimization option was invalid.
 */
#define S1_E_INVALID_OPTIMIZATION               S1_MAKE_ERROR(S1_ERROR_COMP_COMPILEROPTIONS, 0)
/** @} */

/**\name Backend-related result codes
 * @{ */
/**\def S1_ERROR_COMP_BACKEND
 * \hideinitializer
 * Error component: backend-related
 */
#define S1_ERROR_COMP_BACKEND                   2u
/**\def S1_E_UNKNOWN_BACKEND
 * \hideinitializer
 * An unknown backend was requested.
 */
#define S1_E_UNKNOWN_BACKEND                    S1_MAKE_ERROR(S1_ERROR_COMP_BACKEND, 0)
/**\def S1_E_GENERATE_FAILED
 * \hideinitializer
 * Program generation failed.
 */
#define S1_E_GENERATE_FAILED                    S1_MAKE_ERROR(S1_ERROR_COMP_BACKEND, 1)
/** @} */

/**\name Program-related result codes
 * @{ */
/**\def S1_ERROR_COMP_PROGRAM
 * \hideinitializer
 * Error component: program-related
 */
#define S1_ERROR_COMP_PROGRAM                   3u
/**\def S1_E_INVALID_FREQUENCY
 * \hideinitializer
 * An invalid input parameter frequency was given.
 */
#define S1_E_INVALID_FREQUENCY                  S1_MAKE_ERROR(S1_ERROR_COMP_PROGRAM, 0)
/**\def S1_E_UNKNOWN_PARAMETER
 * \hideinitializer
 * A program input parameter is not known.
 */
#define S1_E_UNKNOWN_PARAMETER                  S1_MAKE_ERROR(S1_ERROR_COMP_PROGRAM, 1)
/**\def S1_E_INCOMPATIBLE
 * \hideinitializer
 * The requested compatibility level is not supported.
 */
#define S1_E_INCOMPATIBLE                       S1_MAKE_ERROR(S1_ERROR_COMP_PROGRAM, 2)
/** @} */

#if defined(__cplusplus)
namespace s1
{
  /// Result code type
  typedef s1_ErrorCode ErrorCode;
}
#endif // defined(__cplusplus)

#endif // __S1_ERROR_H__
