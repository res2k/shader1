/**\file
 * Error code definitions
 */
#ifndef __S1_ERROR_H__
#define __S1_ERROR_H__

/// Error code type
typedef unsigned int s1_ErrorCode;

#define S1_ERROR_FLAG                           0x80000000u
#define _S1_ERROR_EXTRA_SHIFT                   20
#define _S1_ERROR_EXTRA_MASK                    0xff
#define _S1_MAKE_ERROR_X(Code, Extra)           ((Code) | ((Extra) << _S1_ERROR_EXTRA_SHIFT) | S1_ERROR_FLAG)
#define _S1_MAKE_SUCCESS_X(Code, Extra)         ((Code) | ((Extra) << _S1_ERROR_EXTRA_SHIFT))
#define _S1_MAKE_ERROR(Code)                    _S1_MAKE_ERROR_X(Code, 0u)
#define _S1_MAKE_SUCCESS(Code)                  _S1_MAKE_SUCCESS_X(Code, 0u)
#define _S1_ERROR_COMP_SHIFT                    16
#define _S1_ERROR_COMP_MASK                     0xf
#define _S1_CODE_COMBINE(Comp, Code)            (((Comp) << _S1_ERROR_COMP_SHIFT) | (Code))
#define S1_MAKE_ERROR(Comp, Code)               (_S1_MAKE_ERROR(_S1_CODE_COMBINE(Comp, Code ## u)))
#define S1_MAKE_SUCCESS(Comp, Code)             (_S1_MAKE_SUCCESS(_S1_CODE_COMBINE(Comp, Code ## u)))
#define S1_MAKE_ERROR_X(Comp, Code, Extra)      (_S1_MAKE_ERROR_X(_S1_CODE_COMBINE(Comp, Code ## u), Extra))
#define S1_MAKE_SUCCESS_X(Comp, Code, Extra)    (_S1_MAKE_SUCCESS_X(_S1_CODE_COMBINE(Comp, Code ## u), Extra))

#define S1_SUCCESSFUL(Code)                     (((Code) & S1_ERROR_FLAG) == 0)
#define S1_FAILED(Code)                         (((Code) & S1_ERROR_FLAG) != 0)
#define S1_WARNING(Code)                        (S1_SUCCESSFUL(Code) && ((Code) != S1_SUCCESS))

#define S1_GET_COMP(Code)                       (((Code) >> _S1_ERROR_COMP_SHIFT) & _S1_ERROR_COMP_MASK)
#define S1_GET_EXTRA(Code)                      (((Code) >> _S1_ERROR_COMP_EXTRA) & _S1_ERROR_COMP_EXTRA)
#define S1_CLEAR_EXTRA(Code)                    ((Code) & ~(_S1_ERROR_EXTRA_MASK << _S1_ERROR_EXTRA_SHIFT))

#define S1_ERROR_EQUAL(Code1, Code2)            (S1_CLEAR_EXTRA(Code1) == S1_CLEAR_EXTRA(Code2))

#define S1_SUCCESS                              _S1_MAKE_SUCCESS(0)

// Error component: 'base' errors
#define _S1_ERROR_COMP_BASE                     0u
#define S1_E_FAILURE                            S1_MAKE_ERROR(_S1_ERROR_COMP_BASE, 0)
#define S1_E_OUT_OF_MEMORY                      S1_MAKE_ERROR(_S1_ERROR_COMP_BASE, 1)
/**\def S1_E_INVALID_ARG
 * Some argument was invalid (e.g. a pointer was \NULL).
 * \remarks You need to use the #S1_ERROR_EQUAL flag to check for this error code.
 */
#define S1_E_INVALID_ARG                        S1_MAKE_ERROR(_S1_ERROR_COMP_BASE, 2)
/**\def S1_E_INVALID_ARG_N(N)
 * A specific argument was invalid (e.g. a pointer was \NULL).
 * \a N is the argument number and can be extracted using #S1_GET_EXTRA. The first argument
 * has index 0. But note that for functions taking an object instance as the first argument that
 * argument is \em not included when counting the invalid argument number.
 * (Note that passing an invalid object instance instead typically results in a crash.)
 */
#define S1_E_INVALID_ARG_N(N)                   S1_MAKE_ERROR_X(_S1_ERROR_COMP_BASE, 2, (N ## u) + 1)

// Error component: compiler options
#define _S1_ERROR_COMP_COMPILEROPTIONS          1u
/**\def S1_E_INVALID_OPTIMIZATION
 * The optimization option was invalid.
 */
#define S1_E_INVALID_OPTIMIZATION               S1_MAKE_ERROR(_S1_ERROR_COMP_COMPILEROPTIONS, 0)

// Error component: backend-related
#define _S1_ERROR_COMP_BACKEND                  2u
/**\def S1_E_UNKNOWN_BACKEND
 * An unknown backend was requested.
 */
#define S1_E_UNKNOWN_BACKEND                    S1_MAKE_ERROR(_S1_ERROR_COMP_BACKEND, 0)
/**\def S1_E_GENERATE_FAILED
 * Program generation failed.
 */
#define S1_E_GENERATE_FAILED                    S1_MAKE_ERROR(_S1_ERROR_COMP_BACKEND, 1)

// Error component: program-related
#define _S1_ERROR_COMP_PROGRAM                  3u
/**\def S1_E_INVALID_FREQUENCY
 * An invalid input parameter frequency was given.
 */
#define S1_E_INVALID_FREQUENCY                  S1_MAKE_ERROR(_S1_ERROR_COMP_PROGRAM, 0)
/**\def S1_E_UNKNOWN_PARAMETER
 * A program input parameter is not known.
 */
#define S1_E_UNKNOWN_PARAMETER                  S1_MAKE_ERROR(_S1_ERROR_COMP_PROGRAM, 1)
/**\def S1_E_INCOMPATIBLE
 * The requested compatibility level is not supported.
 */
#define S1_E_INCOMPATIBLE                       S1_MAKE_ERROR(_S1_ERROR_COMP_PROGRAM, 2)

#if defined(__cplusplus)
namespace s1
{
  typedef s1_ErrorCode ErrorCode;
}
#endif // defined(__cplusplus)

#endif // __S1_ERROR_H__
