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

#define _S1_ERROR_COMP_BASE                     0u
#define S1_E_FAILURE                            S1_MAKE_ERROR(_S1_ERROR_COMP_BASE, 0)
#define S1_E_OUT_OF_MEMORY                      S1_MAKE_ERROR(_S1_ERROR_COMP_BASE, 1)
#define S1_E_INVALID_ARG                        S1_MAKE_ERROR(_S1_ERROR_COMP_BASE, 2)
#define S1_E_INVALID_ARG_N(N)                   S1_MAKE_ERROR_X(_S1_ERROR_COMP_BASE, 2, (N ## u) + 1)

#if defined(__cplusplus)
namespace s1
{
  typedef s1_ErrorCode ErrorCode;
}
#endif // defined(__cplusplus)

#endif // __S1_ERROR_H__
