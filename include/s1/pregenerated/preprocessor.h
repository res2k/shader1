# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002-2011.                             *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_CONFIG_CONFIG_HPP
# define _S1BOOSTPP_CONFIG_CONFIG_HPP
#
# /* _S1BOOSTPP_CONFIG_FLAGS */
#
# define _S1BOOSTPP_CONFIG_STRICT() 0x0001
# define _S1BOOSTPP_CONFIG_IDEAL() 0x0002
#
# define _S1BOOSTPP_CONFIG_MSVC() 0x0004
# define _S1BOOSTPP_CONFIG_MWCC() 0x0008
# define _S1BOOSTPP_CONFIG_BCC() 0x0010
# define _S1BOOSTPP_CONFIG_EDG() 0x0020
# define _S1BOOSTPP_CONFIG_DMC() 0x0040
#
# ifndef _S1BOOSTPP_CONFIG_FLAGS
#    if defined(__GCCXML__)
#        define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_STRICT())
#    elif defined(__WAVE__)
#        define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_STRICT())
#    elif defined(__MWERKS__) && __MWERKS__ >= 0x3200
#        define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_STRICT())
#    elif defined(__EDG__) || defined(__EDG_VERSION__)
#        if defined(_MSC_VER) && __EDG_VERSION__ >= 308
#            define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_MSVC())
#        else
#            define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_EDG() | _S1BOOSTPP_CONFIG_STRICT())
#        endif
#    elif defined(__MWERKS__)
#        define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_MWCC())
#    elif defined(__DMC__)
#        define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_DMC())
#    elif defined(__BORLANDC__) && __BORLANDC__ >= 0x581
#        define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_STRICT())
#    elif defined(__BORLANDC__) || defined(__IBMC__) || defined(__IBMCPP__) || defined(__SUNPRO_CC)
#        define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_BCC())
#    elif defined(_MSC_VER)
#        define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_MSVC())
#    else
#        define _S1BOOSTPP_CONFIG_FLAGS() (_S1BOOSTPP_CONFIG_STRICT())
#    endif
# endif
#
# /* _S1BOOSTPP_CONFIG_EXTENDED_LINE_INFO */
#
# ifndef _S1BOOSTPP_CONFIG_EXTENDED_LINE_INFO
#    define _S1BOOSTPP_CONFIG_EXTENDED_LINE_INFO 0
# endif
#
# /* _S1BOOSTPP_CONFIG_ERRORS */
#
# ifndef _S1BOOSTPP_CONFIG_ERRORS
#    ifdef NDEBUG
#        define _S1BOOSTPP_CONFIG_ERRORS 0
#    else
#        define _S1BOOSTPP_CONFIG_ERRORS 1
#    endif
# endif
#
# /* _S1BOOSTPP_VARIADICS */
#
# if !defined _S1BOOSTPP_VARIADICS
#    /* variadic support explicitly disabled for all untested compilers */
#    if defined __GCCXML__ || defined __CUDACC__ || defined __PATHSCALE__ || defined __clang__ || defined __DMC__ || defined __CODEGEARC__ || defined __BORLANDC__ || defined __MWERKS__ || defined __SUNPRO_CC || defined __HP_aCC && !defined __EDG__ || defined __MRC__ || defined __SC__ || defined __IBMCPP__ || defined __PGI
#        define _S1BOOSTPP_VARIADICS 0
#    /* VC++ (C/C++) */
#    elif defined _MSC_VER && _MSC_VER >= 1400 && !defined __EDG__
#        if _MSC_VER >= 1400
#            define _S1BOOSTPP_VARIADICS 1
#            define _S1BOOSTPP_VARIADICS_MSVC 1
#        else
#            define _S1BOOSTPP_VARIADICS 0
#        endif
#    /* Wave (C/C++), GCC (C++) */
#    elif defined __WAVE__ && __WAVE_HAS_VARIADICS__ || defined __GNUC__ && __GXX_EXPERIMENTAL_CXX0X__
#        define _S1BOOSTPP_VARIADICS 1
#    /* EDG-based (C/C++), GCC (C), and unknown (C/C++) */
#    elif !defined __cplusplus && __STDC_VERSION__ >= 199901L || __cplusplus >= 201103L
#        define _S1BOOSTPP_VARIADICS 1
#    else
#        define _S1BOOSTPP_VARIADICS 0
#    endif
# elif !_S1BOOSTPP_VARIADICS + 1 < 2
#    undef _S1BOOSTPP_VARIADICS
#    define _S1BOOSTPP_VARIADICS 1
#    if defined _MSC_VER && _MSC_VER >= 1400 && !(defined __EDG__ || defined __GCCXML__ || defined __CUDACC__ || defined __PATHSCALE__ || defined __clang__ || defined __DMC__ || defined __CODEGEARC__ || defined __BORLANDC__ || defined __MWERKS__ || defined __SUNPRO_CC || defined __HP_aCC || defined __MRC__ || defined __SC__ || defined __IBMCPP__ || defined __PGI)
#        define _S1BOOSTPP_VARIADICS_MSVC 1
#    endif
# else
#    undef _S1BOOSTPP_VARIADICS
#    define _S1BOOSTPP_VARIADICS 0
# endif
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_CAT_HPP
# define _S1BOOSTPP_CAT_HPP
#
#
# /* _S1BOOSTPP_CAT */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_CAT(a, b) _S1BOOSTPP_CAT_I(a, b)
# else
#    define _S1BOOSTPP_CAT(a, b) _S1BOOSTPP_CAT_OO((a, b))
#    define _S1BOOSTPP_CAT_OO(par) _S1BOOSTPP_CAT_I ## par
# endif
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC()
#    define _S1BOOSTPP_CAT_I(a, b) a ## b
# else
#    define _S1BOOSTPP_CAT_I(a, b) _S1BOOSTPP_CAT_II(~, a ## b)
#    define _S1BOOSTPP_CAT_II(p, res) res
# endif
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_FACILITIES_EMPTY_HPP
# define _S1BOOSTPP_FACILITIES_EMPTY_HPP
#
# /* _S1BOOSTPP_EMPTY */
#
# define _S1BOOSTPP_EMPTY()
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_FACILITIES_EXPAND_HPP
# define _S1BOOSTPP_FACILITIES_EXPAND_HPP
#
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC() && ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_DMC()
#    define _S1BOOSTPP_EXPAND(x) _S1BOOSTPP_EXPAND_I(x)
# else
#    define _S1BOOSTPP_EXPAND(x) _S1BOOSTPP_EXPAND_OO((x))
#    define _S1BOOSTPP_EXPAND_OO(par) _S1BOOSTPP_EXPAND_I ## par
# endif
#
# define _S1BOOSTPP_EXPAND_I(x) x
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_DEBUG_ERROR_HPP
# define _S1BOOSTPP_DEBUG_ERROR_HPP
#
#
# /* _S1BOOSTPP_ERROR */
#
# if _S1BOOSTPP_CONFIG_ERRORS
#    define _S1BOOSTPP_ERROR(code) _S1BOOSTPP_CAT(_S1BOOSTPP_ERROR_, code)
# endif
#
# define _S1BOOSTPP_ERROR_0x0000 _S1BOOSTPP_ERROR(0x0000, _S1BOOSTPP_INDEX_OUT_OF_BOUNDS)
# define _S1BOOSTPP_ERROR_0x0001 _S1BOOSTPP_ERROR(0x0001, _S1BOOSTPP_WHILE_OVERFLOW)
# define _S1BOOSTPP_ERROR_0x0002 _S1BOOSTPP_ERROR(0x0002, _S1BOOSTPP_FOR_OVERFLOW)
# define _S1BOOSTPP_ERROR_0x0003 _S1BOOSTPP_ERROR(0x0003, _S1BOOSTPP_REPEAT_OVERFLOW)
# define _S1BOOSTPP_ERROR_0x0004 _S1BOOSTPP_ERROR(0x0004, _S1BOOSTPP_LIST_FOLD_OVERFLOW)
# define _S1BOOSTPP_ERROR_0x0005 _S1BOOSTPP_ERROR(0x0005, _S1BOOSTPP_SEQ_FOLD_OVERFLOW)
# define _S1BOOSTPP_ERROR_0x0006 _S1BOOSTPP_ERROR(0x0006, _S1BOOSTPP_ARITHMETIC_OVERFLOW)
# define _S1BOOSTPP_ERROR_0x0007 _S1BOOSTPP_ERROR(0x0007, _S1BOOSTPP_DIVISION_BY_ZERO)
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_CONTROL_IIF_HPP
# define _S1BOOSTPP_CONTROL_IIF_HPP
#
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_IIF(bit, t, f) _S1BOOSTPP_IIF_I(bit, t, f)
# else
#    define _S1BOOSTPP_IIF(bit, t, f) _S1BOOSTPP_IIF_OO((bit, t, f))
#    define _S1BOOSTPP_IIF_OO(par) _S1BOOSTPP_IIF_I ## par
# endif
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC()
#    define _S1BOOSTPP_IIF_I(bit, t, f) _S1BOOSTPP_IIF_ ## bit(t, f)
# else
#    define _S1BOOSTPP_IIF_I(bit, t, f) _S1BOOSTPP_IIF_II(_S1BOOSTPP_IIF_ ## bit(t, f))
#    define _S1BOOSTPP_IIF_II(id) id
# endif
#
# define _S1BOOSTPP_IIF_0(t, f) f
# define _S1BOOSTPP_IIF_1(t, f) t
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_CONTROL_EXPR_IIF_HPP
# define _S1BOOSTPP_CONTROL_EXPR_IIF_HPP
#
#
# /* _S1BOOSTPP_EXPR_IIF */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_EXPR_IIF(bit, expr) _S1BOOSTPP_EXPR_IIF_I(bit, expr)
# else
#    define _S1BOOSTPP_EXPR_IIF(bit, expr) _S1BOOSTPP_EXPR_IIF_OO((bit, expr))
#    define _S1BOOSTPP_EXPR_IIF_OO(par) _S1BOOSTPP_EXPR_IIF_I ## par
# endif
#
# define _S1BOOSTPP_EXPR_IIF_I(bit, expr) _S1BOOSTPP_EXPR_IIF_ ## bit(expr)
#
# define _S1BOOSTPP_EXPR_IIF_0(expr)
# define _S1BOOSTPP_EXPR_IIF_1(expr) expr
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_DETAIL_CHECK_HPP
# define _S1BOOSTPP_DETAIL_CHECK_HPP
#
#
# /* _S1BOOSTPP_CHECK */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_CHECK(x, type) _S1BOOSTPP_CHECK_D(x, type)
# else
#    define _S1BOOSTPP_CHECK(x, type) _S1BOOSTPP_CHECK_OO((x, type))
#    define _S1BOOSTPP_CHECK_OO(par) _S1BOOSTPP_CHECK_D ## par
# endif
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC() && ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_DMC()
#    define _S1BOOSTPP_CHECK_D(x, type) _S1BOOSTPP_CHECK_1(_S1BOOSTPP_CAT(_S1BOOSTPP_CHECK_RESULT_, type x))
#    define _S1BOOSTPP_CHECK_1(chk) _S1BOOSTPP_CHECK_2(chk)
#    define _S1BOOSTPP_CHECK_2(res, _) res
# elif _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC()
#    define _S1BOOSTPP_CHECK_D(x, type) _S1BOOSTPP_CHECK_1(type x)
#    define _S1BOOSTPP_CHECK_1(chk) _S1BOOSTPP_CHECK_2(chk)
#    define _S1BOOSTPP_CHECK_2(chk) _S1BOOSTPP_CHECK_3((_S1BOOSTPP_CHECK_RESULT_ ## chk))
#    define _S1BOOSTPP_CHECK_3(im) _S1BOOSTPP_CHECK_5(_S1BOOSTPP_CHECK_4 im)
#    define _S1BOOSTPP_CHECK_4(res, _) res
#    define _S1BOOSTPP_CHECK_5(res) res
# else /* DMC */
#    define _S1BOOSTPP_CHECK_D(x, type) _S1BOOSTPP_CHECK_OO((type x))
#    define _S1BOOSTPP_CHECK_OO(par) _S1BOOSTPP_CHECK_0 ## par
#    define _S1BOOSTPP_CHECK_0(chk) _S1BOOSTPP_CHECK_1(_S1BOOSTPP_CAT(_S1BOOSTPP_CHECK_RESULT_, chk))
#    define _S1BOOSTPP_CHECK_1(chk) _S1BOOSTPP_CHECK_2(chk)
#    define _S1BOOSTPP_CHECK_2(res, _) res
# endif
#
# define _S1BOOSTPP_CHECK_RESULT_1 1, _S1BOOSTPP_NIL
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_DETAIL_IS_BINARY_HPP
# define _S1BOOSTPP_DETAIL_IS_BINARY_HPP
#
#
# /* _S1BOOSTPP_IS_BINARY */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#    define _S1BOOSTPP_IS_BINARY(x) _S1BOOSTPP_CHECK(x, _S1BOOSTPP_IS_BINARY_CHECK)
# else
#    define _S1BOOSTPP_IS_BINARY(x) _S1BOOSTPP_IS_BINARY_I(x)
#    define _S1BOOSTPP_IS_BINARY_I(x) _S1BOOSTPP_CHECK(x, _S1BOOSTPP_IS_BINARY_CHECK)
# endif
#
# define _S1BOOSTPP_IS_BINARY_CHECK(a, b) 1
# define _S1BOOSTPP_CHECK_RESULT__S1BOOSTPP_IS_BINARY_CHECK 0, _S1BOOSTPP_NIL
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LOGICAL_COMPL_HPP
# define _S1BOOSTPP_LOGICAL_COMPL_HPP
#
#
# /* _S1BOOSTPP_COMPL */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_COMPL(x) _S1BOOSTPP_COMPL_I(x)
# else
#    define _S1BOOSTPP_COMPL(x) _S1BOOSTPP_COMPL_OO((x))
#    define _S1BOOSTPP_COMPL_OO(par) _S1BOOSTPP_COMPL_I ## par
# endif
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC()
#    define _S1BOOSTPP_COMPL_I(x) _S1BOOSTPP_COMPL_ ## x
# else
#    define _S1BOOSTPP_COMPL_I(x) _S1BOOSTPP_COMPL_ID(_S1BOOSTPP_COMPL_ ## x)
#    define _S1BOOSTPP_COMPL_ID(id) id
# endif
#
# define _S1BOOSTPP_COMPL_0 1
# define _S1BOOSTPP_COMPL_1 0
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002-2011) */
# /* Revised by Edward Diener (2011) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_TUPLE_EAT_HPP
# define _S1BOOSTPP_TUPLE_EAT_HPP
#
#
# /* _S1BOOSTPP_EAT */
#
# if _S1BOOSTPP_VARIADICS
#    define _S1BOOSTPP_EAT(...)
# else
#    define _S1BOOSTPP_EAT(x)
# endif
#
# /* _S1BOOSTPP_TUPLE_EAT */
#
# if _S1BOOSTPP_VARIADICS
#    define _S1BOOSTPP_TUPLE_EAT(size) _S1BOOSTPP_EAT
# else
#    if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#        define _S1BOOSTPP_TUPLE_EAT(size) _S1BOOSTPP_TUPLE_EAT_I(size)
#    else
#        define _S1BOOSTPP_TUPLE_EAT(size) _S1BOOSTPP_TUPLE_EAT_OO((size))
#        define _S1BOOSTPP_TUPLE_EAT_OO(par) _S1BOOSTPP_TUPLE_EAT_I ## par
#    endif
#    define _S1BOOSTPP_TUPLE_EAT_I(size) _S1BOOSTPP_TUPLE_EAT_ ## size
# endif
# define _S1BOOSTPP_TUPLE_EAT_1(e0)
# define _S1BOOSTPP_TUPLE_EAT_2(e0, e1)
# define _S1BOOSTPP_TUPLE_EAT_3(e0, e1, e2)
# define _S1BOOSTPP_TUPLE_EAT_4(e0, e1, e2, e3)
# define _S1BOOSTPP_TUPLE_EAT_5(e0, e1, e2, e3, e4)
# define _S1BOOSTPP_TUPLE_EAT_6(e0, e1, e2, e3, e4, e5)
# define _S1BOOSTPP_TUPLE_EAT_7(e0, e1, e2, e3, e4, e5, e6)
# define _S1BOOSTPP_TUPLE_EAT_8(e0, e1, e2, e3, e4, e5, e6, e7)
# define _S1BOOSTPP_TUPLE_EAT_9(e0, e1, e2, e3, e4, e5, e6, e7, e8)
# define _S1BOOSTPP_TUPLE_EAT_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9)
# define _S1BOOSTPP_TUPLE_EAT_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10)
# define _S1BOOSTPP_TUPLE_EAT_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11)
# define _S1BOOSTPP_TUPLE_EAT_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12)
# define _S1BOOSTPP_TUPLE_EAT_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13)
# define _S1BOOSTPP_TUPLE_EAT_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14)
# define _S1BOOSTPP_TUPLE_EAT_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15)
# define _S1BOOSTPP_TUPLE_EAT_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16)
# define _S1BOOSTPP_TUPLE_EAT_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17)
# define _S1BOOSTPP_TUPLE_EAT_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18)
# define _S1BOOSTPP_TUPLE_EAT_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19)
# define _S1BOOSTPP_TUPLE_EAT_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20)
# define _S1BOOSTPP_TUPLE_EAT_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21)
# define _S1BOOSTPP_TUPLE_EAT_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22)
# define _S1BOOSTPP_TUPLE_EAT_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23)
# define _S1BOOSTPP_TUPLE_EAT_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24)
# define _S1BOOSTPP_TUPLE_EAT_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25)
# define _S1BOOSTPP_TUPLE_EAT_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26)
# define _S1BOOSTPP_TUPLE_EAT_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27)
# define _S1BOOSTPP_TUPLE_EAT_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28)
# define _S1BOOSTPP_TUPLE_EAT_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29)
# define _S1BOOSTPP_TUPLE_EAT_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30)
# define _S1BOOSTPP_TUPLE_EAT_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31)
# define _S1BOOSTPP_TUPLE_EAT_33(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32)
# define _S1BOOSTPP_TUPLE_EAT_34(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33)
# define _S1BOOSTPP_TUPLE_EAT_35(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34)
# define _S1BOOSTPP_TUPLE_EAT_36(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35)
# define _S1BOOSTPP_TUPLE_EAT_37(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36)
# define _S1BOOSTPP_TUPLE_EAT_38(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37)
# define _S1BOOSTPP_TUPLE_EAT_39(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38)
# define _S1BOOSTPP_TUPLE_EAT_40(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39)
# define _S1BOOSTPP_TUPLE_EAT_41(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40)
# define _S1BOOSTPP_TUPLE_EAT_42(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41)
# define _S1BOOSTPP_TUPLE_EAT_43(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42)
# define _S1BOOSTPP_TUPLE_EAT_44(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43)
# define _S1BOOSTPP_TUPLE_EAT_45(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44)
# define _S1BOOSTPP_TUPLE_EAT_46(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45)
# define _S1BOOSTPP_TUPLE_EAT_47(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46)
# define _S1BOOSTPP_TUPLE_EAT_48(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47)
# define _S1BOOSTPP_TUPLE_EAT_49(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48)
# define _S1BOOSTPP_TUPLE_EAT_50(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49)
# define _S1BOOSTPP_TUPLE_EAT_51(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50)
# define _S1BOOSTPP_TUPLE_EAT_52(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51)
# define _S1BOOSTPP_TUPLE_EAT_53(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52)
# define _S1BOOSTPP_TUPLE_EAT_54(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53)
# define _S1BOOSTPP_TUPLE_EAT_55(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54)
# define _S1BOOSTPP_TUPLE_EAT_56(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55)
# define _S1BOOSTPP_TUPLE_EAT_57(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56)
# define _S1BOOSTPP_TUPLE_EAT_58(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57)
# define _S1BOOSTPP_TUPLE_EAT_59(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58)
# define _S1BOOSTPP_TUPLE_EAT_60(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59)
# define _S1BOOSTPP_TUPLE_EAT_61(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60)
# define _S1BOOSTPP_TUPLE_EAT_62(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61)
# define _S1BOOSTPP_TUPLE_EAT_63(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62)
# define _S1BOOSTPP_TUPLE_EAT_64(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63)
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  *
#  * See http://www.boost.org for most recent version.
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# ifndef _S1BOOSTPP_LIST_ADT_HPP
# define _S1BOOSTPP_LIST_ADT_HPP
#
#
# /* _S1BOOSTPP_LIST_CONS */
#
# define _S1BOOSTPP_LIST_CONS(head, tail) (head, tail)
#
# /* _S1BOOSTPP_LIST_NIL */
#
# define _S1BOOSTPP_LIST_NIL _S1BOOSTPP_NIL
#
# /* _S1BOOSTPP_LIST_FIRST */
#
# define _S1BOOSTPP_LIST_FIRST(list) _S1BOOSTPP_LIST_FIRST_D(list)
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_LIST_FIRST_D(list) _S1BOOSTPP_LIST_FIRST_I list
# else
#    define _S1BOOSTPP_LIST_FIRST_D(list) _S1BOOSTPP_LIST_FIRST_I ## list
# endif
#
# define _S1BOOSTPP_LIST_FIRST_I(head, tail) head
#
# /* _S1BOOSTPP_LIST_REST */
#
# define _S1BOOSTPP_LIST_REST(list) _S1BOOSTPP_LIST_REST_D(list)
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_LIST_REST_D(list) _S1BOOSTPP_LIST_REST_I list
# else
#    define _S1BOOSTPP_LIST_REST_D(list) _S1BOOSTPP_LIST_REST_I ## list
# endif
#
# define _S1BOOSTPP_LIST_REST_I(head, tail) tail
#
# /* _S1BOOSTPP_LIST_IS_CONS */
#
# if _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_BCC()
#    define _S1BOOSTPP_LIST_IS_CONS(list) _S1BOOSTPP_LIST_IS_CONS_D(list)
#    define _S1BOOSTPP_LIST_IS_CONS_D(list) _S1BOOSTPP_LIST_IS_CONS_ ## list
#    define _S1BOOSTPP_LIST_IS_CONS_(head, tail) 1
#    define _S1BOOSTPP_LIST_IS_CONS__S1BOOSTPP_NIL 0
# else
#    define _S1BOOSTPP_LIST_IS_CONS(list) _S1BOOSTPP_IS_BINARY(list)
# endif
#
# /* _S1BOOSTPP_LIST_IS_NIL */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_BCC()
#    define _S1BOOSTPP_LIST_IS_NIL(list) _S1BOOSTPP_COMPL(_S1BOOSTPP_IS_BINARY(list))
# else
#    define _S1BOOSTPP_LIST_IS_NIL(list) _S1BOOSTPP_COMPL(_S1BOOSTPP_LIST_IS_CONS(list))
# endif
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LIST_DETAIL_FOLD_LEFT_HPP
# define _S1BOOSTPP_LIST_DETAIL_FOLD_LEFT_HPP
#
#
# define _S1BOOSTPP_LIST_FOLD_LEFT_1(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_2, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(2, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_2(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_3, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(3, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_3(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_4, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(4, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_4(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_5, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(5, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_5(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_6, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(6, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_6(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_7, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(7, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_7(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_8, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(8, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_8(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_9, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(9, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_9(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_10, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(10, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_10(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_11, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(11, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_11(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_12, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(12, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_12(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_13, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(13, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_13(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_14, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(14, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_14(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_15, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(15, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_15(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_16, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(16, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_16(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_17, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(17, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_17(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_18, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(18, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_18(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_19, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(19, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_19(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_20, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(20, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_20(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_21, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(21, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_21(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_22, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(22, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_22(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_23, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(23, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_23(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_24, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(24, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_24(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_25, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(25, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_25(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_26, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(26, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_26(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_27, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(27, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_27(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_28, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(28, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_28(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_29, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(29, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_29(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_30, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(30, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_30(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_31, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(31, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_31(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_32, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(32, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_32(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_33, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(33, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_33(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_34, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(34, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_34(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_35, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(35, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_35(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_36, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(36, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_36(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_37, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(37, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_37(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_38, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(38, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_38(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_39, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(39, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_39(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_40, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(40, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_40(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_41, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(41, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_41(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_42, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(42, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_42(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_43, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(43, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_43(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_44, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(44, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_44(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_45, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(45, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_45(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_46, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(46, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_46(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_47, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(47, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_47(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_48, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(48, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_48(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_49, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(49, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_49(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_50, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(50, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_50(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_51, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(51, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_51(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_52, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(52, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_52(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_53, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(53, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_53(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_54, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(54, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_54(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_55, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(55, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_55(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_56, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(56, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_56(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_57, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(57, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_57(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_58, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(58, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_58(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_59, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(59, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_59(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_60, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(60, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_60(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_61, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(61, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_61(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_62, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(62, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_62(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_63, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(63, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_63(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_64, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(64, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_64(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_65, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(65, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_65(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_66, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(66, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_66(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_67, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(67, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_67(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_68, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(68, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_68(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_69, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(69, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_69(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_70, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(70, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_70(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_71, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(71, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_71(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_72, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(72, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_72(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_73, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(73, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_73(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_74, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(74, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_74(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_75, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(75, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_75(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_76, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(76, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_76(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_77, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(77, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_77(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_78, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(78, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_78(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_79, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(79, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_79(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_80, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(80, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_80(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_81, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(81, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_81(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_82, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(82, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_82(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_83, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(83, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_83(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_84, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(84, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_84(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_85, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(85, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_85(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_86, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(86, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_86(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_87, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(87, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_87(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_88, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(88, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_88(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_89, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(89, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_89(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_90, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(90, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_90(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_91, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(91, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_91(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_92, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(92, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_92(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_93, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(93, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_93(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_94, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(94, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_94(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_95, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(95, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_95(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_96, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(96, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_96(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_97, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(97, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_97(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_98, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(98, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_98(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_99, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(99, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_99(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_100, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(100, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_100(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_101, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(101, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_101(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_102, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(102, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_102(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_103, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(103, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_103(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_104, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(104, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_104(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_105, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(105, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_105(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_106, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(106, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_106(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_107, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(107, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_107(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_108, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(108, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_108(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_109, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(109, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_109(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_110, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(110, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_110(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_111, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(111, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_111(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_112, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(112, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_112(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_113, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(113, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_113(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_114, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(114, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_114(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_115, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(115, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_115(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_116, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(116, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_116(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_117, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(117, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_117(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_118, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(118, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_118(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_119, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(119, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_119(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_120, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(120, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_120(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_121, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(121, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_121(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_122, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(122, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_122(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_123, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(123, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_123(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_124, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(124, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_124(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_125, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(125, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_125(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_126, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(126, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_126(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_127, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(127, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_127(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_128, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(128, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_128(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_129, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(129, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_129(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_130, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(130, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_130(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_131, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(131, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_131(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_132, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(132, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_132(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_133, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(133, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_133(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_134, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(134, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_134(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_135, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(135, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_135(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_136, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(136, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_136(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_137, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(137, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_137(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_138, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(138, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_138(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_139, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(139, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_139(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_140, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(140, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_140(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_141, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(141, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_141(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_142, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(142, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_142(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_143, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(143, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_143(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_144, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(144, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_144(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_145, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(145, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_145(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_146, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(146, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_146(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_147, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(147, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_147(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_148, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(148, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_148(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_149, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(149, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_149(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_150, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(150, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_150(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_151, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(151, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_151(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_152, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(152, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_152(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_153, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(153, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_153(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_154, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(154, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_154(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_155, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(155, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_155(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_156, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(156, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_156(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_157, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(157, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_157(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_158, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(158, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_158(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_159, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(159, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_159(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_160, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(160, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_160(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_161, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(161, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_161(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_162, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(162, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_162(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_163, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(163, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_163(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_164, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(164, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_164(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_165, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(165, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_165(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_166, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(166, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_166(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_167, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(167, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_167(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_168, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(168, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_168(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_169, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(169, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_169(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_170, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(170, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_170(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_171, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(171, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_171(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_172, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(172, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_172(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_173, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(173, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_173(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_174, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(174, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_174(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_175, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(175, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_175(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_176, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(176, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_176(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_177, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(177, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_177(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_178, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(178, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_178(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_179, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(179, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_179(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_180, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(180, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_180(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_181, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(181, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_181(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_182, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(182, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_182(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_183, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(183, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_183(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_184, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(184, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_184(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_185, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(185, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_185(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_186, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(186, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_186(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_187, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(187, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_187(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_188, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(188, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_188(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_189, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(189, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_189(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_190, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(190, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_190(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_191, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(191, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_191(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_192, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(192, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_192(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_193, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(193, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_193(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_194, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(194, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_194(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_195, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(195, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_195(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_196, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(196, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_196(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_197, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(197, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_197(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_198, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(198, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_198(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_199, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(199, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_199(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_200, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(200, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_200(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_201, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(201, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_201(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_202, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(202, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_202(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_203, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(203, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_203(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_204, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(204, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_204(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_205, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(205, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_205(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_206, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(206, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_206(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_207, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(207, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_207(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_208, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(208, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_208(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_209, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(209, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_209(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_210, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(210, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_210(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_211, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(211, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_211(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_212, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(212, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_212(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_213, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(213, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_213(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_214, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(214, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_214(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_215, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(215, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_215(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_216, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(216, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_216(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_217, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(217, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_217(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_218, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(218, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_218(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_219, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(219, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_219(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_220, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(220, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_220(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_221, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(221, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_221(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_222, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(222, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_222(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_223, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(223, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_223(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_224, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(224, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_224(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_225, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(225, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_225(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_226, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(226, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_226(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_227, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(227, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_227(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_228, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(228, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_228(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_229, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(229, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_229(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_230, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(230, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_230(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_231, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(231, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_231(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_232, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(232, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_232(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_233, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(233, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_233(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_234, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(234, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_234(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_235, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(235, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_235(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_236, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(236, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_236(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_237, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(237, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_237(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_238, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(238, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_238(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_239, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(239, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_239(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_240, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(240, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_240(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_241, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(241, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_241(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_242, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(242, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_242(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_243, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(243, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_243(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_244, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(244, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_244(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_245, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(245, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_245(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_246, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(246, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_246(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_247, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(247, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_247(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_248, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(248, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_248(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_249, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(249, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_249(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_250, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(250, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_250(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_251, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(251, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_251(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_252, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(252, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_252(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_253, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(253, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_253(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_254, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(254, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_254(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_255, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(255, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_255(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_256, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(256, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
# define _S1BOOSTPP_LIST_FOLD_LEFT_256(o, s, l) _S1BOOSTPP_IIF(_S1BOOSTPP_LIST_IS_CONS(l), _S1BOOSTPP_LIST_FOLD_LEFT_257, s _S1BOOSTPP_TUPLE_EAT_3)(o, _S1BOOSTPP_EXPR_IIF(_S1BOOSTPP_LIST_IS_CONS(l), o)(257, s, _S1BOOSTPP_LIST_FIRST(l)), _S1BOOSTPP_LIST_REST(l))
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LIST_FOLD_LEFT_HPP
# define _S1BOOSTPP_LIST_FOLD_LEFT_HPP
#
#
# /* _S1BOOSTPP_LIST_FOLD_LEFT */
#
# if 0
#    define _S1BOOSTPP_LIST_FOLD_LEFT(op, state, list)
# endif
#
# define _S1BOOSTPP_LIST_FOLD_LEFT _S1BOOSTPP_CAT(_S1BOOSTPP_LIST_FOLD_LEFT_, _S1BOOSTPP_AUTO_REC(_S1BOOSTPP_WHILE_P, 256))
#
# define _S1BOOSTPP_LIST_FOLD_LEFT_257(o, s, l) _S1BOOSTPP_ERROR(0x0004)
#
# define _S1BOOSTPP_LIST_FOLD_LEFT_D(d, o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_ ## d(o, s, l)
# define _S1BOOSTPP_LIST_FOLD_LEFT_2ND _S1BOOSTPP_LIST_FOLD_LEFT
# define _S1BOOSTPP_LIST_FOLD_LEFT_2ND_D _S1BOOSTPP_LIST_FOLD_LEFT_D
#
# if _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
# elif _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_DMC()
# else
# endif
#
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_NIL 1
#
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_1(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_2(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_3(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_4(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_5(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_6(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_7(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_8(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_9(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_10(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_11(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_12(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_13(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_14(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_15(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_16(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_17(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_18(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_19(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_20(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_21(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_22(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_23(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_24(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_25(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_26(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_27(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_28(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_29(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_30(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_31(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_32(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_33(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_34(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_35(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_36(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_37(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_38(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_39(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_40(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_41(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_42(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_43(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_44(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_45(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_46(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_47(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_48(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_49(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_50(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_51(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_52(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_53(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_54(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_55(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_56(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_57(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_58(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_59(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_60(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_61(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_62(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_63(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_64(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_65(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_66(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_67(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_68(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_69(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_70(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_71(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_72(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_73(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_74(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_75(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_76(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_77(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_78(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_79(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_80(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_81(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_82(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_83(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_84(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_85(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_86(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_87(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_88(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_89(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_90(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_91(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_92(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_93(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_94(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_95(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_96(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_97(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_98(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_99(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_100(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_101(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_102(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_103(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_104(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_105(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_106(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_107(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_108(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_109(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_110(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_111(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_112(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_113(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_114(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_115(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_116(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_117(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_118(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_119(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_120(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_121(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_122(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_123(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_124(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_125(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_126(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_127(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_128(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_129(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_130(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_131(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_132(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_133(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_134(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_135(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_136(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_137(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_138(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_139(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_140(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_141(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_142(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_143(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_144(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_145(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_146(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_147(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_148(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_149(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_150(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_151(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_152(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_153(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_154(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_155(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_156(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_157(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_158(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_159(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_160(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_161(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_162(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_163(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_164(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_165(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_166(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_167(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_168(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_169(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_170(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_171(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_172(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_173(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_174(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_175(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_176(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_177(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_178(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_179(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_180(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_181(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_182(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_183(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_184(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_185(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_186(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_187(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_188(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_189(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_190(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_191(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_192(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_193(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_194(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_195(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_196(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_197(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_198(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_199(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_200(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_201(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_202(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_203(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_204(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_205(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_206(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_207(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_208(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_209(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_210(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_211(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_212(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_213(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_214(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_215(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_216(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_217(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_218(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_219(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_220(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_221(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_222(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_223(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_224(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_225(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_226(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_227(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_228(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_229(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_230(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_231(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_232(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_233(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_234(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_235(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_236(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_237(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_238(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_239(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_240(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_241(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_242(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_243(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_244(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_245(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_246(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_247(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_248(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_249(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_250(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_251(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_252(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_253(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_254(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_255(o, s, l) 0
# define _S1BOOSTPP_LIST_FOLD_LEFT_CHECK__S1BOOSTPP_LIST_FOLD_LEFT_256(o, s, l) 0
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LOGICAL_BITAND_HPP
# define _S1BOOSTPP_LOGICAL_BITAND_HPP
#
#
# /* _S1BOOSTPP_BITAND */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_BITAND(x, y) _S1BOOSTPP_BITAND_I(x, y)
# else
#    define _S1BOOSTPP_BITAND(x, y) _S1BOOSTPP_BITAND_OO((x, y))
#    define _S1BOOSTPP_BITAND_OO(par) _S1BOOSTPP_BITAND_I ## par
# endif
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC()
#    define _S1BOOSTPP_BITAND_I(x, y) _S1BOOSTPP_BITAND_ ## x ## y
# else
#    define _S1BOOSTPP_BITAND_I(x, y) _S1BOOSTPP_BITAND_ID(_S1BOOSTPP_BITAND_ ## x ## y)
#    define _S1BOOSTPP_BITAND_ID(res) res
# endif
#
# define _S1BOOSTPP_BITAND_00 0
# define _S1BOOSTPP_BITAND_01 0
# define _S1BOOSTPP_BITAND_10 0
# define _S1BOOSTPP_BITAND_11 1
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LOGICAL_BOOL_HPP
# define _S1BOOSTPP_LOGICAL_BOOL_HPP
#
#
# /* _S1BOOSTPP_BOOL */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_BOOL(x) _S1BOOSTPP_BOOL_I(x)
# else
#    define _S1BOOSTPP_BOOL(x) _S1BOOSTPP_BOOL_OO((x))
#    define _S1BOOSTPP_BOOL_OO(par) _S1BOOSTPP_BOOL_I ## par
# endif
#
# define _S1BOOSTPP_BOOL_I(x) _S1BOOSTPP_BOOL_ ## x
#
# define _S1BOOSTPP_BOOL_0 0
# define _S1BOOSTPP_BOOL_1 1
# define _S1BOOSTPP_BOOL_2 1
# define _S1BOOSTPP_BOOL_3 1
# define _S1BOOSTPP_BOOL_4 1
# define _S1BOOSTPP_BOOL_5 1
# define _S1BOOSTPP_BOOL_6 1
# define _S1BOOSTPP_BOOL_7 1
# define _S1BOOSTPP_BOOL_8 1
# define _S1BOOSTPP_BOOL_9 1
# define _S1BOOSTPP_BOOL_10 1
# define _S1BOOSTPP_BOOL_11 1
# define _S1BOOSTPP_BOOL_12 1
# define _S1BOOSTPP_BOOL_13 1
# define _S1BOOSTPP_BOOL_14 1
# define _S1BOOSTPP_BOOL_15 1
# define _S1BOOSTPP_BOOL_16 1
# define _S1BOOSTPP_BOOL_17 1
# define _S1BOOSTPP_BOOL_18 1
# define _S1BOOSTPP_BOOL_19 1
# define _S1BOOSTPP_BOOL_20 1
# define _S1BOOSTPP_BOOL_21 1
# define _S1BOOSTPP_BOOL_22 1
# define _S1BOOSTPP_BOOL_23 1
# define _S1BOOSTPP_BOOL_24 1
# define _S1BOOSTPP_BOOL_25 1
# define _S1BOOSTPP_BOOL_26 1
# define _S1BOOSTPP_BOOL_27 1
# define _S1BOOSTPP_BOOL_28 1
# define _S1BOOSTPP_BOOL_29 1
# define _S1BOOSTPP_BOOL_30 1
# define _S1BOOSTPP_BOOL_31 1
# define _S1BOOSTPP_BOOL_32 1
# define _S1BOOSTPP_BOOL_33 1
# define _S1BOOSTPP_BOOL_34 1
# define _S1BOOSTPP_BOOL_35 1
# define _S1BOOSTPP_BOOL_36 1
# define _S1BOOSTPP_BOOL_37 1
# define _S1BOOSTPP_BOOL_38 1
# define _S1BOOSTPP_BOOL_39 1
# define _S1BOOSTPP_BOOL_40 1
# define _S1BOOSTPP_BOOL_41 1
# define _S1BOOSTPP_BOOL_42 1
# define _S1BOOSTPP_BOOL_43 1
# define _S1BOOSTPP_BOOL_44 1
# define _S1BOOSTPP_BOOL_45 1
# define _S1BOOSTPP_BOOL_46 1
# define _S1BOOSTPP_BOOL_47 1
# define _S1BOOSTPP_BOOL_48 1
# define _S1BOOSTPP_BOOL_49 1
# define _S1BOOSTPP_BOOL_50 1
# define _S1BOOSTPP_BOOL_51 1
# define _S1BOOSTPP_BOOL_52 1
# define _S1BOOSTPP_BOOL_53 1
# define _S1BOOSTPP_BOOL_54 1
# define _S1BOOSTPP_BOOL_55 1
# define _S1BOOSTPP_BOOL_56 1
# define _S1BOOSTPP_BOOL_57 1
# define _S1BOOSTPP_BOOL_58 1
# define _S1BOOSTPP_BOOL_59 1
# define _S1BOOSTPP_BOOL_60 1
# define _S1BOOSTPP_BOOL_61 1
# define _S1BOOSTPP_BOOL_62 1
# define _S1BOOSTPP_BOOL_63 1
# define _S1BOOSTPP_BOOL_64 1
# define _S1BOOSTPP_BOOL_65 1
# define _S1BOOSTPP_BOOL_66 1
# define _S1BOOSTPP_BOOL_67 1
# define _S1BOOSTPP_BOOL_68 1
# define _S1BOOSTPP_BOOL_69 1
# define _S1BOOSTPP_BOOL_70 1
# define _S1BOOSTPP_BOOL_71 1
# define _S1BOOSTPP_BOOL_72 1
# define _S1BOOSTPP_BOOL_73 1
# define _S1BOOSTPP_BOOL_74 1
# define _S1BOOSTPP_BOOL_75 1
# define _S1BOOSTPP_BOOL_76 1
# define _S1BOOSTPP_BOOL_77 1
# define _S1BOOSTPP_BOOL_78 1
# define _S1BOOSTPP_BOOL_79 1
# define _S1BOOSTPP_BOOL_80 1
# define _S1BOOSTPP_BOOL_81 1
# define _S1BOOSTPP_BOOL_82 1
# define _S1BOOSTPP_BOOL_83 1
# define _S1BOOSTPP_BOOL_84 1
# define _S1BOOSTPP_BOOL_85 1
# define _S1BOOSTPP_BOOL_86 1
# define _S1BOOSTPP_BOOL_87 1
# define _S1BOOSTPP_BOOL_88 1
# define _S1BOOSTPP_BOOL_89 1
# define _S1BOOSTPP_BOOL_90 1
# define _S1BOOSTPP_BOOL_91 1
# define _S1BOOSTPP_BOOL_92 1
# define _S1BOOSTPP_BOOL_93 1
# define _S1BOOSTPP_BOOL_94 1
# define _S1BOOSTPP_BOOL_95 1
# define _S1BOOSTPP_BOOL_96 1
# define _S1BOOSTPP_BOOL_97 1
# define _S1BOOSTPP_BOOL_98 1
# define _S1BOOSTPP_BOOL_99 1
# define _S1BOOSTPP_BOOL_100 1
# define _S1BOOSTPP_BOOL_101 1
# define _S1BOOSTPP_BOOL_102 1
# define _S1BOOSTPP_BOOL_103 1
# define _S1BOOSTPP_BOOL_104 1
# define _S1BOOSTPP_BOOL_105 1
# define _S1BOOSTPP_BOOL_106 1
# define _S1BOOSTPP_BOOL_107 1
# define _S1BOOSTPP_BOOL_108 1
# define _S1BOOSTPP_BOOL_109 1
# define _S1BOOSTPP_BOOL_110 1
# define _S1BOOSTPP_BOOL_111 1
# define _S1BOOSTPP_BOOL_112 1
# define _S1BOOSTPP_BOOL_113 1
# define _S1BOOSTPP_BOOL_114 1
# define _S1BOOSTPP_BOOL_115 1
# define _S1BOOSTPP_BOOL_116 1
# define _S1BOOSTPP_BOOL_117 1
# define _S1BOOSTPP_BOOL_118 1
# define _S1BOOSTPP_BOOL_119 1
# define _S1BOOSTPP_BOOL_120 1
# define _S1BOOSTPP_BOOL_121 1
# define _S1BOOSTPP_BOOL_122 1
# define _S1BOOSTPP_BOOL_123 1
# define _S1BOOSTPP_BOOL_124 1
# define _S1BOOSTPP_BOOL_125 1
# define _S1BOOSTPP_BOOL_126 1
# define _S1BOOSTPP_BOOL_127 1
# define _S1BOOSTPP_BOOL_128 1
# define _S1BOOSTPP_BOOL_129 1
# define _S1BOOSTPP_BOOL_130 1
# define _S1BOOSTPP_BOOL_131 1
# define _S1BOOSTPP_BOOL_132 1
# define _S1BOOSTPP_BOOL_133 1
# define _S1BOOSTPP_BOOL_134 1
# define _S1BOOSTPP_BOOL_135 1
# define _S1BOOSTPP_BOOL_136 1
# define _S1BOOSTPP_BOOL_137 1
# define _S1BOOSTPP_BOOL_138 1
# define _S1BOOSTPP_BOOL_139 1
# define _S1BOOSTPP_BOOL_140 1
# define _S1BOOSTPP_BOOL_141 1
# define _S1BOOSTPP_BOOL_142 1
# define _S1BOOSTPP_BOOL_143 1
# define _S1BOOSTPP_BOOL_144 1
# define _S1BOOSTPP_BOOL_145 1
# define _S1BOOSTPP_BOOL_146 1
# define _S1BOOSTPP_BOOL_147 1
# define _S1BOOSTPP_BOOL_148 1
# define _S1BOOSTPP_BOOL_149 1
# define _S1BOOSTPP_BOOL_150 1
# define _S1BOOSTPP_BOOL_151 1
# define _S1BOOSTPP_BOOL_152 1
# define _S1BOOSTPP_BOOL_153 1
# define _S1BOOSTPP_BOOL_154 1
# define _S1BOOSTPP_BOOL_155 1
# define _S1BOOSTPP_BOOL_156 1
# define _S1BOOSTPP_BOOL_157 1
# define _S1BOOSTPP_BOOL_158 1
# define _S1BOOSTPP_BOOL_159 1
# define _S1BOOSTPP_BOOL_160 1
# define _S1BOOSTPP_BOOL_161 1
# define _S1BOOSTPP_BOOL_162 1
# define _S1BOOSTPP_BOOL_163 1
# define _S1BOOSTPP_BOOL_164 1
# define _S1BOOSTPP_BOOL_165 1
# define _S1BOOSTPP_BOOL_166 1
# define _S1BOOSTPP_BOOL_167 1
# define _S1BOOSTPP_BOOL_168 1
# define _S1BOOSTPP_BOOL_169 1
# define _S1BOOSTPP_BOOL_170 1
# define _S1BOOSTPP_BOOL_171 1
# define _S1BOOSTPP_BOOL_172 1
# define _S1BOOSTPP_BOOL_173 1
# define _S1BOOSTPP_BOOL_174 1
# define _S1BOOSTPP_BOOL_175 1
# define _S1BOOSTPP_BOOL_176 1
# define _S1BOOSTPP_BOOL_177 1
# define _S1BOOSTPP_BOOL_178 1
# define _S1BOOSTPP_BOOL_179 1
# define _S1BOOSTPP_BOOL_180 1
# define _S1BOOSTPP_BOOL_181 1
# define _S1BOOSTPP_BOOL_182 1
# define _S1BOOSTPP_BOOL_183 1
# define _S1BOOSTPP_BOOL_184 1
# define _S1BOOSTPP_BOOL_185 1
# define _S1BOOSTPP_BOOL_186 1
# define _S1BOOSTPP_BOOL_187 1
# define _S1BOOSTPP_BOOL_188 1
# define _S1BOOSTPP_BOOL_189 1
# define _S1BOOSTPP_BOOL_190 1
# define _S1BOOSTPP_BOOL_191 1
# define _S1BOOSTPP_BOOL_192 1
# define _S1BOOSTPP_BOOL_193 1
# define _S1BOOSTPP_BOOL_194 1
# define _S1BOOSTPP_BOOL_195 1
# define _S1BOOSTPP_BOOL_196 1
# define _S1BOOSTPP_BOOL_197 1
# define _S1BOOSTPP_BOOL_198 1
# define _S1BOOSTPP_BOOL_199 1
# define _S1BOOSTPP_BOOL_200 1
# define _S1BOOSTPP_BOOL_201 1
# define _S1BOOSTPP_BOOL_202 1
# define _S1BOOSTPP_BOOL_203 1
# define _S1BOOSTPP_BOOL_204 1
# define _S1BOOSTPP_BOOL_205 1
# define _S1BOOSTPP_BOOL_206 1
# define _S1BOOSTPP_BOOL_207 1
# define _S1BOOSTPP_BOOL_208 1
# define _S1BOOSTPP_BOOL_209 1
# define _S1BOOSTPP_BOOL_210 1
# define _S1BOOSTPP_BOOL_211 1
# define _S1BOOSTPP_BOOL_212 1
# define _S1BOOSTPP_BOOL_213 1
# define _S1BOOSTPP_BOOL_214 1
# define _S1BOOSTPP_BOOL_215 1
# define _S1BOOSTPP_BOOL_216 1
# define _S1BOOSTPP_BOOL_217 1
# define _S1BOOSTPP_BOOL_218 1
# define _S1BOOSTPP_BOOL_219 1
# define _S1BOOSTPP_BOOL_220 1
# define _S1BOOSTPP_BOOL_221 1
# define _S1BOOSTPP_BOOL_222 1
# define _S1BOOSTPP_BOOL_223 1
# define _S1BOOSTPP_BOOL_224 1
# define _S1BOOSTPP_BOOL_225 1
# define _S1BOOSTPP_BOOL_226 1
# define _S1BOOSTPP_BOOL_227 1
# define _S1BOOSTPP_BOOL_228 1
# define _S1BOOSTPP_BOOL_229 1
# define _S1BOOSTPP_BOOL_230 1
# define _S1BOOSTPP_BOOL_231 1
# define _S1BOOSTPP_BOOL_232 1
# define _S1BOOSTPP_BOOL_233 1
# define _S1BOOSTPP_BOOL_234 1
# define _S1BOOSTPP_BOOL_235 1
# define _S1BOOSTPP_BOOL_236 1
# define _S1BOOSTPP_BOOL_237 1
# define _S1BOOSTPP_BOOL_238 1
# define _S1BOOSTPP_BOOL_239 1
# define _S1BOOSTPP_BOOL_240 1
# define _S1BOOSTPP_BOOL_241 1
# define _S1BOOSTPP_BOOL_242 1
# define _S1BOOSTPP_BOOL_243 1
# define _S1BOOSTPP_BOOL_244 1
# define _S1BOOSTPP_BOOL_245 1
# define _S1BOOSTPP_BOOL_246 1
# define _S1BOOSTPP_BOOL_247 1
# define _S1BOOSTPP_BOOL_248 1
# define _S1BOOSTPP_BOOL_249 1
# define _S1BOOSTPP_BOOL_250 1
# define _S1BOOSTPP_BOOL_251 1
# define _S1BOOSTPP_BOOL_252 1
# define _S1BOOSTPP_BOOL_253 1
# define _S1BOOSTPP_BOOL_254 1
# define _S1BOOSTPP_BOOL_255 1
# define _S1BOOSTPP_BOOL_256 1
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_CONTROL_DETAIL_WHILE_HPP
# define _S1BOOSTPP_CONTROL_DETAIL_WHILE_HPP
#
#
# define _S1BOOSTPP_WHILE_1(p, o, s) _S1BOOSTPP_WHILE_1_C(_S1BOOSTPP_BOOL(p(2, s)), p, o, s)
# define _S1BOOSTPP_WHILE_2(p, o, s) _S1BOOSTPP_WHILE_2_C(_S1BOOSTPP_BOOL(p(3, s)), p, o, s)
# define _S1BOOSTPP_WHILE_3(p, o, s) _S1BOOSTPP_WHILE_3_C(_S1BOOSTPP_BOOL(p(4, s)), p, o, s)
# define _S1BOOSTPP_WHILE_4(p, o, s) _S1BOOSTPP_WHILE_4_C(_S1BOOSTPP_BOOL(p(5, s)), p, o, s)
# define _S1BOOSTPP_WHILE_5(p, o, s) _S1BOOSTPP_WHILE_5_C(_S1BOOSTPP_BOOL(p(6, s)), p, o, s)
# define _S1BOOSTPP_WHILE_6(p, o, s) _S1BOOSTPP_WHILE_6_C(_S1BOOSTPP_BOOL(p(7, s)), p, o, s)
# define _S1BOOSTPP_WHILE_7(p, o, s) _S1BOOSTPP_WHILE_7_C(_S1BOOSTPP_BOOL(p(8, s)), p, o, s)
# define _S1BOOSTPP_WHILE_8(p, o, s) _S1BOOSTPP_WHILE_8_C(_S1BOOSTPP_BOOL(p(9, s)), p, o, s)
# define _S1BOOSTPP_WHILE_9(p, o, s) _S1BOOSTPP_WHILE_9_C(_S1BOOSTPP_BOOL(p(10, s)), p, o, s)
# define _S1BOOSTPP_WHILE_10(p, o, s) _S1BOOSTPP_WHILE_10_C(_S1BOOSTPP_BOOL(p(11, s)), p, o, s)
# define _S1BOOSTPP_WHILE_11(p, o, s) _S1BOOSTPP_WHILE_11_C(_S1BOOSTPP_BOOL(p(12, s)), p, o, s)
# define _S1BOOSTPP_WHILE_12(p, o, s) _S1BOOSTPP_WHILE_12_C(_S1BOOSTPP_BOOL(p(13, s)), p, o, s)
# define _S1BOOSTPP_WHILE_13(p, o, s) _S1BOOSTPP_WHILE_13_C(_S1BOOSTPP_BOOL(p(14, s)), p, o, s)
# define _S1BOOSTPP_WHILE_14(p, o, s) _S1BOOSTPP_WHILE_14_C(_S1BOOSTPP_BOOL(p(15, s)), p, o, s)
# define _S1BOOSTPP_WHILE_15(p, o, s) _S1BOOSTPP_WHILE_15_C(_S1BOOSTPP_BOOL(p(16, s)), p, o, s)
# define _S1BOOSTPP_WHILE_16(p, o, s) _S1BOOSTPP_WHILE_16_C(_S1BOOSTPP_BOOL(p(17, s)), p, o, s)
# define _S1BOOSTPP_WHILE_17(p, o, s) _S1BOOSTPP_WHILE_17_C(_S1BOOSTPP_BOOL(p(18, s)), p, o, s)
# define _S1BOOSTPP_WHILE_18(p, o, s) _S1BOOSTPP_WHILE_18_C(_S1BOOSTPP_BOOL(p(19, s)), p, o, s)
# define _S1BOOSTPP_WHILE_19(p, o, s) _S1BOOSTPP_WHILE_19_C(_S1BOOSTPP_BOOL(p(20, s)), p, o, s)
# define _S1BOOSTPP_WHILE_20(p, o, s) _S1BOOSTPP_WHILE_20_C(_S1BOOSTPP_BOOL(p(21, s)), p, o, s)
# define _S1BOOSTPP_WHILE_21(p, o, s) _S1BOOSTPP_WHILE_21_C(_S1BOOSTPP_BOOL(p(22, s)), p, o, s)
# define _S1BOOSTPP_WHILE_22(p, o, s) _S1BOOSTPP_WHILE_22_C(_S1BOOSTPP_BOOL(p(23, s)), p, o, s)
# define _S1BOOSTPP_WHILE_23(p, o, s) _S1BOOSTPP_WHILE_23_C(_S1BOOSTPP_BOOL(p(24, s)), p, o, s)
# define _S1BOOSTPP_WHILE_24(p, o, s) _S1BOOSTPP_WHILE_24_C(_S1BOOSTPP_BOOL(p(25, s)), p, o, s)
# define _S1BOOSTPP_WHILE_25(p, o, s) _S1BOOSTPP_WHILE_25_C(_S1BOOSTPP_BOOL(p(26, s)), p, o, s)
# define _S1BOOSTPP_WHILE_26(p, o, s) _S1BOOSTPP_WHILE_26_C(_S1BOOSTPP_BOOL(p(27, s)), p, o, s)
# define _S1BOOSTPP_WHILE_27(p, o, s) _S1BOOSTPP_WHILE_27_C(_S1BOOSTPP_BOOL(p(28, s)), p, o, s)
# define _S1BOOSTPP_WHILE_28(p, o, s) _S1BOOSTPP_WHILE_28_C(_S1BOOSTPP_BOOL(p(29, s)), p, o, s)
# define _S1BOOSTPP_WHILE_29(p, o, s) _S1BOOSTPP_WHILE_29_C(_S1BOOSTPP_BOOL(p(30, s)), p, o, s)
# define _S1BOOSTPP_WHILE_30(p, o, s) _S1BOOSTPP_WHILE_30_C(_S1BOOSTPP_BOOL(p(31, s)), p, o, s)
# define _S1BOOSTPP_WHILE_31(p, o, s) _S1BOOSTPP_WHILE_31_C(_S1BOOSTPP_BOOL(p(32, s)), p, o, s)
# define _S1BOOSTPP_WHILE_32(p, o, s) _S1BOOSTPP_WHILE_32_C(_S1BOOSTPP_BOOL(p(33, s)), p, o, s)
# define _S1BOOSTPP_WHILE_33(p, o, s) _S1BOOSTPP_WHILE_33_C(_S1BOOSTPP_BOOL(p(34, s)), p, o, s)
# define _S1BOOSTPP_WHILE_34(p, o, s) _S1BOOSTPP_WHILE_34_C(_S1BOOSTPP_BOOL(p(35, s)), p, o, s)
# define _S1BOOSTPP_WHILE_35(p, o, s) _S1BOOSTPP_WHILE_35_C(_S1BOOSTPP_BOOL(p(36, s)), p, o, s)
# define _S1BOOSTPP_WHILE_36(p, o, s) _S1BOOSTPP_WHILE_36_C(_S1BOOSTPP_BOOL(p(37, s)), p, o, s)
# define _S1BOOSTPP_WHILE_37(p, o, s) _S1BOOSTPP_WHILE_37_C(_S1BOOSTPP_BOOL(p(38, s)), p, o, s)
# define _S1BOOSTPP_WHILE_38(p, o, s) _S1BOOSTPP_WHILE_38_C(_S1BOOSTPP_BOOL(p(39, s)), p, o, s)
# define _S1BOOSTPP_WHILE_39(p, o, s) _S1BOOSTPP_WHILE_39_C(_S1BOOSTPP_BOOL(p(40, s)), p, o, s)
# define _S1BOOSTPP_WHILE_40(p, o, s) _S1BOOSTPP_WHILE_40_C(_S1BOOSTPP_BOOL(p(41, s)), p, o, s)
# define _S1BOOSTPP_WHILE_41(p, o, s) _S1BOOSTPP_WHILE_41_C(_S1BOOSTPP_BOOL(p(42, s)), p, o, s)
# define _S1BOOSTPP_WHILE_42(p, o, s) _S1BOOSTPP_WHILE_42_C(_S1BOOSTPP_BOOL(p(43, s)), p, o, s)
# define _S1BOOSTPP_WHILE_43(p, o, s) _S1BOOSTPP_WHILE_43_C(_S1BOOSTPP_BOOL(p(44, s)), p, o, s)
# define _S1BOOSTPP_WHILE_44(p, o, s) _S1BOOSTPP_WHILE_44_C(_S1BOOSTPP_BOOL(p(45, s)), p, o, s)
# define _S1BOOSTPP_WHILE_45(p, o, s) _S1BOOSTPP_WHILE_45_C(_S1BOOSTPP_BOOL(p(46, s)), p, o, s)
# define _S1BOOSTPP_WHILE_46(p, o, s) _S1BOOSTPP_WHILE_46_C(_S1BOOSTPP_BOOL(p(47, s)), p, o, s)
# define _S1BOOSTPP_WHILE_47(p, o, s) _S1BOOSTPP_WHILE_47_C(_S1BOOSTPP_BOOL(p(48, s)), p, o, s)
# define _S1BOOSTPP_WHILE_48(p, o, s) _S1BOOSTPP_WHILE_48_C(_S1BOOSTPP_BOOL(p(49, s)), p, o, s)
# define _S1BOOSTPP_WHILE_49(p, o, s) _S1BOOSTPP_WHILE_49_C(_S1BOOSTPP_BOOL(p(50, s)), p, o, s)
# define _S1BOOSTPP_WHILE_50(p, o, s) _S1BOOSTPP_WHILE_50_C(_S1BOOSTPP_BOOL(p(51, s)), p, o, s)
# define _S1BOOSTPP_WHILE_51(p, o, s) _S1BOOSTPP_WHILE_51_C(_S1BOOSTPP_BOOL(p(52, s)), p, o, s)
# define _S1BOOSTPP_WHILE_52(p, o, s) _S1BOOSTPP_WHILE_52_C(_S1BOOSTPP_BOOL(p(53, s)), p, o, s)
# define _S1BOOSTPP_WHILE_53(p, o, s) _S1BOOSTPP_WHILE_53_C(_S1BOOSTPP_BOOL(p(54, s)), p, o, s)
# define _S1BOOSTPP_WHILE_54(p, o, s) _S1BOOSTPP_WHILE_54_C(_S1BOOSTPP_BOOL(p(55, s)), p, o, s)
# define _S1BOOSTPP_WHILE_55(p, o, s) _S1BOOSTPP_WHILE_55_C(_S1BOOSTPP_BOOL(p(56, s)), p, o, s)
# define _S1BOOSTPP_WHILE_56(p, o, s) _S1BOOSTPP_WHILE_56_C(_S1BOOSTPP_BOOL(p(57, s)), p, o, s)
# define _S1BOOSTPP_WHILE_57(p, o, s) _S1BOOSTPP_WHILE_57_C(_S1BOOSTPP_BOOL(p(58, s)), p, o, s)
# define _S1BOOSTPP_WHILE_58(p, o, s) _S1BOOSTPP_WHILE_58_C(_S1BOOSTPP_BOOL(p(59, s)), p, o, s)
# define _S1BOOSTPP_WHILE_59(p, o, s) _S1BOOSTPP_WHILE_59_C(_S1BOOSTPP_BOOL(p(60, s)), p, o, s)
# define _S1BOOSTPP_WHILE_60(p, o, s) _S1BOOSTPP_WHILE_60_C(_S1BOOSTPP_BOOL(p(61, s)), p, o, s)
# define _S1BOOSTPP_WHILE_61(p, o, s) _S1BOOSTPP_WHILE_61_C(_S1BOOSTPP_BOOL(p(62, s)), p, o, s)
# define _S1BOOSTPP_WHILE_62(p, o, s) _S1BOOSTPP_WHILE_62_C(_S1BOOSTPP_BOOL(p(63, s)), p, o, s)
# define _S1BOOSTPP_WHILE_63(p, o, s) _S1BOOSTPP_WHILE_63_C(_S1BOOSTPP_BOOL(p(64, s)), p, o, s)
# define _S1BOOSTPP_WHILE_64(p, o, s) _S1BOOSTPP_WHILE_64_C(_S1BOOSTPP_BOOL(p(65, s)), p, o, s)
# define _S1BOOSTPP_WHILE_65(p, o, s) _S1BOOSTPP_WHILE_65_C(_S1BOOSTPP_BOOL(p(66, s)), p, o, s)
# define _S1BOOSTPP_WHILE_66(p, o, s) _S1BOOSTPP_WHILE_66_C(_S1BOOSTPP_BOOL(p(67, s)), p, o, s)
# define _S1BOOSTPP_WHILE_67(p, o, s) _S1BOOSTPP_WHILE_67_C(_S1BOOSTPP_BOOL(p(68, s)), p, o, s)
# define _S1BOOSTPP_WHILE_68(p, o, s) _S1BOOSTPP_WHILE_68_C(_S1BOOSTPP_BOOL(p(69, s)), p, o, s)
# define _S1BOOSTPP_WHILE_69(p, o, s) _S1BOOSTPP_WHILE_69_C(_S1BOOSTPP_BOOL(p(70, s)), p, o, s)
# define _S1BOOSTPP_WHILE_70(p, o, s) _S1BOOSTPP_WHILE_70_C(_S1BOOSTPP_BOOL(p(71, s)), p, o, s)
# define _S1BOOSTPP_WHILE_71(p, o, s) _S1BOOSTPP_WHILE_71_C(_S1BOOSTPP_BOOL(p(72, s)), p, o, s)
# define _S1BOOSTPP_WHILE_72(p, o, s) _S1BOOSTPP_WHILE_72_C(_S1BOOSTPP_BOOL(p(73, s)), p, o, s)
# define _S1BOOSTPP_WHILE_73(p, o, s) _S1BOOSTPP_WHILE_73_C(_S1BOOSTPP_BOOL(p(74, s)), p, o, s)
# define _S1BOOSTPP_WHILE_74(p, o, s) _S1BOOSTPP_WHILE_74_C(_S1BOOSTPP_BOOL(p(75, s)), p, o, s)
# define _S1BOOSTPP_WHILE_75(p, o, s) _S1BOOSTPP_WHILE_75_C(_S1BOOSTPP_BOOL(p(76, s)), p, o, s)
# define _S1BOOSTPP_WHILE_76(p, o, s) _S1BOOSTPP_WHILE_76_C(_S1BOOSTPP_BOOL(p(77, s)), p, o, s)
# define _S1BOOSTPP_WHILE_77(p, o, s) _S1BOOSTPP_WHILE_77_C(_S1BOOSTPP_BOOL(p(78, s)), p, o, s)
# define _S1BOOSTPP_WHILE_78(p, o, s) _S1BOOSTPP_WHILE_78_C(_S1BOOSTPP_BOOL(p(79, s)), p, o, s)
# define _S1BOOSTPP_WHILE_79(p, o, s) _S1BOOSTPP_WHILE_79_C(_S1BOOSTPP_BOOL(p(80, s)), p, o, s)
# define _S1BOOSTPP_WHILE_80(p, o, s) _S1BOOSTPP_WHILE_80_C(_S1BOOSTPP_BOOL(p(81, s)), p, o, s)
# define _S1BOOSTPP_WHILE_81(p, o, s) _S1BOOSTPP_WHILE_81_C(_S1BOOSTPP_BOOL(p(82, s)), p, o, s)
# define _S1BOOSTPP_WHILE_82(p, o, s) _S1BOOSTPP_WHILE_82_C(_S1BOOSTPP_BOOL(p(83, s)), p, o, s)
# define _S1BOOSTPP_WHILE_83(p, o, s) _S1BOOSTPP_WHILE_83_C(_S1BOOSTPP_BOOL(p(84, s)), p, o, s)
# define _S1BOOSTPP_WHILE_84(p, o, s) _S1BOOSTPP_WHILE_84_C(_S1BOOSTPP_BOOL(p(85, s)), p, o, s)
# define _S1BOOSTPP_WHILE_85(p, o, s) _S1BOOSTPP_WHILE_85_C(_S1BOOSTPP_BOOL(p(86, s)), p, o, s)
# define _S1BOOSTPP_WHILE_86(p, o, s) _S1BOOSTPP_WHILE_86_C(_S1BOOSTPP_BOOL(p(87, s)), p, o, s)
# define _S1BOOSTPP_WHILE_87(p, o, s) _S1BOOSTPP_WHILE_87_C(_S1BOOSTPP_BOOL(p(88, s)), p, o, s)
# define _S1BOOSTPP_WHILE_88(p, o, s) _S1BOOSTPP_WHILE_88_C(_S1BOOSTPP_BOOL(p(89, s)), p, o, s)
# define _S1BOOSTPP_WHILE_89(p, o, s) _S1BOOSTPP_WHILE_89_C(_S1BOOSTPP_BOOL(p(90, s)), p, o, s)
# define _S1BOOSTPP_WHILE_90(p, o, s) _S1BOOSTPP_WHILE_90_C(_S1BOOSTPP_BOOL(p(91, s)), p, o, s)
# define _S1BOOSTPP_WHILE_91(p, o, s) _S1BOOSTPP_WHILE_91_C(_S1BOOSTPP_BOOL(p(92, s)), p, o, s)
# define _S1BOOSTPP_WHILE_92(p, o, s) _S1BOOSTPP_WHILE_92_C(_S1BOOSTPP_BOOL(p(93, s)), p, o, s)
# define _S1BOOSTPP_WHILE_93(p, o, s) _S1BOOSTPP_WHILE_93_C(_S1BOOSTPP_BOOL(p(94, s)), p, o, s)
# define _S1BOOSTPP_WHILE_94(p, o, s) _S1BOOSTPP_WHILE_94_C(_S1BOOSTPP_BOOL(p(95, s)), p, o, s)
# define _S1BOOSTPP_WHILE_95(p, o, s) _S1BOOSTPP_WHILE_95_C(_S1BOOSTPP_BOOL(p(96, s)), p, o, s)
# define _S1BOOSTPP_WHILE_96(p, o, s) _S1BOOSTPP_WHILE_96_C(_S1BOOSTPP_BOOL(p(97, s)), p, o, s)
# define _S1BOOSTPP_WHILE_97(p, o, s) _S1BOOSTPP_WHILE_97_C(_S1BOOSTPP_BOOL(p(98, s)), p, o, s)
# define _S1BOOSTPP_WHILE_98(p, o, s) _S1BOOSTPP_WHILE_98_C(_S1BOOSTPP_BOOL(p(99, s)), p, o, s)
# define _S1BOOSTPP_WHILE_99(p, o, s) _S1BOOSTPP_WHILE_99_C(_S1BOOSTPP_BOOL(p(100, s)), p, o, s)
# define _S1BOOSTPP_WHILE_100(p, o, s) _S1BOOSTPP_WHILE_100_C(_S1BOOSTPP_BOOL(p(101, s)), p, o, s)
# define _S1BOOSTPP_WHILE_101(p, o, s) _S1BOOSTPP_WHILE_101_C(_S1BOOSTPP_BOOL(p(102, s)), p, o, s)
# define _S1BOOSTPP_WHILE_102(p, o, s) _S1BOOSTPP_WHILE_102_C(_S1BOOSTPP_BOOL(p(103, s)), p, o, s)
# define _S1BOOSTPP_WHILE_103(p, o, s) _S1BOOSTPP_WHILE_103_C(_S1BOOSTPP_BOOL(p(104, s)), p, o, s)
# define _S1BOOSTPP_WHILE_104(p, o, s) _S1BOOSTPP_WHILE_104_C(_S1BOOSTPP_BOOL(p(105, s)), p, o, s)
# define _S1BOOSTPP_WHILE_105(p, o, s) _S1BOOSTPP_WHILE_105_C(_S1BOOSTPP_BOOL(p(106, s)), p, o, s)
# define _S1BOOSTPP_WHILE_106(p, o, s) _S1BOOSTPP_WHILE_106_C(_S1BOOSTPP_BOOL(p(107, s)), p, o, s)
# define _S1BOOSTPP_WHILE_107(p, o, s) _S1BOOSTPP_WHILE_107_C(_S1BOOSTPP_BOOL(p(108, s)), p, o, s)
# define _S1BOOSTPP_WHILE_108(p, o, s) _S1BOOSTPP_WHILE_108_C(_S1BOOSTPP_BOOL(p(109, s)), p, o, s)
# define _S1BOOSTPP_WHILE_109(p, o, s) _S1BOOSTPP_WHILE_109_C(_S1BOOSTPP_BOOL(p(110, s)), p, o, s)
# define _S1BOOSTPP_WHILE_110(p, o, s) _S1BOOSTPP_WHILE_110_C(_S1BOOSTPP_BOOL(p(111, s)), p, o, s)
# define _S1BOOSTPP_WHILE_111(p, o, s) _S1BOOSTPP_WHILE_111_C(_S1BOOSTPP_BOOL(p(112, s)), p, o, s)
# define _S1BOOSTPP_WHILE_112(p, o, s) _S1BOOSTPP_WHILE_112_C(_S1BOOSTPP_BOOL(p(113, s)), p, o, s)
# define _S1BOOSTPP_WHILE_113(p, o, s) _S1BOOSTPP_WHILE_113_C(_S1BOOSTPP_BOOL(p(114, s)), p, o, s)
# define _S1BOOSTPP_WHILE_114(p, o, s) _S1BOOSTPP_WHILE_114_C(_S1BOOSTPP_BOOL(p(115, s)), p, o, s)
# define _S1BOOSTPP_WHILE_115(p, o, s) _S1BOOSTPP_WHILE_115_C(_S1BOOSTPP_BOOL(p(116, s)), p, o, s)
# define _S1BOOSTPP_WHILE_116(p, o, s) _S1BOOSTPP_WHILE_116_C(_S1BOOSTPP_BOOL(p(117, s)), p, o, s)
# define _S1BOOSTPP_WHILE_117(p, o, s) _S1BOOSTPP_WHILE_117_C(_S1BOOSTPP_BOOL(p(118, s)), p, o, s)
# define _S1BOOSTPP_WHILE_118(p, o, s) _S1BOOSTPP_WHILE_118_C(_S1BOOSTPP_BOOL(p(119, s)), p, o, s)
# define _S1BOOSTPP_WHILE_119(p, o, s) _S1BOOSTPP_WHILE_119_C(_S1BOOSTPP_BOOL(p(120, s)), p, o, s)
# define _S1BOOSTPP_WHILE_120(p, o, s) _S1BOOSTPP_WHILE_120_C(_S1BOOSTPP_BOOL(p(121, s)), p, o, s)
# define _S1BOOSTPP_WHILE_121(p, o, s) _S1BOOSTPP_WHILE_121_C(_S1BOOSTPP_BOOL(p(122, s)), p, o, s)
# define _S1BOOSTPP_WHILE_122(p, o, s) _S1BOOSTPP_WHILE_122_C(_S1BOOSTPP_BOOL(p(123, s)), p, o, s)
# define _S1BOOSTPP_WHILE_123(p, o, s) _S1BOOSTPP_WHILE_123_C(_S1BOOSTPP_BOOL(p(124, s)), p, o, s)
# define _S1BOOSTPP_WHILE_124(p, o, s) _S1BOOSTPP_WHILE_124_C(_S1BOOSTPP_BOOL(p(125, s)), p, o, s)
# define _S1BOOSTPP_WHILE_125(p, o, s) _S1BOOSTPP_WHILE_125_C(_S1BOOSTPP_BOOL(p(126, s)), p, o, s)
# define _S1BOOSTPP_WHILE_126(p, o, s) _S1BOOSTPP_WHILE_126_C(_S1BOOSTPP_BOOL(p(127, s)), p, o, s)
# define _S1BOOSTPP_WHILE_127(p, o, s) _S1BOOSTPP_WHILE_127_C(_S1BOOSTPP_BOOL(p(128, s)), p, o, s)
# define _S1BOOSTPP_WHILE_128(p, o, s) _S1BOOSTPP_WHILE_128_C(_S1BOOSTPP_BOOL(p(129, s)), p, o, s)
# define _S1BOOSTPP_WHILE_129(p, o, s) _S1BOOSTPP_WHILE_129_C(_S1BOOSTPP_BOOL(p(130, s)), p, o, s)
# define _S1BOOSTPP_WHILE_130(p, o, s) _S1BOOSTPP_WHILE_130_C(_S1BOOSTPP_BOOL(p(131, s)), p, o, s)
# define _S1BOOSTPP_WHILE_131(p, o, s) _S1BOOSTPP_WHILE_131_C(_S1BOOSTPP_BOOL(p(132, s)), p, o, s)
# define _S1BOOSTPP_WHILE_132(p, o, s) _S1BOOSTPP_WHILE_132_C(_S1BOOSTPP_BOOL(p(133, s)), p, o, s)
# define _S1BOOSTPP_WHILE_133(p, o, s) _S1BOOSTPP_WHILE_133_C(_S1BOOSTPP_BOOL(p(134, s)), p, o, s)
# define _S1BOOSTPP_WHILE_134(p, o, s) _S1BOOSTPP_WHILE_134_C(_S1BOOSTPP_BOOL(p(135, s)), p, o, s)
# define _S1BOOSTPP_WHILE_135(p, o, s) _S1BOOSTPP_WHILE_135_C(_S1BOOSTPP_BOOL(p(136, s)), p, o, s)
# define _S1BOOSTPP_WHILE_136(p, o, s) _S1BOOSTPP_WHILE_136_C(_S1BOOSTPP_BOOL(p(137, s)), p, o, s)
# define _S1BOOSTPP_WHILE_137(p, o, s) _S1BOOSTPP_WHILE_137_C(_S1BOOSTPP_BOOL(p(138, s)), p, o, s)
# define _S1BOOSTPP_WHILE_138(p, o, s) _S1BOOSTPP_WHILE_138_C(_S1BOOSTPP_BOOL(p(139, s)), p, o, s)
# define _S1BOOSTPP_WHILE_139(p, o, s) _S1BOOSTPP_WHILE_139_C(_S1BOOSTPP_BOOL(p(140, s)), p, o, s)
# define _S1BOOSTPP_WHILE_140(p, o, s) _S1BOOSTPP_WHILE_140_C(_S1BOOSTPP_BOOL(p(141, s)), p, o, s)
# define _S1BOOSTPP_WHILE_141(p, o, s) _S1BOOSTPP_WHILE_141_C(_S1BOOSTPP_BOOL(p(142, s)), p, o, s)
# define _S1BOOSTPP_WHILE_142(p, o, s) _S1BOOSTPP_WHILE_142_C(_S1BOOSTPP_BOOL(p(143, s)), p, o, s)
# define _S1BOOSTPP_WHILE_143(p, o, s) _S1BOOSTPP_WHILE_143_C(_S1BOOSTPP_BOOL(p(144, s)), p, o, s)
# define _S1BOOSTPP_WHILE_144(p, o, s) _S1BOOSTPP_WHILE_144_C(_S1BOOSTPP_BOOL(p(145, s)), p, o, s)
# define _S1BOOSTPP_WHILE_145(p, o, s) _S1BOOSTPP_WHILE_145_C(_S1BOOSTPP_BOOL(p(146, s)), p, o, s)
# define _S1BOOSTPP_WHILE_146(p, o, s) _S1BOOSTPP_WHILE_146_C(_S1BOOSTPP_BOOL(p(147, s)), p, o, s)
# define _S1BOOSTPP_WHILE_147(p, o, s) _S1BOOSTPP_WHILE_147_C(_S1BOOSTPP_BOOL(p(148, s)), p, o, s)
# define _S1BOOSTPP_WHILE_148(p, o, s) _S1BOOSTPP_WHILE_148_C(_S1BOOSTPP_BOOL(p(149, s)), p, o, s)
# define _S1BOOSTPP_WHILE_149(p, o, s) _S1BOOSTPP_WHILE_149_C(_S1BOOSTPP_BOOL(p(150, s)), p, o, s)
# define _S1BOOSTPP_WHILE_150(p, o, s) _S1BOOSTPP_WHILE_150_C(_S1BOOSTPP_BOOL(p(151, s)), p, o, s)
# define _S1BOOSTPP_WHILE_151(p, o, s) _S1BOOSTPP_WHILE_151_C(_S1BOOSTPP_BOOL(p(152, s)), p, o, s)
# define _S1BOOSTPP_WHILE_152(p, o, s) _S1BOOSTPP_WHILE_152_C(_S1BOOSTPP_BOOL(p(153, s)), p, o, s)
# define _S1BOOSTPP_WHILE_153(p, o, s) _S1BOOSTPP_WHILE_153_C(_S1BOOSTPP_BOOL(p(154, s)), p, o, s)
# define _S1BOOSTPP_WHILE_154(p, o, s) _S1BOOSTPP_WHILE_154_C(_S1BOOSTPP_BOOL(p(155, s)), p, o, s)
# define _S1BOOSTPP_WHILE_155(p, o, s) _S1BOOSTPP_WHILE_155_C(_S1BOOSTPP_BOOL(p(156, s)), p, o, s)
# define _S1BOOSTPP_WHILE_156(p, o, s) _S1BOOSTPP_WHILE_156_C(_S1BOOSTPP_BOOL(p(157, s)), p, o, s)
# define _S1BOOSTPP_WHILE_157(p, o, s) _S1BOOSTPP_WHILE_157_C(_S1BOOSTPP_BOOL(p(158, s)), p, o, s)
# define _S1BOOSTPP_WHILE_158(p, o, s) _S1BOOSTPP_WHILE_158_C(_S1BOOSTPP_BOOL(p(159, s)), p, o, s)
# define _S1BOOSTPP_WHILE_159(p, o, s) _S1BOOSTPP_WHILE_159_C(_S1BOOSTPP_BOOL(p(160, s)), p, o, s)
# define _S1BOOSTPP_WHILE_160(p, o, s) _S1BOOSTPP_WHILE_160_C(_S1BOOSTPP_BOOL(p(161, s)), p, o, s)
# define _S1BOOSTPP_WHILE_161(p, o, s) _S1BOOSTPP_WHILE_161_C(_S1BOOSTPP_BOOL(p(162, s)), p, o, s)
# define _S1BOOSTPP_WHILE_162(p, o, s) _S1BOOSTPP_WHILE_162_C(_S1BOOSTPP_BOOL(p(163, s)), p, o, s)
# define _S1BOOSTPP_WHILE_163(p, o, s) _S1BOOSTPP_WHILE_163_C(_S1BOOSTPP_BOOL(p(164, s)), p, o, s)
# define _S1BOOSTPP_WHILE_164(p, o, s) _S1BOOSTPP_WHILE_164_C(_S1BOOSTPP_BOOL(p(165, s)), p, o, s)
# define _S1BOOSTPP_WHILE_165(p, o, s) _S1BOOSTPP_WHILE_165_C(_S1BOOSTPP_BOOL(p(166, s)), p, o, s)
# define _S1BOOSTPP_WHILE_166(p, o, s) _S1BOOSTPP_WHILE_166_C(_S1BOOSTPP_BOOL(p(167, s)), p, o, s)
# define _S1BOOSTPP_WHILE_167(p, o, s) _S1BOOSTPP_WHILE_167_C(_S1BOOSTPP_BOOL(p(168, s)), p, o, s)
# define _S1BOOSTPP_WHILE_168(p, o, s) _S1BOOSTPP_WHILE_168_C(_S1BOOSTPP_BOOL(p(169, s)), p, o, s)
# define _S1BOOSTPP_WHILE_169(p, o, s) _S1BOOSTPP_WHILE_169_C(_S1BOOSTPP_BOOL(p(170, s)), p, o, s)
# define _S1BOOSTPP_WHILE_170(p, o, s) _S1BOOSTPP_WHILE_170_C(_S1BOOSTPP_BOOL(p(171, s)), p, o, s)
# define _S1BOOSTPP_WHILE_171(p, o, s) _S1BOOSTPP_WHILE_171_C(_S1BOOSTPP_BOOL(p(172, s)), p, o, s)
# define _S1BOOSTPP_WHILE_172(p, o, s) _S1BOOSTPP_WHILE_172_C(_S1BOOSTPP_BOOL(p(173, s)), p, o, s)
# define _S1BOOSTPP_WHILE_173(p, o, s) _S1BOOSTPP_WHILE_173_C(_S1BOOSTPP_BOOL(p(174, s)), p, o, s)
# define _S1BOOSTPP_WHILE_174(p, o, s) _S1BOOSTPP_WHILE_174_C(_S1BOOSTPP_BOOL(p(175, s)), p, o, s)
# define _S1BOOSTPP_WHILE_175(p, o, s) _S1BOOSTPP_WHILE_175_C(_S1BOOSTPP_BOOL(p(176, s)), p, o, s)
# define _S1BOOSTPP_WHILE_176(p, o, s) _S1BOOSTPP_WHILE_176_C(_S1BOOSTPP_BOOL(p(177, s)), p, o, s)
# define _S1BOOSTPP_WHILE_177(p, o, s) _S1BOOSTPP_WHILE_177_C(_S1BOOSTPP_BOOL(p(178, s)), p, o, s)
# define _S1BOOSTPP_WHILE_178(p, o, s) _S1BOOSTPP_WHILE_178_C(_S1BOOSTPP_BOOL(p(179, s)), p, o, s)
# define _S1BOOSTPP_WHILE_179(p, o, s) _S1BOOSTPP_WHILE_179_C(_S1BOOSTPP_BOOL(p(180, s)), p, o, s)
# define _S1BOOSTPP_WHILE_180(p, o, s) _S1BOOSTPP_WHILE_180_C(_S1BOOSTPP_BOOL(p(181, s)), p, o, s)
# define _S1BOOSTPP_WHILE_181(p, o, s) _S1BOOSTPP_WHILE_181_C(_S1BOOSTPP_BOOL(p(182, s)), p, o, s)
# define _S1BOOSTPP_WHILE_182(p, o, s) _S1BOOSTPP_WHILE_182_C(_S1BOOSTPP_BOOL(p(183, s)), p, o, s)
# define _S1BOOSTPP_WHILE_183(p, o, s) _S1BOOSTPP_WHILE_183_C(_S1BOOSTPP_BOOL(p(184, s)), p, o, s)
# define _S1BOOSTPP_WHILE_184(p, o, s) _S1BOOSTPP_WHILE_184_C(_S1BOOSTPP_BOOL(p(185, s)), p, o, s)
# define _S1BOOSTPP_WHILE_185(p, o, s) _S1BOOSTPP_WHILE_185_C(_S1BOOSTPP_BOOL(p(186, s)), p, o, s)
# define _S1BOOSTPP_WHILE_186(p, o, s) _S1BOOSTPP_WHILE_186_C(_S1BOOSTPP_BOOL(p(187, s)), p, o, s)
# define _S1BOOSTPP_WHILE_187(p, o, s) _S1BOOSTPP_WHILE_187_C(_S1BOOSTPP_BOOL(p(188, s)), p, o, s)
# define _S1BOOSTPP_WHILE_188(p, o, s) _S1BOOSTPP_WHILE_188_C(_S1BOOSTPP_BOOL(p(189, s)), p, o, s)
# define _S1BOOSTPP_WHILE_189(p, o, s) _S1BOOSTPP_WHILE_189_C(_S1BOOSTPP_BOOL(p(190, s)), p, o, s)
# define _S1BOOSTPP_WHILE_190(p, o, s) _S1BOOSTPP_WHILE_190_C(_S1BOOSTPP_BOOL(p(191, s)), p, o, s)
# define _S1BOOSTPP_WHILE_191(p, o, s) _S1BOOSTPP_WHILE_191_C(_S1BOOSTPP_BOOL(p(192, s)), p, o, s)
# define _S1BOOSTPP_WHILE_192(p, o, s) _S1BOOSTPP_WHILE_192_C(_S1BOOSTPP_BOOL(p(193, s)), p, o, s)
# define _S1BOOSTPP_WHILE_193(p, o, s) _S1BOOSTPP_WHILE_193_C(_S1BOOSTPP_BOOL(p(194, s)), p, o, s)
# define _S1BOOSTPP_WHILE_194(p, o, s) _S1BOOSTPP_WHILE_194_C(_S1BOOSTPP_BOOL(p(195, s)), p, o, s)
# define _S1BOOSTPP_WHILE_195(p, o, s) _S1BOOSTPP_WHILE_195_C(_S1BOOSTPP_BOOL(p(196, s)), p, o, s)
# define _S1BOOSTPP_WHILE_196(p, o, s) _S1BOOSTPP_WHILE_196_C(_S1BOOSTPP_BOOL(p(197, s)), p, o, s)
# define _S1BOOSTPP_WHILE_197(p, o, s) _S1BOOSTPP_WHILE_197_C(_S1BOOSTPP_BOOL(p(198, s)), p, o, s)
# define _S1BOOSTPP_WHILE_198(p, o, s) _S1BOOSTPP_WHILE_198_C(_S1BOOSTPP_BOOL(p(199, s)), p, o, s)
# define _S1BOOSTPP_WHILE_199(p, o, s) _S1BOOSTPP_WHILE_199_C(_S1BOOSTPP_BOOL(p(200, s)), p, o, s)
# define _S1BOOSTPP_WHILE_200(p, o, s) _S1BOOSTPP_WHILE_200_C(_S1BOOSTPP_BOOL(p(201, s)), p, o, s)
# define _S1BOOSTPP_WHILE_201(p, o, s) _S1BOOSTPP_WHILE_201_C(_S1BOOSTPP_BOOL(p(202, s)), p, o, s)
# define _S1BOOSTPP_WHILE_202(p, o, s) _S1BOOSTPP_WHILE_202_C(_S1BOOSTPP_BOOL(p(203, s)), p, o, s)
# define _S1BOOSTPP_WHILE_203(p, o, s) _S1BOOSTPP_WHILE_203_C(_S1BOOSTPP_BOOL(p(204, s)), p, o, s)
# define _S1BOOSTPP_WHILE_204(p, o, s) _S1BOOSTPP_WHILE_204_C(_S1BOOSTPP_BOOL(p(205, s)), p, o, s)
# define _S1BOOSTPP_WHILE_205(p, o, s) _S1BOOSTPP_WHILE_205_C(_S1BOOSTPP_BOOL(p(206, s)), p, o, s)
# define _S1BOOSTPP_WHILE_206(p, o, s) _S1BOOSTPP_WHILE_206_C(_S1BOOSTPP_BOOL(p(207, s)), p, o, s)
# define _S1BOOSTPP_WHILE_207(p, o, s) _S1BOOSTPP_WHILE_207_C(_S1BOOSTPP_BOOL(p(208, s)), p, o, s)
# define _S1BOOSTPP_WHILE_208(p, o, s) _S1BOOSTPP_WHILE_208_C(_S1BOOSTPP_BOOL(p(209, s)), p, o, s)
# define _S1BOOSTPP_WHILE_209(p, o, s) _S1BOOSTPP_WHILE_209_C(_S1BOOSTPP_BOOL(p(210, s)), p, o, s)
# define _S1BOOSTPP_WHILE_210(p, o, s) _S1BOOSTPP_WHILE_210_C(_S1BOOSTPP_BOOL(p(211, s)), p, o, s)
# define _S1BOOSTPP_WHILE_211(p, o, s) _S1BOOSTPP_WHILE_211_C(_S1BOOSTPP_BOOL(p(212, s)), p, o, s)
# define _S1BOOSTPP_WHILE_212(p, o, s) _S1BOOSTPP_WHILE_212_C(_S1BOOSTPP_BOOL(p(213, s)), p, o, s)
# define _S1BOOSTPP_WHILE_213(p, o, s) _S1BOOSTPP_WHILE_213_C(_S1BOOSTPP_BOOL(p(214, s)), p, o, s)
# define _S1BOOSTPP_WHILE_214(p, o, s) _S1BOOSTPP_WHILE_214_C(_S1BOOSTPP_BOOL(p(215, s)), p, o, s)
# define _S1BOOSTPP_WHILE_215(p, o, s) _S1BOOSTPP_WHILE_215_C(_S1BOOSTPP_BOOL(p(216, s)), p, o, s)
# define _S1BOOSTPP_WHILE_216(p, o, s) _S1BOOSTPP_WHILE_216_C(_S1BOOSTPP_BOOL(p(217, s)), p, o, s)
# define _S1BOOSTPP_WHILE_217(p, o, s) _S1BOOSTPP_WHILE_217_C(_S1BOOSTPP_BOOL(p(218, s)), p, o, s)
# define _S1BOOSTPP_WHILE_218(p, o, s) _S1BOOSTPP_WHILE_218_C(_S1BOOSTPP_BOOL(p(219, s)), p, o, s)
# define _S1BOOSTPP_WHILE_219(p, o, s) _S1BOOSTPP_WHILE_219_C(_S1BOOSTPP_BOOL(p(220, s)), p, o, s)
# define _S1BOOSTPP_WHILE_220(p, o, s) _S1BOOSTPP_WHILE_220_C(_S1BOOSTPP_BOOL(p(221, s)), p, o, s)
# define _S1BOOSTPP_WHILE_221(p, o, s) _S1BOOSTPP_WHILE_221_C(_S1BOOSTPP_BOOL(p(222, s)), p, o, s)
# define _S1BOOSTPP_WHILE_222(p, o, s) _S1BOOSTPP_WHILE_222_C(_S1BOOSTPP_BOOL(p(223, s)), p, o, s)
# define _S1BOOSTPP_WHILE_223(p, o, s) _S1BOOSTPP_WHILE_223_C(_S1BOOSTPP_BOOL(p(224, s)), p, o, s)
# define _S1BOOSTPP_WHILE_224(p, o, s) _S1BOOSTPP_WHILE_224_C(_S1BOOSTPP_BOOL(p(225, s)), p, o, s)
# define _S1BOOSTPP_WHILE_225(p, o, s) _S1BOOSTPP_WHILE_225_C(_S1BOOSTPP_BOOL(p(226, s)), p, o, s)
# define _S1BOOSTPP_WHILE_226(p, o, s) _S1BOOSTPP_WHILE_226_C(_S1BOOSTPP_BOOL(p(227, s)), p, o, s)
# define _S1BOOSTPP_WHILE_227(p, o, s) _S1BOOSTPP_WHILE_227_C(_S1BOOSTPP_BOOL(p(228, s)), p, o, s)
# define _S1BOOSTPP_WHILE_228(p, o, s) _S1BOOSTPP_WHILE_228_C(_S1BOOSTPP_BOOL(p(229, s)), p, o, s)
# define _S1BOOSTPP_WHILE_229(p, o, s) _S1BOOSTPP_WHILE_229_C(_S1BOOSTPP_BOOL(p(230, s)), p, o, s)
# define _S1BOOSTPP_WHILE_230(p, o, s) _S1BOOSTPP_WHILE_230_C(_S1BOOSTPP_BOOL(p(231, s)), p, o, s)
# define _S1BOOSTPP_WHILE_231(p, o, s) _S1BOOSTPP_WHILE_231_C(_S1BOOSTPP_BOOL(p(232, s)), p, o, s)
# define _S1BOOSTPP_WHILE_232(p, o, s) _S1BOOSTPP_WHILE_232_C(_S1BOOSTPP_BOOL(p(233, s)), p, o, s)
# define _S1BOOSTPP_WHILE_233(p, o, s) _S1BOOSTPP_WHILE_233_C(_S1BOOSTPP_BOOL(p(234, s)), p, o, s)
# define _S1BOOSTPP_WHILE_234(p, o, s) _S1BOOSTPP_WHILE_234_C(_S1BOOSTPP_BOOL(p(235, s)), p, o, s)
# define _S1BOOSTPP_WHILE_235(p, o, s) _S1BOOSTPP_WHILE_235_C(_S1BOOSTPP_BOOL(p(236, s)), p, o, s)
# define _S1BOOSTPP_WHILE_236(p, o, s) _S1BOOSTPP_WHILE_236_C(_S1BOOSTPP_BOOL(p(237, s)), p, o, s)
# define _S1BOOSTPP_WHILE_237(p, o, s) _S1BOOSTPP_WHILE_237_C(_S1BOOSTPP_BOOL(p(238, s)), p, o, s)
# define _S1BOOSTPP_WHILE_238(p, o, s) _S1BOOSTPP_WHILE_238_C(_S1BOOSTPP_BOOL(p(239, s)), p, o, s)
# define _S1BOOSTPP_WHILE_239(p, o, s) _S1BOOSTPP_WHILE_239_C(_S1BOOSTPP_BOOL(p(240, s)), p, o, s)
# define _S1BOOSTPP_WHILE_240(p, o, s) _S1BOOSTPP_WHILE_240_C(_S1BOOSTPP_BOOL(p(241, s)), p, o, s)
# define _S1BOOSTPP_WHILE_241(p, o, s) _S1BOOSTPP_WHILE_241_C(_S1BOOSTPP_BOOL(p(242, s)), p, o, s)
# define _S1BOOSTPP_WHILE_242(p, o, s) _S1BOOSTPP_WHILE_242_C(_S1BOOSTPP_BOOL(p(243, s)), p, o, s)
# define _S1BOOSTPP_WHILE_243(p, o, s) _S1BOOSTPP_WHILE_243_C(_S1BOOSTPP_BOOL(p(244, s)), p, o, s)
# define _S1BOOSTPP_WHILE_244(p, o, s) _S1BOOSTPP_WHILE_244_C(_S1BOOSTPP_BOOL(p(245, s)), p, o, s)
# define _S1BOOSTPP_WHILE_245(p, o, s) _S1BOOSTPP_WHILE_245_C(_S1BOOSTPP_BOOL(p(246, s)), p, o, s)
# define _S1BOOSTPP_WHILE_246(p, o, s) _S1BOOSTPP_WHILE_246_C(_S1BOOSTPP_BOOL(p(247, s)), p, o, s)
# define _S1BOOSTPP_WHILE_247(p, o, s) _S1BOOSTPP_WHILE_247_C(_S1BOOSTPP_BOOL(p(248, s)), p, o, s)
# define _S1BOOSTPP_WHILE_248(p, o, s) _S1BOOSTPP_WHILE_248_C(_S1BOOSTPP_BOOL(p(249, s)), p, o, s)
# define _S1BOOSTPP_WHILE_249(p, o, s) _S1BOOSTPP_WHILE_249_C(_S1BOOSTPP_BOOL(p(250, s)), p, o, s)
# define _S1BOOSTPP_WHILE_250(p, o, s) _S1BOOSTPP_WHILE_250_C(_S1BOOSTPP_BOOL(p(251, s)), p, o, s)
# define _S1BOOSTPP_WHILE_251(p, o, s) _S1BOOSTPP_WHILE_251_C(_S1BOOSTPP_BOOL(p(252, s)), p, o, s)
# define _S1BOOSTPP_WHILE_252(p, o, s) _S1BOOSTPP_WHILE_252_C(_S1BOOSTPP_BOOL(p(253, s)), p, o, s)
# define _S1BOOSTPP_WHILE_253(p, o, s) _S1BOOSTPP_WHILE_253_C(_S1BOOSTPP_BOOL(p(254, s)), p, o, s)
# define _S1BOOSTPP_WHILE_254(p, o, s) _S1BOOSTPP_WHILE_254_C(_S1BOOSTPP_BOOL(p(255, s)), p, o, s)
# define _S1BOOSTPP_WHILE_255(p, o, s) _S1BOOSTPP_WHILE_255_C(_S1BOOSTPP_BOOL(p(256, s)), p, o, s)
# define _S1BOOSTPP_WHILE_256(p, o, s) _S1BOOSTPP_WHILE_256_C(_S1BOOSTPP_BOOL(p(257, s)), p, o, s)
#
# define _S1BOOSTPP_WHILE_1_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_2, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(2, s))
# define _S1BOOSTPP_WHILE_2_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_3, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(3, s))
# define _S1BOOSTPP_WHILE_3_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_4, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(4, s))
# define _S1BOOSTPP_WHILE_4_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_5, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(5, s))
# define _S1BOOSTPP_WHILE_5_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_6, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(6, s))
# define _S1BOOSTPP_WHILE_6_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_7, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(7, s))
# define _S1BOOSTPP_WHILE_7_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_8, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(8, s))
# define _S1BOOSTPP_WHILE_8_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_9, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(9, s))
# define _S1BOOSTPP_WHILE_9_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_10, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(10, s))
# define _S1BOOSTPP_WHILE_10_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_11, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(11, s))
# define _S1BOOSTPP_WHILE_11_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_12, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(12, s))
# define _S1BOOSTPP_WHILE_12_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_13, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(13, s))
# define _S1BOOSTPP_WHILE_13_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_14, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(14, s))
# define _S1BOOSTPP_WHILE_14_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_15, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(15, s))
# define _S1BOOSTPP_WHILE_15_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_16, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(16, s))
# define _S1BOOSTPP_WHILE_16_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_17, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(17, s))
# define _S1BOOSTPP_WHILE_17_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_18, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(18, s))
# define _S1BOOSTPP_WHILE_18_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_19, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(19, s))
# define _S1BOOSTPP_WHILE_19_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_20, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(20, s))
# define _S1BOOSTPP_WHILE_20_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_21, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(21, s))
# define _S1BOOSTPP_WHILE_21_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_22, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(22, s))
# define _S1BOOSTPP_WHILE_22_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_23, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(23, s))
# define _S1BOOSTPP_WHILE_23_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_24, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(24, s))
# define _S1BOOSTPP_WHILE_24_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_25, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(25, s))
# define _S1BOOSTPP_WHILE_25_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_26, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(26, s))
# define _S1BOOSTPP_WHILE_26_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_27, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(27, s))
# define _S1BOOSTPP_WHILE_27_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_28, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(28, s))
# define _S1BOOSTPP_WHILE_28_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_29, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(29, s))
# define _S1BOOSTPP_WHILE_29_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_30, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(30, s))
# define _S1BOOSTPP_WHILE_30_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_31, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(31, s))
# define _S1BOOSTPP_WHILE_31_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_32, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(32, s))
# define _S1BOOSTPP_WHILE_32_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_33, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(33, s))
# define _S1BOOSTPP_WHILE_33_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_34, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(34, s))
# define _S1BOOSTPP_WHILE_34_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_35, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(35, s))
# define _S1BOOSTPP_WHILE_35_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_36, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(36, s))
# define _S1BOOSTPP_WHILE_36_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_37, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(37, s))
# define _S1BOOSTPP_WHILE_37_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_38, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(38, s))
# define _S1BOOSTPP_WHILE_38_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_39, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(39, s))
# define _S1BOOSTPP_WHILE_39_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_40, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(40, s))
# define _S1BOOSTPP_WHILE_40_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_41, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(41, s))
# define _S1BOOSTPP_WHILE_41_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_42, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(42, s))
# define _S1BOOSTPP_WHILE_42_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_43, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(43, s))
# define _S1BOOSTPP_WHILE_43_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_44, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(44, s))
# define _S1BOOSTPP_WHILE_44_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_45, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(45, s))
# define _S1BOOSTPP_WHILE_45_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_46, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(46, s))
# define _S1BOOSTPP_WHILE_46_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_47, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(47, s))
# define _S1BOOSTPP_WHILE_47_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_48, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(48, s))
# define _S1BOOSTPP_WHILE_48_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_49, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(49, s))
# define _S1BOOSTPP_WHILE_49_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_50, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(50, s))
# define _S1BOOSTPP_WHILE_50_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_51, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(51, s))
# define _S1BOOSTPP_WHILE_51_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_52, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(52, s))
# define _S1BOOSTPP_WHILE_52_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_53, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(53, s))
# define _S1BOOSTPP_WHILE_53_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_54, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(54, s))
# define _S1BOOSTPP_WHILE_54_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_55, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(55, s))
# define _S1BOOSTPP_WHILE_55_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_56, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(56, s))
# define _S1BOOSTPP_WHILE_56_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_57, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(57, s))
# define _S1BOOSTPP_WHILE_57_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_58, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(58, s))
# define _S1BOOSTPP_WHILE_58_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_59, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(59, s))
# define _S1BOOSTPP_WHILE_59_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_60, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(60, s))
# define _S1BOOSTPP_WHILE_60_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_61, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(61, s))
# define _S1BOOSTPP_WHILE_61_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_62, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(62, s))
# define _S1BOOSTPP_WHILE_62_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_63, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(63, s))
# define _S1BOOSTPP_WHILE_63_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_64, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(64, s))
# define _S1BOOSTPP_WHILE_64_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_65, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(65, s))
# define _S1BOOSTPP_WHILE_65_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_66, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(66, s))
# define _S1BOOSTPP_WHILE_66_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_67, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(67, s))
# define _S1BOOSTPP_WHILE_67_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_68, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(68, s))
# define _S1BOOSTPP_WHILE_68_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_69, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(69, s))
# define _S1BOOSTPP_WHILE_69_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_70, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(70, s))
# define _S1BOOSTPP_WHILE_70_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_71, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(71, s))
# define _S1BOOSTPP_WHILE_71_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_72, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(72, s))
# define _S1BOOSTPP_WHILE_72_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_73, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(73, s))
# define _S1BOOSTPP_WHILE_73_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_74, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(74, s))
# define _S1BOOSTPP_WHILE_74_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_75, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(75, s))
# define _S1BOOSTPP_WHILE_75_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_76, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(76, s))
# define _S1BOOSTPP_WHILE_76_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_77, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(77, s))
# define _S1BOOSTPP_WHILE_77_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_78, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(78, s))
# define _S1BOOSTPP_WHILE_78_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_79, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(79, s))
# define _S1BOOSTPP_WHILE_79_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_80, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(80, s))
# define _S1BOOSTPP_WHILE_80_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_81, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(81, s))
# define _S1BOOSTPP_WHILE_81_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_82, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(82, s))
# define _S1BOOSTPP_WHILE_82_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_83, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(83, s))
# define _S1BOOSTPP_WHILE_83_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_84, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(84, s))
# define _S1BOOSTPP_WHILE_84_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_85, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(85, s))
# define _S1BOOSTPP_WHILE_85_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_86, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(86, s))
# define _S1BOOSTPP_WHILE_86_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_87, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(87, s))
# define _S1BOOSTPP_WHILE_87_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_88, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(88, s))
# define _S1BOOSTPP_WHILE_88_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_89, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(89, s))
# define _S1BOOSTPP_WHILE_89_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_90, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(90, s))
# define _S1BOOSTPP_WHILE_90_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_91, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(91, s))
# define _S1BOOSTPP_WHILE_91_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_92, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(92, s))
# define _S1BOOSTPP_WHILE_92_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_93, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(93, s))
# define _S1BOOSTPP_WHILE_93_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_94, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(94, s))
# define _S1BOOSTPP_WHILE_94_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_95, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(95, s))
# define _S1BOOSTPP_WHILE_95_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_96, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(96, s))
# define _S1BOOSTPP_WHILE_96_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_97, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(97, s))
# define _S1BOOSTPP_WHILE_97_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_98, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(98, s))
# define _S1BOOSTPP_WHILE_98_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_99, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(99, s))
# define _S1BOOSTPP_WHILE_99_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_100, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(100, s))
# define _S1BOOSTPP_WHILE_100_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_101, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(101, s))
# define _S1BOOSTPP_WHILE_101_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_102, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(102, s))
# define _S1BOOSTPP_WHILE_102_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_103, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(103, s))
# define _S1BOOSTPP_WHILE_103_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_104, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(104, s))
# define _S1BOOSTPP_WHILE_104_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_105, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(105, s))
# define _S1BOOSTPP_WHILE_105_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_106, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(106, s))
# define _S1BOOSTPP_WHILE_106_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_107, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(107, s))
# define _S1BOOSTPP_WHILE_107_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_108, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(108, s))
# define _S1BOOSTPP_WHILE_108_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_109, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(109, s))
# define _S1BOOSTPP_WHILE_109_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_110, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(110, s))
# define _S1BOOSTPP_WHILE_110_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_111, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(111, s))
# define _S1BOOSTPP_WHILE_111_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_112, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(112, s))
# define _S1BOOSTPP_WHILE_112_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_113, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(113, s))
# define _S1BOOSTPP_WHILE_113_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_114, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(114, s))
# define _S1BOOSTPP_WHILE_114_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_115, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(115, s))
# define _S1BOOSTPP_WHILE_115_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_116, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(116, s))
# define _S1BOOSTPP_WHILE_116_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_117, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(117, s))
# define _S1BOOSTPP_WHILE_117_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_118, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(118, s))
# define _S1BOOSTPP_WHILE_118_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_119, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(119, s))
# define _S1BOOSTPP_WHILE_119_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_120, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(120, s))
# define _S1BOOSTPP_WHILE_120_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_121, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(121, s))
# define _S1BOOSTPP_WHILE_121_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_122, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(122, s))
# define _S1BOOSTPP_WHILE_122_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_123, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(123, s))
# define _S1BOOSTPP_WHILE_123_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_124, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(124, s))
# define _S1BOOSTPP_WHILE_124_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_125, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(125, s))
# define _S1BOOSTPP_WHILE_125_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_126, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(126, s))
# define _S1BOOSTPP_WHILE_126_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_127, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(127, s))
# define _S1BOOSTPP_WHILE_127_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_128, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(128, s))
# define _S1BOOSTPP_WHILE_128_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_129, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(129, s))
# define _S1BOOSTPP_WHILE_129_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_130, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(130, s))
# define _S1BOOSTPP_WHILE_130_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_131, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(131, s))
# define _S1BOOSTPP_WHILE_131_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_132, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(132, s))
# define _S1BOOSTPP_WHILE_132_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_133, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(133, s))
# define _S1BOOSTPP_WHILE_133_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_134, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(134, s))
# define _S1BOOSTPP_WHILE_134_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_135, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(135, s))
# define _S1BOOSTPP_WHILE_135_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_136, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(136, s))
# define _S1BOOSTPP_WHILE_136_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_137, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(137, s))
# define _S1BOOSTPP_WHILE_137_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_138, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(138, s))
# define _S1BOOSTPP_WHILE_138_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_139, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(139, s))
# define _S1BOOSTPP_WHILE_139_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_140, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(140, s))
# define _S1BOOSTPP_WHILE_140_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_141, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(141, s))
# define _S1BOOSTPP_WHILE_141_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_142, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(142, s))
# define _S1BOOSTPP_WHILE_142_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_143, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(143, s))
# define _S1BOOSTPP_WHILE_143_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_144, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(144, s))
# define _S1BOOSTPP_WHILE_144_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_145, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(145, s))
# define _S1BOOSTPP_WHILE_145_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_146, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(146, s))
# define _S1BOOSTPP_WHILE_146_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_147, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(147, s))
# define _S1BOOSTPP_WHILE_147_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_148, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(148, s))
# define _S1BOOSTPP_WHILE_148_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_149, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(149, s))
# define _S1BOOSTPP_WHILE_149_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_150, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(150, s))
# define _S1BOOSTPP_WHILE_150_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_151, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(151, s))
# define _S1BOOSTPP_WHILE_151_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_152, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(152, s))
# define _S1BOOSTPP_WHILE_152_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_153, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(153, s))
# define _S1BOOSTPP_WHILE_153_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_154, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(154, s))
# define _S1BOOSTPP_WHILE_154_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_155, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(155, s))
# define _S1BOOSTPP_WHILE_155_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_156, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(156, s))
# define _S1BOOSTPP_WHILE_156_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_157, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(157, s))
# define _S1BOOSTPP_WHILE_157_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_158, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(158, s))
# define _S1BOOSTPP_WHILE_158_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_159, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(159, s))
# define _S1BOOSTPP_WHILE_159_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_160, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(160, s))
# define _S1BOOSTPP_WHILE_160_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_161, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(161, s))
# define _S1BOOSTPP_WHILE_161_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_162, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(162, s))
# define _S1BOOSTPP_WHILE_162_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_163, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(163, s))
# define _S1BOOSTPP_WHILE_163_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_164, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(164, s))
# define _S1BOOSTPP_WHILE_164_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_165, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(165, s))
# define _S1BOOSTPP_WHILE_165_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_166, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(166, s))
# define _S1BOOSTPP_WHILE_166_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_167, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(167, s))
# define _S1BOOSTPP_WHILE_167_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_168, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(168, s))
# define _S1BOOSTPP_WHILE_168_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_169, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(169, s))
# define _S1BOOSTPP_WHILE_169_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_170, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(170, s))
# define _S1BOOSTPP_WHILE_170_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_171, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(171, s))
# define _S1BOOSTPP_WHILE_171_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_172, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(172, s))
# define _S1BOOSTPP_WHILE_172_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_173, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(173, s))
# define _S1BOOSTPP_WHILE_173_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_174, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(174, s))
# define _S1BOOSTPP_WHILE_174_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_175, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(175, s))
# define _S1BOOSTPP_WHILE_175_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_176, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(176, s))
# define _S1BOOSTPP_WHILE_176_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_177, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(177, s))
# define _S1BOOSTPP_WHILE_177_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_178, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(178, s))
# define _S1BOOSTPP_WHILE_178_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_179, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(179, s))
# define _S1BOOSTPP_WHILE_179_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_180, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(180, s))
# define _S1BOOSTPP_WHILE_180_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_181, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(181, s))
# define _S1BOOSTPP_WHILE_181_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_182, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(182, s))
# define _S1BOOSTPP_WHILE_182_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_183, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(183, s))
# define _S1BOOSTPP_WHILE_183_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_184, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(184, s))
# define _S1BOOSTPP_WHILE_184_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_185, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(185, s))
# define _S1BOOSTPP_WHILE_185_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_186, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(186, s))
# define _S1BOOSTPP_WHILE_186_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_187, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(187, s))
# define _S1BOOSTPP_WHILE_187_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_188, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(188, s))
# define _S1BOOSTPP_WHILE_188_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_189, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(189, s))
# define _S1BOOSTPP_WHILE_189_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_190, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(190, s))
# define _S1BOOSTPP_WHILE_190_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_191, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(191, s))
# define _S1BOOSTPP_WHILE_191_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_192, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(192, s))
# define _S1BOOSTPP_WHILE_192_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_193, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(193, s))
# define _S1BOOSTPP_WHILE_193_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_194, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(194, s))
# define _S1BOOSTPP_WHILE_194_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_195, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(195, s))
# define _S1BOOSTPP_WHILE_195_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_196, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(196, s))
# define _S1BOOSTPP_WHILE_196_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_197, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(197, s))
# define _S1BOOSTPP_WHILE_197_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_198, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(198, s))
# define _S1BOOSTPP_WHILE_198_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_199, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(199, s))
# define _S1BOOSTPP_WHILE_199_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_200, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(200, s))
# define _S1BOOSTPP_WHILE_200_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_201, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(201, s))
# define _S1BOOSTPP_WHILE_201_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_202, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(202, s))
# define _S1BOOSTPP_WHILE_202_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_203, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(203, s))
# define _S1BOOSTPP_WHILE_203_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_204, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(204, s))
# define _S1BOOSTPP_WHILE_204_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_205, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(205, s))
# define _S1BOOSTPP_WHILE_205_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_206, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(206, s))
# define _S1BOOSTPP_WHILE_206_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_207, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(207, s))
# define _S1BOOSTPP_WHILE_207_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_208, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(208, s))
# define _S1BOOSTPP_WHILE_208_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_209, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(209, s))
# define _S1BOOSTPP_WHILE_209_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_210, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(210, s))
# define _S1BOOSTPP_WHILE_210_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_211, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(211, s))
# define _S1BOOSTPP_WHILE_211_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_212, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(212, s))
# define _S1BOOSTPP_WHILE_212_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_213, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(213, s))
# define _S1BOOSTPP_WHILE_213_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_214, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(214, s))
# define _S1BOOSTPP_WHILE_214_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_215, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(215, s))
# define _S1BOOSTPP_WHILE_215_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_216, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(216, s))
# define _S1BOOSTPP_WHILE_216_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_217, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(217, s))
# define _S1BOOSTPP_WHILE_217_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_218, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(218, s))
# define _S1BOOSTPP_WHILE_218_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_219, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(219, s))
# define _S1BOOSTPP_WHILE_219_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_220, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(220, s))
# define _S1BOOSTPP_WHILE_220_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_221, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(221, s))
# define _S1BOOSTPP_WHILE_221_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_222, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(222, s))
# define _S1BOOSTPP_WHILE_222_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_223, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(223, s))
# define _S1BOOSTPP_WHILE_223_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_224, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(224, s))
# define _S1BOOSTPP_WHILE_224_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_225, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(225, s))
# define _S1BOOSTPP_WHILE_225_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_226, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(226, s))
# define _S1BOOSTPP_WHILE_226_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_227, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(227, s))
# define _S1BOOSTPP_WHILE_227_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_228, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(228, s))
# define _S1BOOSTPP_WHILE_228_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_229, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(229, s))
# define _S1BOOSTPP_WHILE_229_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_230, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(230, s))
# define _S1BOOSTPP_WHILE_230_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_231, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(231, s))
# define _S1BOOSTPP_WHILE_231_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_232, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(232, s))
# define _S1BOOSTPP_WHILE_232_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_233, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(233, s))
# define _S1BOOSTPP_WHILE_233_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_234, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(234, s))
# define _S1BOOSTPP_WHILE_234_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_235, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(235, s))
# define _S1BOOSTPP_WHILE_235_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_236, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(236, s))
# define _S1BOOSTPP_WHILE_236_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_237, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(237, s))
# define _S1BOOSTPP_WHILE_237_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_238, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(238, s))
# define _S1BOOSTPP_WHILE_238_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_239, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(239, s))
# define _S1BOOSTPP_WHILE_239_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_240, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(240, s))
# define _S1BOOSTPP_WHILE_240_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_241, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(241, s))
# define _S1BOOSTPP_WHILE_241_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_242, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(242, s))
# define _S1BOOSTPP_WHILE_242_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_243, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(243, s))
# define _S1BOOSTPP_WHILE_243_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_244, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(244, s))
# define _S1BOOSTPP_WHILE_244_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_245, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(245, s))
# define _S1BOOSTPP_WHILE_245_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_246, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(246, s))
# define _S1BOOSTPP_WHILE_246_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_247, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(247, s))
# define _S1BOOSTPP_WHILE_247_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_248, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(248, s))
# define _S1BOOSTPP_WHILE_248_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_249, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(249, s))
# define _S1BOOSTPP_WHILE_249_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_250, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(250, s))
# define _S1BOOSTPP_WHILE_250_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_251, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(251, s))
# define _S1BOOSTPP_WHILE_251_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_252, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(252, s))
# define _S1BOOSTPP_WHILE_252_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_253, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(253, s))
# define _S1BOOSTPP_WHILE_253_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_254, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(254, s))
# define _S1BOOSTPP_WHILE_254_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_255, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(255, s))
# define _S1BOOSTPP_WHILE_255_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_256, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(256, s))
# define _S1BOOSTPP_WHILE_256_C(c, p, o, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_WHILE_257, s _S1BOOSTPP_TUPLE_EAT_3)(p, o, _S1BOOSTPP_IIF(c, o, _S1BOOSTPP_NIL _S1BOOSTPP_TUPLE_EAT_2)(257, s))
#
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_CONTROL_WHILE_HPP
# define _S1BOOSTPP_CONTROL_WHILE_HPP
#
#
# /* _S1BOOSTPP_WHILE */
#
# if 0
#    define _S1BOOSTPP_WHILE(pred, op, state)
# endif
#
# define _S1BOOSTPP_WHILE _S1BOOSTPP_CAT(_S1BOOSTPP_WHILE_, _S1BOOSTPP_AUTO_REC(_S1BOOSTPP_WHILE_P, 256))
#
# if _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#    define _S1BOOSTPP_WHILE_P(n) _S1BOOSTPP_BITAND(_S1BOOSTPP_CAT(_S1BOOSTPP_WHILE_CHECK_, _S1BOOSTPP_WHILE_ ## n(_S1BOOSTPP_WHILE_F, _S1BOOSTPP_NIL, _S1BOOSTPP_NIL)), _S1BOOSTPP_BITAND(_S1BOOSTPP_CAT(_S1BOOSTPP_LIST_FOLD_LEFT_CHECK_, _S1BOOSTPP_LIST_FOLD_LEFT_ ## n(_S1BOOSTPP_NIL, _S1BOOSTPP_NIL, _S1BOOSTPP_NIL)), _S1BOOSTPP_CAT(_S1BOOSTPP_LIST_FOLD_RIGHT_CHECK_, _S1BOOSTPP_LIST_FOLD_RIGHT_ ## n(_S1BOOSTPP_NIL, _S1BOOSTPP_NIL, _S1BOOSTPP_NIL))))
# else
#    define _S1BOOSTPP_WHILE_P(n) _S1BOOSTPP_BITAND(_S1BOOSTPP_CAT(_S1BOOSTPP_WHILE_CHECK_, _S1BOOSTPP_WHILE_ ## n(_S1BOOSTPP_WHILE_F, _S1BOOSTPP_NIL, _S1BOOSTPP_NIL)), _S1BOOSTPP_CAT(_S1BOOSTPP_LIST_FOLD_LEFT_CHECK_, _S1BOOSTPP_LIST_FOLD_LEFT_ ## n(_S1BOOSTPP_NIL, _S1BOOSTPP_NIL, _S1BOOSTPP_NIL)))
# endif
#
# define _S1BOOSTPP_WHILE_F(d, _) 0
#
# if _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
# elif _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC()
# elif _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_DMC()
# else
# endif
#
# define _S1BOOSTPP_WHILE_257(p, o, s) _S1BOOSTPP_ERROR(0x0001)
#
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_NIL 1
#
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_1(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_2(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_3(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_4(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_5(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_6(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_7(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_8(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_9(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_10(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_11(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_12(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_13(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_14(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_15(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_16(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_17(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_18(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_19(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_20(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_21(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_22(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_23(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_24(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_25(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_26(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_27(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_28(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_29(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_30(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_31(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_32(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_33(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_34(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_35(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_36(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_37(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_38(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_39(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_40(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_41(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_42(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_43(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_44(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_45(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_46(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_47(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_48(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_49(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_50(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_51(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_52(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_53(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_54(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_55(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_56(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_57(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_58(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_59(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_60(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_61(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_62(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_63(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_64(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_65(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_66(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_67(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_68(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_69(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_70(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_71(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_72(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_73(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_74(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_75(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_76(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_77(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_78(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_79(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_80(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_81(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_82(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_83(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_84(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_85(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_86(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_87(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_88(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_89(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_90(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_91(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_92(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_93(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_94(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_95(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_96(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_97(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_98(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_99(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_100(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_101(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_102(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_103(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_104(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_105(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_106(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_107(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_108(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_109(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_110(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_111(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_112(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_113(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_114(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_115(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_116(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_117(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_118(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_119(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_120(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_121(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_122(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_123(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_124(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_125(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_126(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_127(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_128(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_129(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_130(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_131(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_132(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_133(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_134(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_135(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_136(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_137(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_138(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_139(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_140(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_141(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_142(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_143(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_144(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_145(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_146(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_147(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_148(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_149(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_150(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_151(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_152(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_153(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_154(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_155(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_156(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_157(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_158(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_159(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_160(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_161(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_162(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_163(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_164(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_165(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_166(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_167(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_168(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_169(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_170(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_171(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_172(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_173(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_174(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_175(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_176(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_177(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_178(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_179(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_180(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_181(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_182(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_183(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_184(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_185(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_186(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_187(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_188(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_189(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_190(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_191(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_192(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_193(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_194(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_195(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_196(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_197(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_198(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_199(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_200(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_201(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_202(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_203(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_204(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_205(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_206(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_207(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_208(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_209(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_210(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_211(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_212(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_213(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_214(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_215(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_216(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_217(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_218(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_219(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_220(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_221(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_222(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_223(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_224(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_225(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_226(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_227(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_228(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_229(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_230(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_231(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_232(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_233(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_234(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_235(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_236(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_237(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_238(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_239(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_240(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_241(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_242(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_243(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_244(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_245(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_246(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_247(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_248(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_249(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_250(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_251(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_252(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_253(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_254(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_255(p, o, s) 0
# define _S1BOOSTPP_WHILE_CHECK__S1BOOSTPP_WHILE_256(p, o, s) 0
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LIST_REVERSE_HPP
# define _S1BOOSTPP_LIST_REVERSE_HPP
#
#
# /* _S1BOOSTPP_LIST_REVERSE */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#    define _S1BOOSTPP_LIST_REVERSE(list) _S1BOOSTPP_LIST_FOLD_LEFT(_S1BOOSTPP_LIST_REVERSE_O, _S1BOOSTPP_NIL, list)
# else
#    define _S1BOOSTPP_LIST_REVERSE(list) _S1BOOSTPP_LIST_REVERSE_I(list)
#    define _S1BOOSTPP_LIST_REVERSE_I(list) _S1BOOSTPP_LIST_FOLD_LEFT(_S1BOOSTPP_LIST_REVERSE_O, _S1BOOSTPP_NIL, list)
# endif
#
# define _S1BOOSTPP_LIST_REVERSE_O(d, s, x) (x, s)
#
# /* _S1BOOSTPP_LIST_REVERSE_D */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#    define _S1BOOSTPP_LIST_REVERSE_D(d, list) _S1BOOSTPP_LIST_FOLD_LEFT_ ## d(_S1BOOSTPP_LIST_REVERSE_O, _S1BOOSTPP_NIL, list)
# else
#    define _S1BOOSTPP_LIST_REVERSE_D(d, list) _S1BOOSTPP_LIST_REVERSE_D_I(d, list)
#    define _S1BOOSTPP_LIST_REVERSE_D_I(d, list) _S1BOOSTPP_LIST_FOLD_LEFT_ ## d(_S1BOOSTPP_LIST_REVERSE_O, _S1BOOSTPP_NIL, list)
# endif
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LIST_DETAIL_FOLD_RIGHT_HPP
# define _S1BOOSTPP_LIST_DETAIL_FOLD_RIGHT_HPP
#
#
# define _S1BOOSTPP_LIST_FOLD_RIGHT_1(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_1(o, s, _S1BOOSTPP_LIST_REVERSE_D(1, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_2(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_2(o, s, _S1BOOSTPP_LIST_REVERSE_D(2, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_3(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_3(o, s, _S1BOOSTPP_LIST_REVERSE_D(3, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_4(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_4(o, s, _S1BOOSTPP_LIST_REVERSE_D(4, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_5(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_5(o, s, _S1BOOSTPP_LIST_REVERSE_D(5, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_6(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_6(o, s, _S1BOOSTPP_LIST_REVERSE_D(6, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_7(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_7(o, s, _S1BOOSTPP_LIST_REVERSE_D(7, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_8(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_8(o, s, _S1BOOSTPP_LIST_REVERSE_D(8, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_9(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_9(o, s, _S1BOOSTPP_LIST_REVERSE_D(9, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_10(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_10(o, s, _S1BOOSTPP_LIST_REVERSE_D(10, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_11(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_11(o, s, _S1BOOSTPP_LIST_REVERSE_D(11, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_12(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_12(o, s, _S1BOOSTPP_LIST_REVERSE_D(12, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_13(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_13(o, s, _S1BOOSTPP_LIST_REVERSE_D(13, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_14(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_14(o, s, _S1BOOSTPP_LIST_REVERSE_D(14, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_15(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_15(o, s, _S1BOOSTPP_LIST_REVERSE_D(15, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_16(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_16(o, s, _S1BOOSTPP_LIST_REVERSE_D(16, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_17(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_17(o, s, _S1BOOSTPP_LIST_REVERSE_D(17, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_18(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_18(o, s, _S1BOOSTPP_LIST_REVERSE_D(18, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_19(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_19(o, s, _S1BOOSTPP_LIST_REVERSE_D(19, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_20(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_20(o, s, _S1BOOSTPP_LIST_REVERSE_D(20, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_21(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_21(o, s, _S1BOOSTPP_LIST_REVERSE_D(21, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_22(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_22(o, s, _S1BOOSTPP_LIST_REVERSE_D(22, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_23(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_23(o, s, _S1BOOSTPP_LIST_REVERSE_D(23, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_24(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_24(o, s, _S1BOOSTPP_LIST_REVERSE_D(24, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_25(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_25(o, s, _S1BOOSTPP_LIST_REVERSE_D(25, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_26(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_26(o, s, _S1BOOSTPP_LIST_REVERSE_D(26, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_27(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_27(o, s, _S1BOOSTPP_LIST_REVERSE_D(27, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_28(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_28(o, s, _S1BOOSTPP_LIST_REVERSE_D(28, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_29(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_29(o, s, _S1BOOSTPP_LIST_REVERSE_D(29, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_30(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_30(o, s, _S1BOOSTPP_LIST_REVERSE_D(30, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_31(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_31(o, s, _S1BOOSTPP_LIST_REVERSE_D(31, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_32(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_32(o, s, _S1BOOSTPP_LIST_REVERSE_D(32, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_33(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_33(o, s, _S1BOOSTPP_LIST_REVERSE_D(33, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_34(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_34(o, s, _S1BOOSTPP_LIST_REVERSE_D(34, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_35(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_35(o, s, _S1BOOSTPP_LIST_REVERSE_D(35, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_36(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_36(o, s, _S1BOOSTPP_LIST_REVERSE_D(36, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_37(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_37(o, s, _S1BOOSTPP_LIST_REVERSE_D(37, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_38(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_38(o, s, _S1BOOSTPP_LIST_REVERSE_D(38, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_39(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_39(o, s, _S1BOOSTPP_LIST_REVERSE_D(39, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_40(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_40(o, s, _S1BOOSTPP_LIST_REVERSE_D(40, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_41(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_41(o, s, _S1BOOSTPP_LIST_REVERSE_D(41, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_42(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_42(o, s, _S1BOOSTPP_LIST_REVERSE_D(42, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_43(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_43(o, s, _S1BOOSTPP_LIST_REVERSE_D(43, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_44(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_44(o, s, _S1BOOSTPP_LIST_REVERSE_D(44, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_45(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_45(o, s, _S1BOOSTPP_LIST_REVERSE_D(45, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_46(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_46(o, s, _S1BOOSTPP_LIST_REVERSE_D(46, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_47(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_47(o, s, _S1BOOSTPP_LIST_REVERSE_D(47, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_48(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_48(o, s, _S1BOOSTPP_LIST_REVERSE_D(48, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_49(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_49(o, s, _S1BOOSTPP_LIST_REVERSE_D(49, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_50(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_50(o, s, _S1BOOSTPP_LIST_REVERSE_D(50, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_51(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_51(o, s, _S1BOOSTPP_LIST_REVERSE_D(51, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_52(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_52(o, s, _S1BOOSTPP_LIST_REVERSE_D(52, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_53(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_53(o, s, _S1BOOSTPP_LIST_REVERSE_D(53, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_54(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_54(o, s, _S1BOOSTPP_LIST_REVERSE_D(54, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_55(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_55(o, s, _S1BOOSTPP_LIST_REVERSE_D(55, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_56(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_56(o, s, _S1BOOSTPP_LIST_REVERSE_D(56, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_57(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_57(o, s, _S1BOOSTPP_LIST_REVERSE_D(57, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_58(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_58(o, s, _S1BOOSTPP_LIST_REVERSE_D(58, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_59(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_59(o, s, _S1BOOSTPP_LIST_REVERSE_D(59, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_60(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_60(o, s, _S1BOOSTPP_LIST_REVERSE_D(60, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_61(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_61(o, s, _S1BOOSTPP_LIST_REVERSE_D(61, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_62(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_62(o, s, _S1BOOSTPP_LIST_REVERSE_D(62, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_63(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_63(o, s, _S1BOOSTPP_LIST_REVERSE_D(63, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_64(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_64(o, s, _S1BOOSTPP_LIST_REVERSE_D(64, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_65(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_65(o, s, _S1BOOSTPP_LIST_REVERSE_D(65, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_66(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_66(o, s, _S1BOOSTPP_LIST_REVERSE_D(66, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_67(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_67(o, s, _S1BOOSTPP_LIST_REVERSE_D(67, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_68(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_68(o, s, _S1BOOSTPP_LIST_REVERSE_D(68, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_69(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_69(o, s, _S1BOOSTPP_LIST_REVERSE_D(69, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_70(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_70(o, s, _S1BOOSTPP_LIST_REVERSE_D(70, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_71(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_71(o, s, _S1BOOSTPP_LIST_REVERSE_D(71, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_72(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_72(o, s, _S1BOOSTPP_LIST_REVERSE_D(72, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_73(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_73(o, s, _S1BOOSTPP_LIST_REVERSE_D(73, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_74(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_74(o, s, _S1BOOSTPP_LIST_REVERSE_D(74, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_75(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_75(o, s, _S1BOOSTPP_LIST_REVERSE_D(75, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_76(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_76(o, s, _S1BOOSTPP_LIST_REVERSE_D(76, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_77(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_77(o, s, _S1BOOSTPP_LIST_REVERSE_D(77, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_78(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_78(o, s, _S1BOOSTPP_LIST_REVERSE_D(78, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_79(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_79(o, s, _S1BOOSTPP_LIST_REVERSE_D(79, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_80(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_80(o, s, _S1BOOSTPP_LIST_REVERSE_D(80, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_81(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_81(o, s, _S1BOOSTPP_LIST_REVERSE_D(81, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_82(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_82(o, s, _S1BOOSTPP_LIST_REVERSE_D(82, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_83(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_83(o, s, _S1BOOSTPP_LIST_REVERSE_D(83, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_84(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_84(o, s, _S1BOOSTPP_LIST_REVERSE_D(84, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_85(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_85(o, s, _S1BOOSTPP_LIST_REVERSE_D(85, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_86(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_86(o, s, _S1BOOSTPP_LIST_REVERSE_D(86, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_87(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_87(o, s, _S1BOOSTPP_LIST_REVERSE_D(87, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_88(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_88(o, s, _S1BOOSTPP_LIST_REVERSE_D(88, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_89(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_89(o, s, _S1BOOSTPP_LIST_REVERSE_D(89, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_90(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_90(o, s, _S1BOOSTPP_LIST_REVERSE_D(90, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_91(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_91(o, s, _S1BOOSTPP_LIST_REVERSE_D(91, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_92(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_92(o, s, _S1BOOSTPP_LIST_REVERSE_D(92, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_93(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_93(o, s, _S1BOOSTPP_LIST_REVERSE_D(93, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_94(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_94(o, s, _S1BOOSTPP_LIST_REVERSE_D(94, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_95(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_95(o, s, _S1BOOSTPP_LIST_REVERSE_D(95, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_96(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_96(o, s, _S1BOOSTPP_LIST_REVERSE_D(96, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_97(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_97(o, s, _S1BOOSTPP_LIST_REVERSE_D(97, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_98(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_98(o, s, _S1BOOSTPP_LIST_REVERSE_D(98, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_99(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_99(o, s, _S1BOOSTPP_LIST_REVERSE_D(99, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_100(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_100(o, s, _S1BOOSTPP_LIST_REVERSE_D(100, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_101(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_101(o, s, _S1BOOSTPP_LIST_REVERSE_D(101, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_102(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_102(o, s, _S1BOOSTPP_LIST_REVERSE_D(102, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_103(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_103(o, s, _S1BOOSTPP_LIST_REVERSE_D(103, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_104(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_104(o, s, _S1BOOSTPP_LIST_REVERSE_D(104, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_105(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_105(o, s, _S1BOOSTPP_LIST_REVERSE_D(105, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_106(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_106(o, s, _S1BOOSTPP_LIST_REVERSE_D(106, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_107(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_107(o, s, _S1BOOSTPP_LIST_REVERSE_D(107, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_108(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_108(o, s, _S1BOOSTPP_LIST_REVERSE_D(108, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_109(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_109(o, s, _S1BOOSTPP_LIST_REVERSE_D(109, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_110(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_110(o, s, _S1BOOSTPP_LIST_REVERSE_D(110, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_111(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_111(o, s, _S1BOOSTPP_LIST_REVERSE_D(111, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_112(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_112(o, s, _S1BOOSTPP_LIST_REVERSE_D(112, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_113(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_113(o, s, _S1BOOSTPP_LIST_REVERSE_D(113, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_114(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_114(o, s, _S1BOOSTPP_LIST_REVERSE_D(114, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_115(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_115(o, s, _S1BOOSTPP_LIST_REVERSE_D(115, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_116(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_116(o, s, _S1BOOSTPP_LIST_REVERSE_D(116, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_117(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_117(o, s, _S1BOOSTPP_LIST_REVERSE_D(117, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_118(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_118(o, s, _S1BOOSTPP_LIST_REVERSE_D(118, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_119(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_119(o, s, _S1BOOSTPP_LIST_REVERSE_D(119, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_120(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_120(o, s, _S1BOOSTPP_LIST_REVERSE_D(120, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_121(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_121(o, s, _S1BOOSTPP_LIST_REVERSE_D(121, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_122(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_122(o, s, _S1BOOSTPP_LIST_REVERSE_D(122, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_123(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_123(o, s, _S1BOOSTPP_LIST_REVERSE_D(123, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_124(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_124(o, s, _S1BOOSTPP_LIST_REVERSE_D(124, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_125(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_125(o, s, _S1BOOSTPP_LIST_REVERSE_D(125, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_126(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_126(o, s, _S1BOOSTPP_LIST_REVERSE_D(126, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_127(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_127(o, s, _S1BOOSTPP_LIST_REVERSE_D(127, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_128(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_128(o, s, _S1BOOSTPP_LIST_REVERSE_D(128, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_129(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_129(o, s, _S1BOOSTPP_LIST_REVERSE_D(129, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_130(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_130(o, s, _S1BOOSTPP_LIST_REVERSE_D(130, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_131(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_131(o, s, _S1BOOSTPP_LIST_REVERSE_D(131, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_132(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_132(o, s, _S1BOOSTPP_LIST_REVERSE_D(132, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_133(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_133(o, s, _S1BOOSTPP_LIST_REVERSE_D(133, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_134(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_134(o, s, _S1BOOSTPP_LIST_REVERSE_D(134, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_135(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_135(o, s, _S1BOOSTPP_LIST_REVERSE_D(135, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_136(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_136(o, s, _S1BOOSTPP_LIST_REVERSE_D(136, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_137(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_137(o, s, _S1BOOSTPP_LIST_REVERSE_D(137, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_138(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_138(o, s, _S1BOOSTPP_LIST_REVERSE_D(138, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_139(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_139(o, s, _S1BOOSTPP_LIST_REVERSE_D(139, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_140(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_140(o, s, _S1BOOSTPP_LIST_REVERSE_D(140, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_141(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_141(o, s, _S1BOOSTPP_LIST_REVERSE_D(141, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_142(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_142(o, s, _S1BOOSTPP_LIST_REVERSE_D(142, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_143(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_143(o, s, _S1BOOSTPP_LIST_REVERSE_D(143, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_144(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_144(o, s, _S1BOOSTPP_LIST_REVERSE_D(144, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_145(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_145(o, s, _S1BOOSTPP_LIST_REVERSE_D(145, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_146(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_146(o, s, _S1BOOSTPP_LIST_REVERSE_D(146, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_147(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_147(o, s, _S1BOOSTPP_LIST_REVERSE_D(147, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_148(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_148(o, s, _S1BOOSTPP_LIST_REVERSE_D(148, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_149(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_149(o, s, _S1BOOSTPP_LIST_REVERSE_D(149, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_150(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_150(o, s, _S1BOOSTPP_LIST_REVERSE_D(150, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_151(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_151(o, s, _S1BOOSTPP_LIST_REVERSE_D(151, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_152(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_152(o, s, _S1BOOSTPP_LIST_REVERSE_D(152, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_153(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_153(o, s, _S1BOOSTPP_LIST_REVERSE_D(153, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_154(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_154(o, s, _S1BOOSTPP_LIST_REVERSE_D(154, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_155(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_155(o, s, _S1BOOSTPP_LIST_REVERSE_D(155, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_156(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_156(o, s, _S1BOOSTPP_LIST_REVERSE_D(156, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_157(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_157(o, s, _S1BOOSTPP_LIST_REVERSE_D(157, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_158(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_158(o, s, _S1BOOSTPP_LIST_REVERSE_D(158, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_159(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_159(o, s, _S1BOOSTPP_LIST_REVERSE_D(159, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_160(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_160(o, s, _S1BOOSTPP_LIST_REVERSE_D(160, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_161(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_161(o, s, _S1BOOSTPP_LIST_REVERSE_D(161, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_162(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_162(o, s, _S1BOOSTPP_LIST_REVERSE_D(162, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_163(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_163(o, s, _S1BOOSTPP_LIST_REVERSE_D(163, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_164(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_164(o, s, _S1BOOSTPP_LIST_REVERSE_D(164, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_165(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_165(o, s, _S1BOOSTPP_LIST_REVERSE_D(165, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_166(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_166(o, s, _S1BOOSTPP_LIST_REVERSE_D(166, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_167(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_167(o, s, _S1BOOSTPP_LIST_REVERSE_D(167, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_168(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_168(o, s, _S1BOOSTPP_LIST_REVERSE_D(168, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_169(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_169(o, s, _S1BOOSTPP_LIST_REVERSE_D(169, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_170(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_170(o, s, _S1BOOSTPP_LIST_REVERSE_D(170, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_171(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_171(o, s, _S1BOOSTPP_LIST_REVERSE_D(171, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_172(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_172(o, s, _S1BOOSTPP_LIST_REVERSE_D(172, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_173(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_173(o, s, _S1BOOSTPP_LIST_REVERSE_D(173, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_174(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_174(o, s, _S1BOOSTPP_LIST_REVERSE_D(174, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_175(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_175(o, s, _S1BOOSTPP_LIST_REVERSE_D(175, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_176(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_176(o, s, _S1BOOSTPP_LIST_REVERSE_D(176, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_177(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_177(o, s, _S1BOOSTPP_LIST_REVERSE_D(177, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_178(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_178(o, s, _S1BOOSTPP_LIST_REVERSE_D(178, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_179(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_179(o, s, _S1BOOSTPP_LIST_REVERSE_D(179, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_180(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_180(o, s, _S1BOOSTPP_LIST_REVERSE_D(180, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_181(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_181(o, s, _S1BOOSTPP_LIST_REVERSE_D(181, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_182(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_182(o, s, _S1BOOSTPP_LIST_REVERSE_D(182, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_183(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_183(o, s, _S1BOOSTPP_LIST_REVERSE_D(183, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_184(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_184(o, s, _S1BOOSTPP_LIST_REVERSE_D(184, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_185(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_185(o, s, _S1BOOSTPP_LIST_REVERSE_D(185, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_186(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_186(o, s, _S1BOOSTPP_LIST_REVERSE_D(186, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_187(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_187(o, s, _S1BOOSTPP_LIST_REVERSE_D(187, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_188(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_188(o, s, _S1BOOSTPP_LIST_REVERSE_D(188, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_189(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_189(o, s, _S1BOOSTPP_LIST_REVERSE_D(189, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_190(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_190(o, s, _S1BOOSTPP_LIST_REVERSE_D(190, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_191(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_191(o, s, _S1BOOSTPP_LIST_REVERSE_D(191, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_192(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_192(o, s, _S1BOOSTPP_LIST_REVERSE_D(192, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_193(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_193(o, s, _S1BOOSTPP_LIST_REVERSE_D(193, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_194(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_194(o, s, _S1BOOSTPP_LIST_REVERSE_D(194, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_195(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_195(o, s, _S1BOOSTPP_LIST_REVERSE_D(195, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_196(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_196(o, s, _S1BOOSTPP_LIST_REVERSE_D(196, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_197(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_197(o, s, _S1BOOSTPP_LIST_REVERSE_D(197, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_198(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_198(o, s, _S1BOOSTPP_LIST_REVERSE_D(198, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_199(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_199(o, s, _S1BOOSTPP_LIST_REVERSE_D(199, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_200(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_200(o, s, _S1BOOSTPP_LIST_REVERSE_D(200, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_201(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_201(o, s, _S1BOOSTPP_LIST_REVERSE_D(201, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_202(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_202(o, s, _S1BOOSTPP_LIST_REVERSE_D(202, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_203(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_203(o, s, _S1BOOSTPP_LIST_REVERSE_D(203, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_204(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_204(o, s, _S1BOOSTPP_LIST_REVERSE_D(204, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_205(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_205(o, s, _S1BOOSTPP_LIST_REVERSE_D(205, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_206(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_206(o, s, _S1BOOSTPP_LIST_REVERSE_D(206, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_207(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_207(o, s, _S1BOOSTPP_LIST_REVERSE_D(207, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_208(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_208(o, s, _S1BOOSTPP_LIST_REVERSE_D(208, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_209(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_209(o, s, _S1BOOSTPP_LIST_REVERSE_D(209, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_210(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_210(o, s, _S1BOOSTPP_LIST_REVERSE_D(210, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_211(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_211(o, s, _S1BOOSTPP_LIST_REVERSE_D(211, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_212(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_212(o, s, _S1BOOSTPP_LIST_REVERSE_D(212, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_213(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_213(o, s, _S1BOOSTPP_LIST_REVERSE_D(213, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_214(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_214(o, s, _S1BOOSTPP_LIST_REVERSE_D(214, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_215(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_215(o, s, _S1BOOSTPP_LIST_REVERSE_D(215, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_216(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_216(o, s, _S1BOOSTPP_LIST_REVERSE_D(216, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_217(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_217(o, s, _S1BOOSTPP_LIST_REVERSE_D(217, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_218(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_218(o, s, _S1BOOSTPP_LIST_REVERSE_D(218, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_219(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_219(o, s, _S1BOOSTPP_LIST_REVERSE_D(219, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_220(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_220(o, s, _S1BOOSTPP_LIST_REVERSE_D(220, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_221(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_221(o, s, _S1BOOSTPP_LIST_REVERSE_D(221, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_222(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_222(o, s, _S1BOOSTPP_LIST_REVERSE_D(222, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_223(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_223(o, s, _S1BOOSTPP_LIST_REVERSE_D(223, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_224(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_224(o, s, _S1BOOSTPP_LIST_REVERSE_D(224, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_225(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_225(o, s, _S1BOOSTPP_LIST_REVERSE_D(225, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_226(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_226(o, s, _S1BOOSTPP_LIST_REVERSE_D(226, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_227(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_227(o, s, _S1BOOSTPP_LIST_REVERSE_D(227, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_228(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_228(o, s, _S1BOOSTPP_LIST_REVERSE_D(228, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_229(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_229(o, s, _S1BOOSTPP_LIST_REVERSE_D(229, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_230(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_230(o, s, _S1BOOSTPP_LIST_REVERSE_D(230, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_231(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_231(o, s, _S1BOOSTPP_LIST_REVERSE_D(231, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_232(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_232(o, s, _S1BOOSTPP_LIST_REVERSE_D(232, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_233(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_233(o, s, _S1BOOSTPP_LIST_REVERSE_D(233, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_234(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_234(o, s, _S1BOOSTPP_LIST_REVERSE_D(234, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_235(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_235(o, s, _S1BOOSTPP_LIST_REVERSE_D(235, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_236(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_236(o, s, _S1BOOSTPP_LIST_REVERSE_D(236, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_237(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_237(o, s, _S1BOOSTPP_LIST_REVERSE_D(237, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_238(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_238(o, s, _S1BOOSTPP_LIST_REVERSE_D(238, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_239(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_239(o, s, _S1BOOSTPP_LIST_REVERSE_D(239, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_240(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_240(o, s, _S1BOOSTPP_LIST_REVERSE_D(240, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_241(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_241(o, s, _S1BOOSTPP_LIST_REVERSE_D(241, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_242(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_242(o, s, _S1BOOSTPP_LIST_REVERSE_D(242, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_243(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_243(o, s, _S1BOOSTPP_LIST_REVERSE_D(243, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_244(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_244(o, s, _S1BOOSTPP_LIST_REVERSE_D(244, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_245(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_245(o, s, _S1BOOSTPP_LIST_REVERSE_D(245, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_246(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_246(o, s, _S1BOOSTPP_LIST_REVERSE_D(246, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_247(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_247(o, s, _S1BOOSTPP_LIST_REVERSE_D(247, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_248(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_248(o, s, _S1BOOSTPP_LIST_REVERSE_D(248, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_249(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_249(o, s, _S1BOOSTPP_LIST_REVERSE_D(249, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_250(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_250(o, s, _S1BOOSTPP_LIST_REVERSE_D(250, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_251(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_251(o, s, _S1BOOSTPP_LIST_REVERSE_D(251, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_252(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_252(o, s, _S1BOOSTPP_LIST_REVERSE_D(252, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_253(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_253(o, s, _S1BOOSTPP_LIST_REVERSE_D(253, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_254(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_254(o, s, _S1BOOSTPP_LIST_REVERSE_D(254, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_255(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_255(o, s, _S1BOOSTPP_LIST_REVERSE_D(255, l))
# define _S1BOOSTPP_LIST_FOLD_RIGHT_256(o, s, l) _S1BOOSTPP_LIST_FOLD_LEFT_256(o, s, _S1BOOSTPP_LIST_REVERSE_D(256, l))
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LIST_FOLD_RIGHT_HPP
# define _S1BOOSTPP_LIST_FOLD_RIGHT_HPP
#
#
# if 0
#    define _S1BOOSTPP_LIST_FOLD_RIGHT(op, state, list)
# endif
#
# define _S1BOOSTPP_LIST_FOLD_RIGHT _S1BOOSTPP_CAT(_S1BOOSTPP_LIST_FOLD_RIGHT_, _S1BOOSTPP_AUTO_REC(_S1BOOSTPP_WHILE_P, 256))
#
# define _S1BOOSTPP_LIST_FOLD_RIGHT_257(o, s, l) _S1BOOSTPP_ERROR(0x0004)
#
# define _S1BOOSTPP_LIST_FOLD_RIGHT_D(d, o, s, l) _S1BOOSTPP_LIST_FOLD_RIGHT_ ## d(o, s, l)
# define _S1BOOSTPP_LIST_FOLD_RIGHT_2ND _S1BOOSTPP_LIST_FOLD_RIGHT
# define _S1BOOSTPP_LIST_FOLD_RIGHT_2ND_D _S1BOOSTPP_LIST_FOLD_RIGHT_D
#
# if _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
# else
# endif
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_ARITHMETIC_INC_HPP
# define _S1BOOSTPP_ARITHMETIC_INC_HPP
#
#
# /* _S1BOOSTPP_INC */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_INC(x) _S1BOOSTPP_INC_I(x)
# else
#    define _S1BOOSTPP_INC(x) _S1BOOSTPP_INC_OO((x))
#    define _S1BOOSTPP_INC_OO(par) _S1BOOSTPP_INC_I ## par
# endif
#
# define _S1BOOSTPP_INC_I(x) _S1BOOSTPP_INC_ ## x
#
# define _S1BOOSTPP_INC_0 1
# define _S1BOOSTPP_INC_1 2
# define _S1BOOSTPP_INC_2 3
# define _S1BOOSTPP_INC_3 4
# define _S1BOOSTPP_INC_4 5
# define _S1BOOSTPP_INC_5 6
# define _S1BOOSTPP_INC_6 7
# define _S1BOOSTPP_INC_7 8
# define _S1BOOSTPP_INC_8 9
# define _S1BOOSTPP_INC_9 10
# define _S1BOOSTPP_INC_10 11
# define _S1BOOSTPP_INC_11 12
# define _S1BOOSTPP_INC_12 13
# define _S1BOOSTPP_INC_13 14
# define _S1BOOSTPP_INC_14 15
# define _S1BOOSTPP_INC_15 16
# define _S1BOOSTPP_INC_16 17
# define _S1BOOSTPP_INC_17 18
# define _S1BOOSTPP_INC_18 19
# define _S1BOOSTPP_INC_19 20
# define _S1BOOSTPP_INC_20 21
# define _S1BOOSTPP_INC_21 22
# define _S1BOOSTPP_INC_22 23
# define _S1BOOSTPP_INC_23 24
# define _S1BOOSTPP_INC_24 25
# define _S1BOOSTPP_INC_25 26
# define _S1BOOSTPP_INC_26 27
# define _S1BOOSTPP_INC_27 28
# define _S1BOOSTPP_INC_28 29
# define _S1BOOSTPP_INC_29 30
# define _S1BOOSTPP_INC_30 31
# define _S1BOOSTPP_INC_31 32
# define _S1BOOSTPP_INC_32 33
# define _S1BOOSTPP_INC_33 34
# define _S1BOOSTPP_INC_34 35
# define _S1BOOSTPP_INC_35 36
# define _S1BOOSTPP_INC_36 37
# define _S1BOOSTPP_INC_37 38
# define _S1BOOSTPP_INC_38 39
# define _S1BOOSTPP_INC_39 40
# define _S1BOOSTPP_INC_40 41
# define _S1BOOSTPP_INC_41 42
# define _S1BOOSTPP_INC_42 43
# define _S1BOOSTPP_INC_43 44
# define _S1BOOSTPP_INC_44 45
# define _S1BOOSTPP_INC_45 46
# define _S1BOOSTPP_INC_46 47
# define _S1BOOSTPP_INC_47 48
# define _S1BOOSTPP_INC_48 49
# define _S1BOOSTPP_INC_49 50
# define _S1BOOSTPP_INC_50 51
# define _S1BOOSTPP_INC_51 52
# define _S1BOOSTPP_INC_52 53
# define _S1BOOSTPP_INC_53 54
# define _S1BOOSTPP_INC_54 55
# define _S1BOOSTPP_INC_55 56
# define _S1BOOSTPP_INC_56 57
# define _S1BOOSTPP_INC_57 58
# define _S1BOOSTPP_INC_58 59
# define _S1BOOSTPP_INC_59 60
# define _S1BOOSTPP_INC_60 61
# define _S1BOOSTPP_INC_61 62
# define _S1BOOSTPP_INC_62 63
# define _S1BOOSTPP_INC_63 64
# define _S1BOOSTPP_INC_64 65
# define _S1BOOSTPP_INC_65 66
# define _S1BOOSTPP_INC_66 67
# define _S1BOOSTPP_INC_67 68
# define _S1BOOSTPP_INC_68 69
# define _S1BOOSTPP_INC_69 70
# define _S1BOOSTPP_INC_70 71
# define _S1BOOSTPP_INC_71 72
# define _S1BOOSTPP_INC_72 73
# define _S1BOOSTPP_INC_73 74
# define _S1BOOSTPP_INC_74 75
# define _S1BOOSTPP_INC_75 76
# define _S1BOOSTPP_INC_76 77
# define _S1BOOSTPP_INC_77 78
# define _S1BOOSTPP_INC_78 79
# define _S1BOOSTPP_INC_79 80
# define _S1BOOSTPP_INC_80 81
# define _S1BOOSTPP_INC_81 82
# define _S1BOOSTPP_INC_82 83
# define _S1BOOSTPP_INC_83 84
# define _S1BOOSTPP_INC_84 85
# define _S1BOOSTPP_INC_85 86
# define _S1BOOSTPP_INC_86 87
# define _S1BOOSTPP_INC_87 88
# define _S1BOOSTPP_INC_88 89
# define _S1BOOSTPP_INC_89 90
# define _S1BOOSTPP_INC_90 91
# define _S1BOOSTPP_INC_91 92
# define _S1BOOSTPP_INC_92 93
# define _S1BOOSTPP_INC_93 94
# define _S1BOOSTPP_INC_94 95
# define _S1BOOSTPP_INC_95 96
# define _S1BOOSTPP_INC_96 97
# define _S1BOOSTPP_INC_97 98
# define _S1BOOSTPP_INC_98 99
# define _S1BOOSTPP_INC_99 100
# define _S1BOOSTPP_INC_100 101
# define _S1BOOSTPP_INC_101 102
# define _S1BOOSTPP_INC_102 103
# define _S1BOOSTPP_INC_103 104
# define _S1BOOSTPP_INC_104 105
# define _S1BOOSTPP_INC_105 106
# define _S1BOOSTPP_INC_106 107
# define _S1BOOSTPP_INC_107 108
# define _S1BOOSTPP_INC_108 109
# define _S1BOOSTPP_INC_109 110
# define _S1BOOSTPP_INC_110 111
# define _S1BOOSTPP_INC_111 112
# define _S1BOOSTPP_INC_112 113
# define _S1BOOSTPP_INC_113 114
# define _S1BOOSTPP_INC_114 115
# define _S1BOOSTPP_INC_115 116
# define _S1BOOSTPP_INC_116 117
# define _S1BOOSTPP_INC_117 118
# define _S1BOOSTPP_INC_118 119
# define _S1BOOSTPP_INC_119 120
# define _S1BOOSTPP_INC_120 121
# define _S1BOOSTPP_INC_121 122
# define _S1BOOSTPP_INC_122 123
# define _S1BOOSTPP_INC_123 124
# define _S1BOOSTPP_INC_124 125
# define _S1BOOSTPP_INC_125 126
# define _S1BOOSTPP_INC_126 127
# define _S1BOOSTPP_INC_127 128
# define _S1BOOSTPP_INC_128 129
# define _S1BOOSTPP_INC_129 130
# define _S1BOOSTPP_INC_130 131
# define _S1BOOSTPP_INC_131 132
# define _S1BOOSTPP_INC_132 133
# define _S1BOOSTPP_INC_133 134
# define _S1BOOSTPP_INC_134 135
# define _S1BOOSTPP_INC_135 136
# define _S1BOOSTPP_INC_136 137
# define _S1BOOSTPP_INC_137 138
# define _S1BOOSTPP_INC_138 139
# define _S1BOOSTPP_INC_139 140
# define _S1BOOSTPP_INC_140 141
# define _S1BOOSTPP_INC_141 142
# define _S1BOOSTPP_INC_142 143
# define _S1BOOSTPP_INC_143 144
# define _S1BOOSTPP_INC_144 145
# define _S1BOOSTPP_INC_145 146
# define _S1BOOSTPP_INC_146 147
# define _S1BOOSTPP_INC_147 148
# define _S1BOOSTPP_INC_148 149
# define _S1BOOSTPP_INC_149 150
# define _S1BOOSTPP_INC_150 151
# define _S1BOOSTPP_INC_151 152
# define _S1BOOSTPP_INC_152 153
# define _S1BOOSTPP_INC_153 154
# define _S1BOOSTPP_INC_154 155
# define _S1BOOSTPP_INC_155 156
# define _S1BOOSTPP_INC_156 157
# define _S1BOOSTPP_INC_157 158
# define _S1BOOSTPP_INC_158 159
# define _S1BOOSTPP_INC_159 160
# define _S1BOOSTPP_INC_160 161
# define _S1BOOSTPP_INC_161 162
# define _S1BOOSTPP_INC_162 163
# define _S1BOOSTPP_INC_163 164
# define _S1BOOSTPP_INC_164 165
# define _S1BOOSTPP_INC_165 166
# define _S1BOOSTPP_INC_166 167
# define _S1BOOSTPP_INC_167 168
# define _S1BOOSTPP_INC_168 169
# define _S1BOOSTPP_INC_169 170
# define _S1BOOSTPP_INC_170 171
# define _S1BOOSTPP_INC_171 172
# define _S1BOOSTPP_INC_172 173
# define _S1BOOSTPP_INC_173 174
# define _S1BOOSTPP_INC_174 175
# define _S1BOOSTPP_INC_175 176
# define _S1BOOSTPP_INC_176 177
# define _S1BOOSTPP_INC_177 178
# define _S1BOOSTPP_INC_178 179
# define _S1BOOSTPP_INC_179 180
# define _S1BOOSTPP_INC_180 181
# define _S1BOOSTPP_INC_181 182
# define _S1BOOSTPP_INC_182 183
# define _S1BOOSTPP_INC_183 184
# define _S1BOOSTPP_INC_184 185
# define _S1BOOSTPP_INC_185 186
# define _S1BOOSTPP_INC_186 187
# define _S1BOOSTPP_INC_187 188
# define _S1BOOSTPP_INC_188 189
# define _S1BOOSTPP_INC_189 190
# define _S1BOOSTPP_INC_190 191
# define _S1BOOSTPP_INC_191 192
# define _S1BOOSTPP_INC_192 193
# define _S1BOOSTPP_INC_193 194
# define _S1BOOSTPP_INC_194 195
# define _S1BOOSTPP_INC_195 196
# define _S1BOOSTPP_INC_196 197
# define _S1BOOSTPP_INC_197 198
# define _S1BOOSTPP_INC_198 199
# define _S1BOOSTPP_INC_199 200
# define _S1BOOSTPP_INC_200 201
# define _S1BOOSTPP_INC_201 202
# define _S1BOOSTPP_INC_202 203
# define _S1BOOSTPP_INC_203 204
# define _S1BOOSTPP_INC_204 205
# define _S1BOOSTPP_INC_205 206
# define _S1BOOSTPP_INC_206 207
# define _S1BOOSTPP_INC_207 208
# define _S1BOOSTPP_INC_208 209
# define _S1BOOSTPP_INC_209 210
# define _S1BOOSTPP_INC_210 211
# define _S1BOOSTPP_INC_211 212
# define _S1BOOSTPP_INC_212 213
# define _S1BOOSTPP_INC_213 214
# define _S1BOOSTPP_INC_214 215
# define _S1BOOSTPP_INC_215 216
# define _S1BOOSTPP_INC_216 217
# define _S1BOOSTPP_INC_217 218
# define _S1BOOSTPP_INC_218 219
# define _S1BOOSTPP_INC_219 220
# define _S1BOOSTPP_INC_220 221
# define _S1BOOSTPP_INC_221 222
# define _S1BOOSTPP_INC_222 223
# define _S1BOOSTPP_INC_223 224
# define _S1BOOSTPP_INC_224 225
# define _S1BOOSTPP_INC_225 226
# define _S1BOOSTPP_INC_226 227
# define _S1BOOSTPP_INC_227 228
# define _S1BOOSTPP_INC_228 229
# define _S1BOOSTPP_INC_229 230
# define _S1BOOSTPP_INC_230 231
# define _S1BOOSTPP_INC_231 232
# define _S1BOOSTPP_INC_232 233
# define _S1BOOSTPP_INC_233 234
# define _S1BOOSTPP_INC_234 235
# define _S1BOOSTPP_INC_235 236
# define _S1BOOSTPP_INC_236 237
# define _S1BOOSTPP_INC_237 238
# define _S1BOOSTPP_INC_238 239
# define _S1BOOSTPP_INC_239 240
# define _S1BOOSTPP_INC_240 241
# define _S1BOOSTPP_INC_241 242
# define _S1BOOSTPP_INC_242 243
# define _S1BOOSTPP_INC_243 244
# define _S1BOOSTPP_INC_244 245
# define _S1BOOSTPP_INC_245 246
# define _S1BOOSTPP_INC_246 247
# define _S1BOOSTPP_INC_247 248
# define _S1BOOSTPP_INC_248 249
# define _S1BOOSTPP_INC_249 250
# define _S1BOOSTPP_INC_250 251
# define _S1BOOSTPP_INC_251 252
# define _S1BOOSTPP_INC_252 253
# define _S1BOOSTPP_INC_253 254
# define _S1BOOSTPP_INC_254 255
# define _S1BOOSTPP_INC_255 256
# define _S1BOOSTPP_INC_256 256
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
#
# if _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_DMC()
# else
#
# ifndef _S1BOOSTPP_DETAIL_AUTO_REC_HPP
# define _S1BOOSTPP_DETAIL_AUTO_REC_HPP
#
#
# /* _S1BOOSTPP_AUTO_REC */
#
# define _S1BOOSTPP_AUTO_REC(pred, n) _S1BOOSTPP_NODE_ENTRY_ ## n(pred)
#
# define _S1BOOSTPP_NODE_ENTRY_256(p) _S1BOOSTPP_NODE_128(p)(p)(p)(p)(p)(p)(p)(p)
# define _S1BOOSTPP_NODE_ENTRY_128(p) _S1BOOSTPP_NODE_64(p)(p)(p)(p)(p)(p)(p)
# define _S1BOOSTPP_NODE_ENTRY_64(p) _S1BOOSTPP_NODE_32(p)(p)(p)(p)(p)(p)
# define _S1BOOSTPP_NODE_ENTRY_32(p) _S1BOOSTPP_NODE_16(p)(p)(p)(p)(p)
# define _S1BOOSTPP_NODE_ENTRY_16(p) _S1BOOSTPP_NODE_8(p)(p)(p)(p)
# define _S1BOOSTPP_NODE_ENTRY_8(p) _S1BOOSTPP_NODE_4(p)(p)(p)
# define _S1BOOSTPP_NODE_ENTRY_4(p) _S1BOOSTPP_NODE_2(p)(p)
# define _S1BOOSTPP_NODE_ENTRY_2(p) _S1BOOSTPP_NODE_1(p)
#
# define _S1BOOSTPP_NODE_128(p) _S1BOOSTPP_IIF(p(128), _S1BOOSTPP_NODE_64, _S1BOOSTPP_NODE_192)
#    define _S1BOOSTPP_NODE_64(p) _S1BOOSTPP_IIF(p(64), _S1BOOSTPP_NODE_32, _S1BOOSTPP_NODE_96)
#        define _S1BOOSTPP_NODE_32(p) _S1BOOSTPP_IIF(p(32), _S1BOOSTPP_NODE_16, _S1BOOSTPP_NODE_48)
#            define _S1BOOSTPP_NODE_16(p) _S1BOOSTPP_IIF(p(16), _S1BOOSTPP_NODE_8, _S1BOOSTPP_NODE_24)
#                define _S1BOOSTPP_NODE_8(p) _S1BOOSTPP_IIF(p(8), _S1BOOSTPP_NODE_4, _S1BOOSTPP_NODE_12)
#                    define _S1BOOSTPP_NODE_4(p) _S1BOOSTPP_IIF(p(4), _S1BOOSTPP_NODE_2, _S1BOOSTPP_NODE_6)
#                        define _S1BOOSTPP_NODE_2(p) _S1BOOSTPP_IIF(p(2), _S1BOOSTPP_NODE_1, _S1BOOSTPP_NODE_3)
#                            define _S1BOOSTPP_NODE_1(p) _S1BOOSTPP_IIF(p(1), 1, 2)
#                            define _S1BOOSTPP_NODE_3(p) _S1BOOSTPP_IIF(p(3), 3, 4)
#                        define _S1BOOSTPP_NODE_6(p) _S1BOOSTPP_IIF(p(6), _S1BOOSTPP_NODE_5, _S1BOOSTPP_NODE_7)
#                            define _S1BOOSTPP_NODE_5(p) _S1BOOSTPP_IIF(p(5), 5, 6)
#                            define _S1BOOSTPP_NODE_7(p) _S1BOOSTPP_IIF(p(7), 7, 8)
#                    define _S1BOOSTPP_NODE_12(p) _S1BOOSTPP_IIF(p(12), _S1BOOSTPP_NODE_10, _S1BOOSTPP_NODE_14)
#                        define _S1BOOSTPP_NODE_10(p) _S1BOOSTPP_IIF(p(10), _S1BOOSTPP_NODE_9, _S1BOOSTPP_NODE_11)
#                            define _S1BOOSTPP_NODE_9(p) _S1BOOSTPP_IIF(p(9), 9, 10)
#                            define _S1BOOSTPP_NODE_11(p) _S1BOOSTPP_IIF(p(11), 11, 12)
#                        define _S1BOOSTPP_NODE_14(p) _S1BOOSTPP_IIF(p(14), _S1BOOSTPP_NODE_13, _S1BOOSTPP_NODE_15)
#                            define _S1BOOSTPP_NODE_13(p) _S1BOOSTPP_IIF(p(13), 13, 14)
#                            define _S1BOOSTPP_NODE_15(p) _S1BOOSTPP_IIF(p(15), 15, 16)
#                define _S1BOOSTPP_NODE_24(p) _S1BOOSTPP_IIF(p(24), _S1BOOSTPP_NODE_20, _S1BOOSTPP_NODE_28)
#                    define _S1BOOSTPP_NODE_20(p) _S1BOOSTPP_IIF(p(20), _S1BOOSTPP_NODE_18, _S1BOOSTPP_NODE_22)
#                        define _S1BOOSTPP_NODE_18(p) _S1BOOSTPP_IIF(p(18), _S1BOOSTPP_NODE_17, _S1BOOSTPP_NODE_19)
#                            define _S1BOOSTPP_NODE_17(p) _S1BOOSTPP_IIF(p(17), 17, 18)
#                            define _S1BOOSTPP_NODE_19(p) _S1BOOSTPP_IIF(p(19), 19, 20)
#                        define _S1BOOSTPP_NODE_22(p) _S1BOOSTPP_IIF(p(22), _S1BOOSTPP_NODE_21, _S1BOOSTPP_NODE_23)
#                            define _S1BOOSTPP_NODE_21(p) _S1BOOSTPP_IIF(p(21), 21, 22)
#                            define _S1BOOSTPP_NODE_23(p) _S1BOOSTPP_IIF(p(23), 23, 24)
#                    define _S1BOOSTPP_NODE_28(p) _S1BOOSTPP_IIF(p(28), _S1BOOSTPP_NODE_26, _S1BOOSTPP_NODE_30)
#                        define _S1BOOSTPP_NODE_26(p) _S1BOOSTPP_IIF(p(26), _S1BOOSTPP_NODE_25, _S1BOOSTPP_NODE_27)
#                            define _S1BOOSTPP_NODE_25(p) _S1BOOSTPP_IIF(p(25), 25, 26)
#                            define _S1BOOSTPP_NODE_27(p) _S1BOOSTPP_IIF(p(27), 27, 28)
#                        define _S1BOOSTPP_NODE_30(p) _S1BOOSTPP_IIF(p(30), _S1BOOSTPP_NODE_29, _S1BOOSTPP_NODE_31)
#                            define _S1BOOSTPP_NODE_29(p) _S1BOOSTPP_IIF(p(29), 29, 30)
#                            define _S1BOOSTPP_NODE_31(p) _S1BOOSTPP_IIF(p(31), 31, 32)
#            define _S1BOOSTPP_NODE_48(p) _S1BOOSTPP_IIF(p(48), _S1BOOSTPP_NODE_40, _S1BOOSTPP_NODE_56)
#                define _S1BOOSTPP_NODE_40(p) _S1BOOSTPP_IIF(p(40), _S1BOOSTPP_NODE_36, _S1BOOSTPP_NODE_44)
#                    define _S1BOOSTPP_NODE_36(p) _S1BOOSTPP_IIF(p(36), _S1BOOSTPP_NODE_34, _S1BOOSTPP_NODE_38)
#                        define _S1BOOSTPP_NODE_34(p) _S1BOOSTPP_IIF(p(34), _S1BOOSTPP_NODE_33, _S1BOOSTPP_NODE_35)
#                            define _S1BOOSTPP_NODE_33(p) _S1BOOSTPP_IIF(p(33), 33, 34)
#                            define _S1BOOSTPP_NODE_35(p) _S1BOOSTPP_IIF(p(35), 35, 36)
#                        define _S1BOOSTPP_NODE_38(p) _S1BOOSTPP_IIF(p(38), _S1BOOSTPP_NODE_37, _S1BOOSTPP_NODE_39)
#                            define _S1BOOSTPP_NODE_37(p) _S1BOOSTPP_IIF(p(37), 37, 38)
#                            define _S1BOOSTPP_NODE_39(p) _S1BOOSTPP_IIF(p(39), 39, 40)
#                    define _S1BOOSTPP_NODE_44(p) _S1BOOSTPP_IIF(p(44), _S1BOOSTPP_NODE_42, _S1BOOSTPP_NODE_46)
#                        define _S1BOOSTPP_NODE_42(p) _S1BOOSTPP_IIF(p(42), _S1BOOSTPP_NODE_41, _S1BOOSTPP_NODE_43)
#                            define _S1BOOSTPP_NODE_41(p) _S1BOOSTPP_IIF(p(41), 41, 42)
#                            define _S1BOOSTPP_NODE_43(p) _S1BOOSTPP_IIF(p(43), 43, 44)
#                        define _S1BOOSTPP_NODE_46(p) _S1BOOSTPP_IIF(p(46), _S1BOOSTPP_NODE_45, _S1BOOSTPP_NODE_47)
#                            define _S1BOOSTPP_NODE_45(p) _S1BOOSTPP_IIF(p(45), 45, 46)
#                            define _S1BOOSTPP_NODE_47(p) _S1BOOSTPP_IIF(p(47), 47, 48)
#                define _S1BOOSTPP_NODE_56(p) _S1BOOSTPP_IIF(p(56), _S1BOOSTPP_NODE_52, _S1BOOSTPP_NODE_60)
#                    define _S1BOOSTPP_NODE_52(p) _S1BOOSTPP_IIF(p(52), _S1BOOSTPP_NODE_50, _S1BOOSTPP_NODE_54)
#                        define _S1BOOSTPP_NODE_50(p) _S1BOOSTPP_IIF(p(50), _S1BOOSTPP_NODE_49, _S1BOOSTPP_NODE_51)
#                            define _S1BOOSTPP_NODE_49(p) _S1BOOSTPP_IIF(p(49), 49, 50)
#                            define _S1BOOSTPP_NODE_51(p) _S1BOOSTPP_IIF(p(51), 51, 52)
#                        define _S1BOOSTPP_NODE_54(p) _S1BOOSTPP_IIF(p(54), _S1BOOSTPP_NODE_53, _S1BOOSTPP_NODE_55)
#                            define _S1BOOSTPP_NODE_53(p) _S1BOOSTPP_IIF(p(53), 53, 54)
#                            define _S1BOOSTPP_NODE_55(p) _S1BOOSTPP_IIF(p(55), 55, 56)
#                    define _S1BOOSTPP_NODE_60(p) _S1BOOSTPP_IIF(p(60), _S1BOOSTPP_NODE_58, _S1BOOSTPP_NODE_62)
#                        define _S1BOOSTPP_NODE_58(p) _S1BOOSTPP_IIF(p(58), _S1BOOSTPP_NODE_57, _S1BOOSTPP_NODE_59)
#                            define _S1BOOSTPP_NODE_57(p) _S1BOOSTPP_IIF(p(57), 57, 58)
#                            define _S1BOOSTPP_NODE_59(p) _S1BOOSTPP_IIF(p(59), 59, 60)
#                        define _S1BOOSTPP_NODE_62(p) _S1BOOSTPP_IIF(p(62), _S1BOOSTPP_NODE_61, _S1BOOSTPP_NODE_63)
#                            define _S1BOOSTPP_NODE_61(p) _S1BOOSTPP_IIF(p(61), 61, 62)
#                            define _S1BOOSTPP_NODE_63(p) _S1BOOSTPP_IIF(p(63), 63, 64)
#        define _S1BOOSTPP_NODE_96(p) _S1BOOSTPP_IIF(p(96), _S1BOOSTPP_NODE_80, _S1BOOSTPP_NODE_112)
#            define _S1BOOSTPP_NODE_80(p) _S1BOOSTPP_IIF(p(80), _S1BOOSTPP_NODE_72, _S1BOOSTPP_NODE_88)
#                define _S1BOOSTPP_NODE_72(p) _S1BOOSTPP_IIF(p(72), _S1BOOSTPP_NODE_68, _S1BOOSTPP_NODE_76)
#                    define _S1BOOSTPP_NODE_68(p) _S1BOOSTPP_IIF(p(68), _S1BOOSTPP_NODE_66, _S1BOOSTPP_NODE_70)
#                        define _S1BOOSTPP_NODE_66(p) _S1BOOSTPP_IIF(p(66), _S1BOOSTPP_NODE_65, _S1BOOSTPP_NODE_67)
#                            define _S1BOOSTPP_NODE_65(p) _S1BOOSTPP_IIF(p(65), 65, 66)
#                            define _S1BOOSTPP_NODE_67(p) _S1BOOSTPP_IIF(p(67), 67, 68)
#                        define _S1BOOSTPP_NODE_70(p) _S1BOOSTPP_IIF(p(70), _S1BOOSTPP_NODE_69, _S1BOOSTPP_NODE_71)
#                            define _S1BOOSTPP_NODE_69(p) _S1BOOSTPP_IIF(p(69), 69, 70)
#                            define _S1BOOSTPP_NODE_71(p) _S1BOOSTPP_IIF(p(71), 71, 72)
#                    define _S1BOOSTPP_NODE_76(p) _S1BOOSTPP_IIF(p(76), _S1BOOSTPP_NODE_74, _S1BOOSTPP_NODE_78)
#                        define _S1BOOSTPP_NODE_74(p) _S1BOOSTPP_IIF(p(74), _S1BOOSTPP_NODE_73, _S1BOOSTPP_NODE_75)
#                            define _S1BOOSTPP_NODE_73(p) _S1BOOSTPP_IIF(p(73), 73, 74)
#                            define _S1BOOSTPP_NODE_75(p) _S1BOOSTPP_IIF(p(75), 75, 76)
#                        define _S1BOOSTPP_NODE_78(p) _S1BOOSTPP_IIF(p(78), _S1BOOSTPP_NODE_77, _S1BOOSTPP_NODE_79)
#                            define _S1BOOSTPP_NODE_77(p) _S1BOOSTPP_IIF(p(77), 77, 78)
#                            define _S1BOOSTPP_NODE_79(p) _S1BOOSTPP_IIF(p(79), 79, 80)
#                define _S1BOOSTPP_NODE_88(p) _S1BOOSTPP_IIF(p(88), _S1BOOSTPP_NODE_84, _S1BOOSTPP_NODE_92)
#                    define _S1BOOSTPP_NODE_84(p) _S1BOOSTPP_IIF(p(84), _S1BOOSTPP_NODE_82, _S1BOOSTPP_NODE_86)
#                        define _S1BOOSTPP_NODE_82(p) _S1BOOSTPP_IIF(p(82), _S1BOOSTPP_NODE_81, _S1BOOSTPP_NODE_83)
#                            define _S1BOOSTPP_NODE_81(p) _S1BOOSTPP_IIF(p(81), 81, 82)
#                            define _S1BOOSTPP_NODE_83(p) _S1BOOSTPP_IIF(p(83), 83, 84)
#                        define _S1BOOSTPP_NODE_86(p) _S1BOOSTPP_IIF(p(86), _S1BOOSTPP_NODE_85, _S1BOOSTPP_NODE_87)
#                            define _S1BOOSTPP_NODE_85(p) _S1BOOSTPP_IIF(p(85), 85, 86)
#                            define _S1BOOSTPP_NODE_87(p) _S1BOOSTPP_IIF(p(87), 87, 88)
#                    define _S1BOOSTPP_NODE_92(p) _S1BOOSTPP_IIF(p(92), _S1BOOSTPP_NODE_90, _S1BOOSTPP_NODE_94)
#                        define _S1BOOSTPP_NODE_90(p) _S1BOOSTPP_IIF(p(90), _S1BOOSTPP_NODE_89, _S1BOOSTPP_NODE_91)
#                            define _S1BOOSTPP_NODE_89(p) _S1BOOSTPP_IIF(p(89), 89, 90)
#                            define _S1BOOSTPP_NODE_91(p) _S1BOOSTPP_IIF(p(91), 91, 92)
#                        define _S1BOOSTPP_NODE_94(p) _S1BOOSTPP_IIF(p(94), _S1BOOSTPP_NODE_93, _S1BOOSTPP_NODE_95)
#                            define _S1BOOSTPP_NODE_93(p) _S1BOOSTPP_IIF(p(93), 93, 94)
#                            define _S1BOOSTPP_NODE_95(p) _S1BOOSTPP_IIF(p(95), 95, 96)
#            define _S1BOOSTPP_NODE_112(p) _S1BOOSTPP_IIF(p(112), _S1BOOSTPP_NODE_104, _S1BOOSTPP_NODE_120)
#                define _S1BOOSTPP_NODE_104(p) _S1BOOSTPP_IIF(p(104), _S1BOOSTPP_NODE_100, _S1BOOSTPP_NODE_108)
#                    define _S1BOOSTPP_NODE_100(p) _S1BOOSTPP_IIF(p(100), _S1BOOSTPP_NODE_98, _S1BOOSTPP_NODE_102)
#                        define _S1BOOSTPP_NODE_98(p) _S1BOOSTPP_IIF(p(98), _S1BOOSTPP_NODE_97, _S1BOOSTPP_NODE_99)
#                            define _S1BOOSTPP_NODE_97(p) _S1BOOSTPP_IIF(p(97), 97, 98)
#                            define _S1BOOSTPP_NODE_99(p) _S1BOOSTPP_IIF(p(99), 99, 100)
#                        define _S1BOOSTPP_NODE_102(p) _S1BOOSTPP_IIF(p(102), _S1BOOSTPP_NODE_101, _S1BOOSTPP_NODE_103)
#                            define _S1BOOSTPP_NODE_101(p) _S1BOOSTPP_IIF(p(101), 101, 102)
#                            define _S1BOOSTPP_NODE_103(p) _S1BOOSTPP_IIF(p(103), 103, 104)
#                    define _S1BOOSTPP_NODE_108(p) _S1BOOSTPP_IIF(p(108), _S1BOOSTPP_NODE_106, _S1BOOSTPP_NODE_110)
#                        define _S1BOOSTPP_NODE_106(p) _S1BOOSTPP_IIF(p(106), _S1BOOSTPP_NODE_105, _S1BOOSTPP_NODE_107)
#                            define _S1BOOSTPP_NODE_105(p) _S1BOOSTPP_IIF(p(105), 105, 106)
#                            define _S1BOOSTPP_NODE_107(p) _S1BOOSTPP_IIF(p(107), 107, 108)
#                        define _S1BOOSTPP_NODE_110(p) _S1BOOSTPP_IIF(p(110), _S1BOOSTPP_NODE_109, _S1BOOSTPP_NODE_111)
#                            define _S1BOOSTPP_NODE_109(p) _S1BOOSTPP_IIF(p(109), 109, 110)
#                            define _S1BOOSTPP_NODE_111(p) _S1BOOSTPP_IIF(p(111), 111, 112)
#                define _S1BOOSTPP_NODE_120(p) _S1BOOSTPP_IIF(p(120), _S1BOOSTPP_NODE_116, _S1BOOSTPP_NODE_124)
#                    define _S1BOOSTPP_NODE_116(p) _S1BOOSTPP_IIF(p(116), _S1BOOSTPP_NODE_114, _S1BOOSTPP_NODE_118)
#                        define _S1BOOSTPP_NODE_114(p) _S1BOOSTPP_IIF(p(114), _S1BOOSTPP_NODE_113, _S1BOOSTPP_NODE_115)
#                            define _S1BOOSTPP_NODE_113(p) _S1BOOSTPP_IIF(p(113), 113, 114)
#                            define _S1BOOSTPP_NODE_115(p) _S1BOOSTPP_IIF(p(115), 115, 116)
#                        define _S1BOOSTPP_NODE_118(p) _S1BOOSTPP_IIF(p(118), _S1BOOSTPP_NODE_117, _S1BOOSTPP_NODE_119)
#                            define _S1BOOSTPP_NODE_117(p) _S1BOOSTPP_IIF(p(117), 117, 118)
#                            define _S1BOOSTPP_NODE_119(p) _S1BOOSTPP_IIF(p(119), 119, 120)
#                    define _S1BOOSTPP_NODE_124(p) _S1BOOSTPP_IIF(p(124), _S1BOOSTPP_NODE_122, _S1BOOSTPP_NODE_126)
#                        define _S1BOOSTPP_NODE_122(p) _S1BOOSTPP_IIF(p(122), _S1BOOSTPP_NODE_121, _S1BOOSTPP_NODE_123)
#                            define _S1BOOSTPP_NODE_121(p) _S1BOOSTPP_IIF(p(121), 121, 122)
#                            define _S1BOOSTPP_NODE_123(p) _S1BOOSTPP_IIF(p(123), 123, 124)
#                        define _S1BOOSTPP_NODE_126(p) _S1BOOSTPP_IIF(p(126), _S1BOOSTPP_NODE_125, _S1BOOSTPP_NODE_127)
#                            define _S1BOOSTPP_NODE_125(p) _S1BOOSTPP_IIF(p(125), 125, 126)
#                            define _S1BOOSTPP_NODE_127(p) _S1BOOSTPP_IIF(p(127), 127, 128)
#    define _S1BOOSTPP_NODE_192(p) _S1BOOSTPP_IIF(p(192), _S1BOOSTPP_NODE_160, _S1BOOSTPP_NODE_224)
#        define _S1BOOSTPP_NODE_160(p) _S1BOOSTPP_IIF(p(160), _S1BOOSTPP_NODE_144, _S1BOOSTPP_NODE_176)
#            define _S1BOOSTPP_NODE_144(p) _S1BOOSTPP_IIF(p(144), _S1BOOSTPP_NODE_136, _S1BOOSTPP_NODE_152)
#                define _S1BOOSTPP_NODE_136(p) _S1BOOSTPP_IIF(p(136), _S1BOOSTPP_NODE_132, _S1BOOSTPP_NODE_140)
#                    define _S1BOOSTPP_NODE_132(p) _S1BOOSTPP_IIF(p(132), _S1BOOSTPP_NODE_130, _S1BOOSTPP_NODE_134)
#                        define _S1BOOSTPP_NODE_130(p) _S1BOOSTPP_IIF(p(130), _S1BOOSTPP_NODE_129, _S1BOOSTPP_NODE_131)
#                            define _S1BOOSTPP_NODE_129(p) _S1BOOSTPP_IIF(p(129), 129, 130)
#                            define _S1BOOSTPP_NODE_131(p) _S1BOOSTPP_IIF(p(131), 131, 132)
#                        define _S1BOOSTPP_NODE_134(p) _S1BOOSTPP_IIF(p(134), _S1BOOSTPP_NODE_133, _S1BOOSTPP_NODE_135)
#                            define _S1BOOSTPP_NODE_133(p) _S1BOOSTPP_IIF(p(133), 133, 134)
#                            define _S1BOOSTPP_NODE_135(p) _S1BOOSTPP_IIF(p(135), 135, 136)
#                    define _S1BOOSTPP_NODE_140(p) _S1BOOSTPP_IIF(p(140), _S1BOOSTPP_NODE_138, _S1BOOSTPP_NODE_142)
#                        define _S1BOOSTPP_NODE_138(p) _S1BOOSTPP_IIF(p(138), _S1BOOSTPP_NODE_137, _S1BOOSTPP_NODE_139)
#                            define _S1BOOSTPP_NODE_137(p) _S1BOOSTPP_IIF(p(137), 137, 138)
#                            define _S1BOOSTPP_NODE_139(p) _S1BOOSTPP_IIF(p(139), 139, 140)
#                        define _S1BOOSTPP_NODE_142(p) _S1BOOSTPP_IIF(p(142), _S1BOOSTPP_NODE_141, _S1BOOSTPP_NODE_143)
#                            define _S1BOOSTPP_NODE_141(p) _S1BOOSTPP_IIF(p(141), 141, 142)
#                            define _S1BOOSTPP_NODE_143(p) _S1BOOSTPP_IIF(p(143), 143, 144)
#                define _S1BOOSTPP_NODE_152(p) _S1BOOSTPP_IIF(p(152), _S1BOOSTPP_NODE_148, _S1BOOSTPP_NODE_156)
#                    define _S1BOOSTPP_NODE_148(p) _S1BOOSTPP_IIF(p(148), _S1BOOSTPP_NODE_146, _S1BOOSTPP_NODE_150)
#                        define _S1BOOSTPP_NODE_146(p) _S1BOOSTPP_IIF(p(146), _S1BOOSTPP_NODE_145, _S1BOOSTPP_NODE_147)
#                            define _S1BOOSTPP_NODE_145(p) _S1BOOSTPP_IIF(p(145), 145, 146)
#                            define _S1BOOSTPP_NODE_147(p) _S1BOOSTPP_IIF(p(147), 147, 148)
#                        define _S1BOOSTPP_NODE_150(p) _S1BOOSTPP_IIF(p(150), _S1BOOSTPP_NODE_149, _S1BOOSTPP_NODE_151)
#                            define _S1BOOSTPP_NODE_149(p) _S1BOOSTPP_IIF(p(149), 149, 150)
#                            define _S1BOOSTPP_NODE_151(p) _S1BOOSTPP_IIF(p(151), 151, 152)
#                    define _S1BOOSTPP_NODE_156(p) _S1BOOSTPP_IIF(p(156), _S1BOOSTPP_NODE_154, _S1BOOSTPP_NODE_158)
#                        define _S1BOOSTPP_NODE_154(p) _S1BOOSTPP_IIF(p(154), _S1BOOSTPP_NODE_153, _S1BOOSTPP_NODE_155)
#                            define _S1BOOSTPP_NODE_153(p) _S1BOOSTPP_IIF(p(153), 153, 154)
#                            define _S1BOOSTPP_NODE_155(p) _S1BOOSTPP_IIF(p(155), 155, 156)
#                        define _S1BOOSTPP_NODE_158(p) _S1BOOSTPP_IIF(p(158), _S1BOOSTPP_NODE_157, _S1BOOSTPP_NODE_159)
#                            define _S1BOOSTPP_NODE_157(p) _S1BOOSTPP_IIF(p(157), 157, 158)
#                            define _S1BOOSTPP_NODE_159(p) _S1BOOSTPP_IIF(p(159), 159, 160)
#            define _S1BOOSTPP_NODE_176(p) _S1BOOSTPP_IIF(p(176), _S1BOOSTPP_NODE_168, _S1BOOSTPP_NODE_184)
#                define _S1BOOSTPP_NODE_168(p) _S1BOOSTPP_IIF(p(168), _S1BOOSTPP_NODE_164, _S1BOOSTPP_NODE_172)
#                    define _S1BOOSTPP_NODE_164(p) _S1BOOSTPP_IIF(p(164), _S1BOOSTPP_NODE_162, _S1BOOSTPP_NODE_166)
#                        define _S1BOOSTPP_NODE_162(p) _S1BOOSTPP_IIF(p(162), _S1BOOSTPP_NODE_161, _S1BOOSTPP_NODE_163)
#                            define _S1BOOSTPP_NODE_161(p) _S1BOOSTPP_IIF(p(161), 161, 162)
#                            define _S1BOOSTPP_NODE_163(p) _S1BOOSTPP_IIF(p(163), 163, 164)
#                        define _S1BOOSTPP_NODE_166(p) _S1BOOSTPP_IIF(p(166), _S1BOOSTPP_NODE_165, _S1BOOSTPP_NODE_167)
#                            define _S1BOOSTPP_NODE_165(p) _S1BOOSTPP_IIF(p(165), 165, 166)
#                            define _S1BOOSTPP_NODE_167(p) _S1BOOSTPP_IIF(p(167), 167, 168)
#                    define _S1BOOSTPP_NODE_172(p) _S1BOOSTPP_IIF(p(172), _S1BOOSTPP_NODE_170, _S1BOOSTPP_NODE_174)
#                        define _S1BOOSTPP_NODE_170(p) _S1BOOSTPP_IIF(p(170), _S1BOOSTPP_NODE_169, _S1BOOSTPP_NODE_171)
#                            define _S1BOOSTPP_NODE_169(p) _S1BOOSTPP_IIF(p(169), 169, 170)
#                            define _S1BOOSTPP_NODE_171(p) _S1BOOSTPP_IIF(p(171), 171, 172)
#                        define _S1BOOSTPP_NODE_174(p) _S1BOOSTPP_IIF(p(174), _S1BOOSTPP_NODE_173, _S1BOOSTPP_NODE_175)
#                            define _S1BOOSTPP_NODE_173(p) _S1BOOSTPP_IIF(p(173), 173, 174)
#                            define _S1BOOSTPP_NODE_175(p) _S1BOOSTPP_IIF(p(175), 175, 176)
#                define _S1BOOSTPP_NODE_184(p) _S1BOOSTPP_IIF(p(184), _S1BOOSTPP_NODE_180, _S1BOOSTPP_NODE_188)
#                    define _S1BOOSTPP_NODE_180(p) _S1BOOSTPP_IIF(p(180), _S1BOOSTPP_NODE_178, _S1BOOSTPP_NODE_182)
#                        define _S1BOOSTPP_NODE_178(p) _S1BOOSTPP_IIF(p(178), _S1BOOSTPP_NODE_177, _S1BOOSTPP_NODE_179)
#                            define _S1BOOSTPP_NODE_177(p) _S1BOOSTPP_IIF(p(177), 177, 178)
#                            define _S1BOOSTPP_NODE_179(p) _S1BOOSTPP_IIF(p(179), 179, 180)
#                        define _S1BOOSTPP_NODE_182(p) _S1BOOSTPP_IIF(p(182), _S1BOOSTPP_NODE_181, _S1BOOSTPP_NODE_183)
#                            define _S1BOOSTPP_NODE_181(p) _S1BOOSTPP_IIF(p(181), 181, 182)
#                            define _S1BOOSTPP_NODE_183(p) _S1BOOSTPP_IIF(p(183), 183, 184)
#                    define _S1BOOSTPP_NODE_188(p) _S1BOOSTPP_IIF(p(188), _S1BOOSTPP_NODE_186, _S1BOOSTPP_NODE_190)
#                        define _S1BOOSTPP_NODE_186(p) _S1BOOSTPP_IIF(p(186), _S1BOOSTPP_NODE_185, _S1BOOSTPP_NODE_187)
#                            define _S1BOOSTPP_NODE_185(p) _S1BOOSTPP_IIF(p(185), 185, 186)
#                            define _S1BOOSTPP_NODE_187(p) _S1BOOSTPP_IIF(p(187), 187, 188)
#                        define _S1BOOSTPP_NODE_190(p) _S1BOOSTPP_IIF(p(190), _S1BOOSTPP_NODE_189, _S1BOOSTPP_NODE_191)
#                            define _S1BOOSTPP_NODE_189(p) _S1BOOSTPP_IIF(p(189), 189, 190)
#                            define _S1BOOSTPP_NODE_191(p) _S1BOOSTPP_IIF(p(191), 191, 192)
#        define _S1BOOSTPP_NODE_224(p) _S1BOOSTPP_IIF(p(224), _S1BOOSTPP_NODE_208, _S1BOOSTPP_NODE_240)
#            define _S1BOOSTPP_NODE_208(p) _S1BOOSTPP_IIF(p(208), _S1BOOSTPP_NODE_200, _S1BOOSTPP_NODE_216)
#                define _S1BOOSTPP_NODE_200(p) _S1BOOSTPP_IIF(p(200), _S1BOOSTPP_NODE_196, _S1BOOSTPP_NODE_204)
#                    define _S1BOOSTPP_NODE_196(p) _S1BOOSTPP_IIF(p(196), _S1BOOSTPP_NODE_194, _S1BOOSTPP_NODE_198)
#                        define _S1BOOSTPP_NODE_194(p) _S1BOOSTPP_IIF(p(194), _S1BOOSTPP_NODE_193, _S1BOOSTPP_NODE_195)
#                            define _S1BOOSTPP_NODE_193(p) _S1BOOSTPP_IIF(p(193), 193, 194)
#                            define _S1BOOSTPP_NODE_195(p) _S1BOOSTPP_IIF(p(195), 195, 196)
#                        define _S1BOOSTPP_NODE_198(p) _S1BOOSTPP_IIF(p(198), _S1BOOSTPP_NODE_197, _S1BOOSTPP_NODE_199)
#                            define _S1BOOSTPP_NODE_197(p) _S1BOOSTPP_IIF(p(197), 197, 198)
#                            define _S1BOOSTPP_NODE_199(p) _S1BOOSTPP_IIF(p(199), 199, 200)
#                    define _S1BOOSTPP_NODE_204(p) _S1BOOSTPP_IIF(p(204), _S1BOOSTPP_NODE_202, _S1BOOSTPP_NODE_206)
#                        define _S1BOOSTPP_NODE_202(p) _S1BOOSTPP_IIF(p(202), _S1BOOSTPP_NODE_201, _S1BOOSTPP_NODE_203)
#                            define _S1BOOSTPP_NODE_201(p) _S1BOOSTPP_IIF(p(201), 201, 202)
#                            define _S1BOOSTPP_NODE_203(p) _S1BOOSTPP_IIF(p(203), 203, 204)
#                        define _S1BOOSTPP_NODE_206(p) _S1BOOSTPP_IIF(p(206), _S1BOOSTPP_NODE_205, _S1BOOSTPP_NODE_207)
#                            define _S1BOOSTPP_NODE_205(p) _S1BOOSTPP_IIF(p(205), 205, 206)
#                            define _S1BOOSTPP_NODE_207(p) _S1BOOSTPP_IIF(p(207), 207, 208)
#                define _S1BOOSTPP_NODE_216(p) _S1BOOSTPP_IIF(p(216), _S1BOOSTPP_NODE_212, _S1BOOSTPP_NODE_220)
#                    define _S1BOOSTPP_NODE_212(p) _S1BOOSTPP_IIF(p(212), _S1BOOSTPP_NODE_210, _S1BOOSTPP_NODE_214)
#                        define _S1BOOSTPP_NODE_210(p) _S1BOOSTPP_IIF(p(210), _S1BOOSTPP_NODE_209, _S1BOOSTPP_NODE_211)
#                            define _S1BOOSTPP_NODE_209(p) _S1BOOSTPP_IIF(p(209), 209, 210)
#                            define _S1BOOSTPP_NODE_211(p) _S1BOOSTPP_IIF(p(211), 211, 212)
#                        define _S1BOOSTPP_NODE_214(p) _S1BOOSTPP_IIF(p(214), _S1BOOSTPP_NODE_213, _S1BOOSTPP_NODE_215)
#                            define _S1BOOSTPP_NODE_213(p) _S1BOOSTPP_IIF(p(213), 213, 214)
#                            define _S1BOOSTPP_NODE_215(p) _S1BOOSTPP_IIF(p(215), 215, 216)
#                    define _S1BOOSTPP_NODE_220(p) _S1BOOSTPP_IIF(p(220), _S1BOOSTPP_NODE_218, _S1BOOSTPP_NODE_222)
#                        define _S1BOOSTPP_NODE_218(p) _S1BOOSTPP_IIF(p(218), _S1BOOSTPP_NODE_217, _S1BOOSTPP_NODE_219)
#                            define _S1BOOSTPP_NODE_217(p) _S1BOOSTPP_IIF(p(217), 217, 218)
#                            define _S1BOOSTPP_NODE_219(p) _S1BOOSTPP_IIF(p(219), 219, 220)
#                        define _S1BOOSTPP_NODE_222(p) _S1BOOSTPP_IIF(p(222), _S1BOOSTPP_NODE_221, _S1BOOSTPP_NODE_223)
#                            define _S1BOOSTPP_NODE_221(p) _S1BOOSTPP_IIF(p(221), 221, 222)
#                            define _S1BOOSTPP_NODE_223(p) _S1BOOSTPP_IIF(p(223), 223, 224)
#            define _S1BOOSTPP_NODE_240(p) _S1BOOSTPP_IIF(p(240), _S1BOOSTPP_NODE_232, _S1BOOSTPP_NODE_248)
#                define _S1BOOSTPP_NODE_232(p) _S1BOOSTPP_IIF(p(232), _S1BOOSTPP_NODE_228, _S1BOOSTPP_NODE_236)
#                    define _S1BOOSTPP_NODE_228(p) _S1BOOSTPP_IIF(p(228), _S1BOOSTPP_NODE_226, _S1BOOSTPP_NODE_230)
#                        define _S1BOOSTPP_NODE_226(p) _S1BOOSTPP_IIF(p(226), _S1BOOSTPP_NODE_225, _S1BOOSTPP_NODE_227)
#                            define _S1BOOSTPP_NODE_225(p) _S1BOOSTPP_IIF(p(225), 225, 226)
#                            define _S1BOOSTPP_NODE_227(p) _S1BOOSTPP_IIF(p(227), 227, 228)
#                        define _S1BOOSTPP_NODE_230(p) _S1BOOSTPP_IIF(p(230), _S1BOOSTPP_NODE_229, _S1BOOSTPP_NODE_231)
#                            define _S1BOOSTPP_NODE_229(p) _S1BOOSTPP_IIF(p(229), 229, 230)
#                            define _S1BOOSTPP_NODE_231(p) _S1BOOSTPP_IIF(p(231), 231, 232)
#                    define _S1BOOSTPP_NODE_236(p) _S1BOOSTPP_IIF(p(236), _S1BOOSTPP_NODE_234, _S1BOOSTPP_NODE_238)
#                        define _S1BOOSTPP_NODE_234(p) _S1BOOSTPP_IIF(p(234), _S1BOOSTPP_NODE_233, _S1BOOSTPP_NODE_235)
#                            define _S1BOOSTPP_NODE_233(p) _S1BOOSTPP_IIF(p(233), 233, 234)
#                            define _S1BOOSTPP_NODE_235(p) _S1BOOSTPP_IIF(p(235), 235, 236)
#                        define _S1BOOSTPP_NODE_238(p) _S1BOOSTPP_IIF(p(238), _S1BOOSTPP_NODE_237, _S1BOOSTPP_NODE_239)
#                            define _S1BOOSTPP_NODE_237(p) _S1BOOSTPP_IIF(p(237), 237, 238)
#                            define _S1BOOSTPP_NODE_239(p) _S1BOOSTPP_IIF(p(239), 239, 240)
#                define _S1BOOSTPP_NODE_248(p) _S1BOOSTPP_IIF(p(248), _S1BOOSTPP_NODE_244, _S1BOOSTPP_NODE_252)
#                    define _S1BOOSTPP_NODE_244(p) _S1BOOSTPP_IIF(p(244), _S1BOOSTPP_NODE_242, _S1BOOSTPP_NODE_246)
#                        define _S1BOOSTPP_NODE_242(p) _S1BOOSTPP_IIF(p(242), _S1BOOSTPP_NODE_241, _S1BOOSTPP_NODE_243)
#                            define _S1BOOSTPP_NODE_241(p) _S1BOOSTPP_IIF(p(241), 241, 242)
#                            define _S1BOOSTPP_NODE_243(p) _S1BOOSTPP_IIF(p(243), 243, 244)
#                        define _S1BOOSTPP_NODE_246(p) _S1BOOSTPP_IIF(p(246), _S1BOOSTPP_NODE_245, _S1BOOSTPP_NODE_247)
#                            define _S1BOOSTPP_NODE_245(p) _S1BOOSTPP_IIF(p(245), 245, 246)
#                            define _S1BOOSTPP_NODE_247(p) _S1BOOSTPP_IIF(p(247), 247, 248)
#                    define _S1BOOSTPP_NODE_252(p) _S1BOOSTPP_IIF(p(252), _S1BOOSTPP_NODE_250, _S1BOOSTPP_NODE_254)
#                        define _S1BOOSTPP_NODE_250(p) _S1BOOSTPP_IIF(p(250), _S1BOOSTPP_NODE_249, _S1BOOSTPP_NODE_251)
#                            define _S1BOOSTPP_NODE_249(p) _S1BOOSTPP_IIF(p(249), 249, 250)
#                            define _S1BOOSTPP_NODE_251(p) _S1BOOSTPP_IIF(p(251), 251, 252)
#                        define _S1BOOSTPP_NODE_254(p) _S1BOOSTPP_IIF(p(254), _S1BOOSTPP_NODE_253, _S1BOOSTPP_NODE_255)
#                            define _S1BOOSTPP_NODE_253(p) _S1BOOSTPP_IIF(p(253), 253, 254)
#                            define _S1BOOSTPP_NODE_255(p) _S1BOOSTPP_IIF(p(255), 255, 256)
#
# endif
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_REPETITION_DETAIL_FOR_HPP
# define _S1BOOSTPP_REPETITION_DETAIL_FOR_HPP
#
#
# define _S1BOOSTPP_FOR_1(s, p, o, m) _S1BOOSTPP_FOR_1_C(_S1BOOSTPP_BOOL(p(2, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_2(s, p, o, m) _S1BOOSTPP_FOR_2_C(_S1BOOSTPP_BOOL(p(3, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_3(s, p, o, m) _S1BOOSTPP_FOR_3_C(_S1BOOSTPP_BOOL(p(4, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_4(s, p, o, m) _S1BOOSTPP_FOR_4_C(_S1BOOSTPP_BOOL(p(5, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_5(s, p, o, m) _S1BOOSTPP_FOR_5_C(_S1BOOSTPP_BOOL(p(6, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_6(s, p, o, m) _S1BOOSTPP_FOR_6_C(_S1BOOSTPP_BOOL(p(7, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_7(s, p, o, m) _S1BOOSTPP_FOR_7_C(_S1BOOSTPP_BOOL(p(8, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_8(s, p, o, m) _S1BOOSTPP_FOR_8_C(_S1BOOSTPP_BOOL(p(9, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_9(s, p, o, m) _S1BOOSTPP_FOR_9_C(_S1BOOSTPP_BOOL(p(10, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_10(s, p, o, m) _S1BOOSTPP_FOR_10_C(_S1BOOSTPP_BOOL(p(11, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_11(s, p, o, m) _S1BOOSTPP_FOR_11_C(_S1BOOSTPP_BOOL(p(12, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_12(s, p, o, m) _S1BOOSTPP_FOR_12_C(_S1BOOSTPP_BOOL(p(13, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_13(s, p, o, m) _S1BOOSTPP_FOR_13_C(_S1BOOSTPP_BOOL(p(14, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_14(s, p, o, m) _S1BOOSTPP_FOR_14_C(_S1BOOSTPP_BOOL(p(15, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_15(s, p, o, m) _S1BOOSTPP_FOR_15_C(_S1BOOSTPP_BOOL(p(16, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_16(s, p, o, m) _S1BOOSTPP_FOR_16_C(_S1BOOSTPP_BOOL(p(17, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_17(s, p, o, m) _S1BOOSTPP_FOR_17_C(_S1BOOSTPP_BOOL(p(18, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_18(s, p, o, m) _S1BOOSTPP_FOR_18_C(_S1BOOSTPP_BOOL(p(19, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_19(s, p, o, m) _S1BOOSTPP_FOR_19_C(_S1BOOSTPP_BOOL(p(20, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_20(s, p, o, m) _S1BOOSTPP_FOR_20_C(_S1BOOSTPP_BOOL(p(21, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_21(s, p, o, m) _S1BOOSTPP_FOR_21_C(_S1BOOSTPP_BOOL(p(22, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_22(s, p, o, m) _S1BOOSTPP_FOR_22_C(_S1BOOSTPP_BOOL(p(23, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_23(s, p, o, m) _S1BOOSTPP_FOR_23_C(_S1BOOSTPP_BOOL(p(24, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_24(s, p, o, m) _S1BOOSTPP_FOR_24_C(_S1BOOSTPP_BOOL(p(25, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_25(s, p, o, m) _S1BOOSTPP_FOR_25_C(_S1BOOSTPP_BOOL(p(26, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_26(s, p, o, m) _S1BOOSTPP_FOR_26_C(_S1BOOSTPP_BOOL(p(27, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_27(s, p, o, m) _S1BOOSTPP_FOR_27_C(_S1BOOSTPP_BOOL(p(28, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_28(s, p, o, m) _S1BOOSTPP_FOR_28_C(_S1BOOSTPP_BOOL(p(29, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_29(s, p, o, m) _S1BOOSTPP_FOR_29_C(_S1BOOSTPP_BOOL(p(30, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_30(s, p, o, m) _S1BOOSTPP_FOR_30_C(_S1BOOSTPP_BOOL(p(31, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_31(s, p, o, m) _S1BOOSTPP_FOR_31_C(_S1BOOSTPP_BOOL(p(32, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_32(s, p, o, m) _S1BOOSTPP_FOR_32_C(_S1BOOSTPP_BOOL(p(33, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_33(s, p, o, m) _S1BOOSTPP_FOR_33_C(_S1BOOSTPP_BOOL(p(34, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_34(s, p, o, m) _S1BOOSTPP_FOR_34_C(_S1BOOSTPP_BOOL(p(35, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_35(s, p, o, m) _S1BOOSTPP_FOR_35_C(_S1BOOSTPP_BOOL(p(36, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_36(s, p, o, m) _S1BOOSTPP_FOR_36_C(_S1BOOSTPP_BOOL(p(37, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_37(s, p, o, m) _S1BOOSTPP_FOR_37_C(_S1BOOSTPP_BOOL(p(38, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_38(s, p, o, m) _S1BOOSTPP_FOR_38_C(_S1BOOSTPP_BOOL(p(39, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_39(s, p, o, m) _S1BOOSTPP_FOR_39_C(_S1BOOSTPP_BOOL(p(40, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_40(s, p, o, m) _S1BOOSTPP_FOR_40_C(_S1BOOSTPP_BOOL(p(41, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_41(s, p, o, m) _S1BOOSTPP_FOR_41_C(_S1BOOSTPP_BOOL(p(42, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_42(s, p, o, m) _S1BOOSTPP_FOR_42_C(_S1BOOSTPP_BOOL(p(43, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_43(s, p, o, m) _S1BOOSTPP_FOR_43_C(_S1BOOSTPP_BOOL(p(44, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_44(s, p, o, m) _S1BOOSTPP_FOR_44_C(_S1BOOSTPP_BOOL(p(45, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_45(s, p, o, m) _S1BOOSTPP_FOR_45_C(_S1BOOSTPP_BOOL(p(46, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_46(s, p, o, m) _S1BOOSTPP_FOR_46_C(_S1BOOSTPP_BOOL(p(47, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_47(s, p, o, m) _S1BOOSTPP_FOR_47_C(_S1BOOSTPP_BOOL(p(48, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_48(s, p, o, m) _S1BOOSTPP_FOR_48_C(_S1BOOSTPP_BOOL(p(49, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_49(s, p, o, m) _S1BOOSTPP_FOR_49_C(_S1BOOSTPP_BOOL(p(50, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_50(s, p, o, m) _S1BOOSTPP_FOR_50_C(_S1BOOSTPP_BOOL(p(51, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_51(s, p, o, m) _S1BOOSTPP_FOR_51_C(_S1BOOSTPP_BOOL(p(52, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_52(s, p, o, m) _S1BOOSTPP_FOR_52_C(_S1BOOSTPP_BOOL(p(53, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_53(s, p, o, m) _S1BOOSTPP_FOR_53_C(_S1BOOSTPP_BOOL(p(54, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_54(s, p, o, m) _S1BOOSTPP_FOR_54_C(_S1BOOSTPP_BOOL(p(55, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_55(s, p, o, m) _S1BOOSTPP_FOR_55_C(_S1BOOSTPP_BOOL(p(56, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_56(s, p, o, m) _S1BOOSTPP_FOR_56_C(_S1BOOSTPP_BOOL(p(57, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_57(s, p, o, m) _S1BOOSTPP_FOR_57_C(_S1BOOSTPP_BOOL(p(58, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_58(s, p, o, m) _S1BOOSTPP_FOR_58_C(_S1BOOSTPP_BOOL(p(59, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_59(s, p, o, m) _S1BOOSTPP_FOR_59_C(_S1BOOSTPP_BOOL(p(60, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_60(s, p, o, m) _S1BOOSTPP_FOR_60_C(_S1BOOSTPP_BOOL(p(61, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_61(s, p, o, m) _S1BOOSTPP_FOR_61_C(_S1BOOSTPP_BOOL(p(62, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_62(s, p, o, m) _S1BOOSTPP_FOR_62_C(_S1BOOSTPP_BOOL(p(63, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_63(s, p, o, m) _S1BOOSTPP_FOR_63_C(_S1BOOSTPP_BOOL(p(64, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_64(s, p, o, m) _S1BOOSTPP_FOR_64_C(_S1BOOSTPP_BOOL(p(65, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_65(s, p, o, m) _S1BOOSTPP_FOR_65_C(_S1BOOSTPP_BOOL(p(66, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_66(s, p, o, m) _S1BOOSTPP_FOR_66_C(_S1BOOSTPP_BOOL(p(67, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_67(s, p, o, m) _S1BOOSTPP_FOR_67_C(_S1BOOSTPP_BOOL(p(68, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_68(s, p, o, m) _S1BOOSTPP_FOR_68_C(_S1BOOSTPP_BOOL(p(69, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_69(s, p, o, m) _S1BOOSTPP_FOR_69_C(_S1BOOSTPP_BOOL(p(70, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_70(s, p, o, m) _S1BOOSTPP_FOR_70_C(_S1BOOSTPP_BOOL(p(71, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_71(s, p, o, m) _S1BOOSTPP_FOR_71_C(_S1BOOSTPP_BOOL(p(72, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_72(s, p, o, m) _S1BOOSTPP_FOR_72_C(_S1BOOSTPP_BOOL(p(73, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_73(s, p, o, m) _S1BOOSTPP_FOR_73_C(_S1BOOSTPP_BOOL(p(74, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_74(s, p, o, m) _S1BOOSTPP_FOR_74_C(_S1BOOSTPP_BOOL(p(75, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_75(s, p, o, m) _S1BOOSTPP_FOR_75_C(_S1BOOSTPP_BOOL(p(76, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_76(s, p, o, m) _S1BOOSTPP_FOR_76_C(_S1BOOSTPP_BOOL(p(77, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_77(s, p, o, m) _S1BOOSTPP_FOR_77_C(_S1BOOSTPP_BOOL(p(78, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_78(s, p, o, m) _S1BOOSTPP_FOR_78_C(_S1BOOSTPP_BOOL(p(79, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_79(s, p, o, m) _S1BOOSTPP_FOR_79_C(_S1BOOSTPP_BOOL(p(80, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_80(s, p, o, m) _S1BOOSTPP_FOR_80_C(_S1BOOSTPP_BOOL(p(81, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_81(s, p, o, m) _S1BOOSTPP_FOR_81_C(_S1BOOSTPP_BOOL(p(82, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_82(s, p, o, m) _S1BOOSTPP_FOR_82_C(_S1BOOSTPP_BOOL(p(83, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_83(s, p, o, m) _S1BOOSTPP_FOR_83_C(_S1BOOSTPP_BOOL(p(84, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_84(s, p, o, m) _S1BOOSTPP_FOR_84_C(_S1BOOSTPP_BOOL(p(85, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_85(s, p, o, m) _S1BOOSTPP_FOR_85_C(_S1BOOSTPP_BOOL(p(86, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_86(s, p, o, m) _S1BOOSTPP_FOR_86_C(_S1BOOSTPP_BOOL(p(87, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_87(s, p, o, m) _S1BOOSTPP_FOR_87_C(_S1BOOSTPP_BOOL(p(88, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_88(s, p, o, m) _S1BOOSTPP_FOR_88_C(_S1BOOSTPP_BOOL(p(89, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_89(s, p, o, m) _S1BOOSTPP_FOR_89_C(_S1BOOSTPP_BOOL(p(90, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_90(s, p, o, m) _S1BOOSTPP_FOR_90_C(_S1BOOSTPP_BOOL(p(91, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_91(s, p, o, m) _S1BOOSTPP_FOR_91_C(_S1BOOSTPP_BOOL(p(92, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_92(s, p, o, m) _S1BOOSTPP_FOR_92_C(_S1BOOSTPP_BOOL(p(93, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_93(s, p, o, m) _S1BOOSTPP_FOR_93_C(_S1BOOSTPP_BOOL(p(94, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_94(s, p, o, m) _S1BOOSTPP_FOR_94_C(_S1BOOSTPP_BOOL(p(95, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_95(s, p, o, m) _S1BOOSTPP_FOR_95_C(_S1BOOSTPP_BOOL(p(96, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_96(s, p, o, m) _S1BOOSTPP_FOR_96_C(_S1BOOSTPP_BOOL(p(97, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_97(s, p, o, m) _S1BOOSTPP_FOR_97_C(_S1BOOSTPP_BOOL(p(98, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_98(s, p, o, m) _S1BOOSTPP_FOR_98_C(_S1BOOSTPP_BOOL(p(99, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_99(s, p, o, m) _S1BOOSTPP_FOR_99_C(_S1BOOSTPP_BOOL(p(100, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_100(s, p, o, m) _S1BOOSTPP_FOR_100_C(_S1BOOSTPP_BOOL(p(101, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_101(s, p, o, m) _S1BOOSTPP_FOR_101_C(_S1BOOSTPP_BOOL(p(102, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_102(s, p, o, m) _S1BOOSTPP_FOR_102_C(_S1BOOSTPP_BOOL(p(103, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_103(s, p, o, m) _S1BOOSTPP_FOR_103_C(_S1BOOSTPP_BOOL(p(104, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_104(s, p, o, m) _S1BOOSTPP_FOR_104_C(_S1BOOSTPP_BOOL(p(105, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_105(s, p, o, m) _S1BOOSTPP_FOR_105_C(_S1BOOSTPP_BOOL(p(106, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_106(s, p, o, m) _S1BOOSTPP_FOR_106_C(_S1BOOSTPP_BOOL(p(107, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_107(s, p, o, m) _S1BOOSTPP_FOR_107_C(_S1BOOSTPP_BOOL(p(108, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_108(s, p, o, m) _S1BOOSTPP_FOR_108_C(_S1BOOSTPP_BOOL(p(109, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_109(s, p, o, m) _S1BOOSTPP_FOR_109_C(_S1BOOSTPP_BOOL(p(110, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_110(s, p, o, m) _S1BOOSTPP_FOR_110_C(_S1BOOSTPP_BOOL(p(111, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_111(s, p, o, m) _S1BOOSTPP_FOR_111_C(_S1BOOSTPP_BOOL(p(112, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_112(s, p, o, m) _S1BOOSTPP_FOR_112_C(_S1BOOSTPP_BOOL(p(113, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_113(s, p, o, m) _S1BOOSTPP_FOR_113_C(_S1BOOSTPP_BOOL(p(114, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_114(s, p, o, m) _S1BOOSTPP_FOR_114_C(_S1BOOSTPP_BOOL(p(115, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_115(s, p, o, m) _S1BOOSTPP_FOR_115_C(_S1BOOSTPP_BOOL(p(116, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_116(s, p, o, m) _S1BOOSTPP_FOR_116_C(_S1BOOSTPP_BOOL(p(117, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_117(s, p, o, m) _S1BOOSTPP_FOR_117_C(_S1BOOSTPP_BOOL(p(118, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_118(s, p, o, m) _S1BOOSTPP_FOR_118_C(_S1BOOSTPP_BOOL(p(119, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_119(s, p, o, m) _S1BOOSTPP_FOR_119_C(_S1BOOSTPP_BOOL(p(120, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_120(s, p, o, m) _S1BOOSTPP_FOR_120_C(_S1BOOSTPP_BOOL(p(121, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_121(s, p, o, m) _S1BOOSTPP_FOR_121_C(_S1BOOSTPP_BOOL(p(122, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_122(s, p, o, m) _S1BOOSTPP_FOR_122_C(_S1BOOSTPP_BOOL(p(123, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_123(s, p, o, m) _S1BOOSTPP_FOR_123_C(_S1BOOSTPP_BOOL(p(124, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_124(s, p, o, m) _S1BOOSTPP_FOR_124_C(_S1BOOSTPP_BOOL(p(125, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_125(s, p, o, m) _S1BOOSTPP_FOR_125_C(_S1BOOSTPP_BOOL(p(126, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_126(s, p, o, m) _S1BOOSTPP_FOR_126_C(_S1BOOSTPP_BOOL(p(127, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_127(s, p, o, m) _S1BOOSTPP_FOR_127_C(_S1BOOSTPP_BOOL(p(128, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_128(s, p, o, m) _S1BOOSTPP_FOR_128_C(_S1BOOSTPP_BOOL(p(129, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_129(s, p, o, m) _S1BOOSTPP_FOR_129_C(_S1BOOSTPP_BOOL(p(130, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_130(s, p, o, m) _S1BOOSTPP_FOR_130_C(_S1BOOSTPP_BOOL(p(131, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_131(s, p, o, m) _S1BOOSTPP_FOR_131_C(_S1BOOSTPP_BOOL(p(132, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_132(s, p, o, m) _S1BOOSTPP_FOR_132_C(_S1BOOSTPP_BOOL(p(133, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_133(s, p, o, m) _S1BOOSTPP_FOR_133_C(_S1BOOSTPP_BOOL(p(134, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_134(s, p, o, m) _S1BOOSTPP_FOR_134_C(_S1BOOSTPP_BOOL(p(135, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_135(s, p, o, m) _S1BOOSTPP_FOR_135_C(_S1BOOSTPP_BOOL(p(136, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_136(s, p, o, m) _S1BOOSTPP_FOR_136_C(_S1BOOSTPP_BOOL(p(137, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_137(s, p, o, m) _S1BOOSTPP_FOR_137_C(_S1BOOSTPP_BOOL(p(138, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_138(s, p, o, m) _S1BOOSTPP_FOR_138_C(_S1BOOSTPP_BOOL(p(139, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_139(s, p, o, m) _S1BOOSTPP_FOR_139_C(_S1BOOSTPP_BOOL(p(140, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_140(s, p, o, m) _S1BOOSTPP_FOR_140_C(_S1BOOSTPP_BOOL(p(141, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_141(s, p, o, m) _S1BOOSTPP_FOR_141_C(_S1BOOSTPP_BOOL(p(142, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_142(s, p, o, m) _S1BOOSTPP_FOR_142_C(_S1BOOSTPP_BOOL(p(143, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_143(s, p, o, m) _S1BOOSTPP_FOR_143_C(_S1BOOSTPP_BOOL(p(144, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_144(s, p, o, m) _S1BOOSTPP_FOR_144_C(_S1BOOSTPP_BOOL(p(145, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_145(s, p, o, m) _S1BOOSTPP_FOR_145_C(_S1BOOSTPP_BOOL(p(146, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_146(s, p, o, m) _S1BOOSTPP_FOR_146_C(_S1BOOSTPP_BOOL(p(147, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_147(s, p, o, m) _S1BOOSTPP_FOR_147_C(_S1BOOSTPP_BOOL(p(148, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_148(s, p, o, m) _S1BOOSTPP_FOR_148_C(_S1BOOSTPP_BOOL(p(149, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_149(s, p, o, m) _S1BOOSTPP_FOR_149_C(_S1BOOSTPP_BOOL(p(150, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_150(s, p, o, m) _S1BOOSTPP_FOR_150_C(_S1BOOSTPP_BOOL(p(151, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_151(s, p, o, m) _S1BOOSTPP_FOR_151_C(_S1BOOSTPP_BOOL(p(152, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_152(s, p, o, m) _S1BOOSTPP_FOR_152_C(_S1BOOSTPP_BOOL(p(153, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_153(s, p, o, m) _S1BOOSTPP_FOR_153_C(_S1BOOSTPP_BOOL(p(154, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_154(s, p, o, m) _S1BOOSTPP_FOR_154_C(_S1BOOSTPP_BOOL(p(155, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_155(s, p, o, m) _S1BOOSTPP_FOR_155_C(_S1BOOSTPP_BOOL(p(156, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_156(s, p, o, m) _S1BOOSTPP_FOR_156_C(_S1BOOSTPP_BOOL(p(157, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_157(s, p, o, m) _S1BOOSTPP_FOR_157_C(_S1BOOSTPP_BOOL(p(158, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_158(s, p, o, m) _S1BOOSTPP_FOR_158_C(_S1BOOSTPP_BOOL(p(159, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_159(s, p, o, m) _S1BOOSTPP_FOR_159_C(_S1BOOSTPP_BOOL(p(160, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_160(s, p, o, m) _S1BOOSTPP_FOR_160_C(_S1BOOSTPP_BOOL(p(161, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_161(s, p, o, m) _S1BOOSTPP_FOR_161_C(_S1BOOSTPP_BOOL(p(162, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_162(s, p, o, m) _S1BOOSTPP_FOR_162_C(_S1BOOSTPP_BOOL(p(163, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_163(s, p, o, m) _S1BOOSTPP_FOR_163_C(_S1BOOSTPP_BOOL(p(164, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_164(s, p, o, m) _S1BOOSTPP_FOR_164_C(_S1BOOSTPP_BOOL(p(165, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_165(s, p, o, m) _S1BOOSTPP_FOR_165_C(_S1BOOSTPP_BOOL(p(166, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_166(s, p, o, m) _S1BOOSTPP_FOR_166_C(_S1BOOSTPP_BOOL(p(167, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_167(s, p, o, m) _S1BOOSTPP_FOR_167_C(_S1BOOSTPP_BOOL(p(168, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_168(s, p, o, m) _S1BOOSTPP_FOR_168_C(_S1BOOSTPP_BOOL(p(169, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_169(s, p, o, m) _S1BOOSTPP_FOR_169_C(_S1BOOSTPP_BOOL(p(170, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_170(s, p, o, m) _S1BOOSTPP_FOR_170_C(_S1BOOSTPP_BOOL(p(171, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_171(s, p, o, m) _S1BOOSTPP_FOR_171_C(_S1BOOSTPP_BOOL(p(172, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_172(s, p, o, m) _S1BOOSTPP_FOR_172_C(_S1BOOSTPP_BOOL(p(173, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_173(s, p, o, m) _S1BOOSTPP_FOR_173_C(_S1BOOSTPP_BOOL(p(174, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_174(s, p, o, m) _S1BOOSTPP_FOR_174_C(_S1BOOSTPP_BOOL(p(175, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_175(s, p, o, m) _S1BOOSTPP_FOR_175_C(_S1BOOSTPP_BOOL(p(176, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_176(s, p, o, m) _S1BOOSTPP_FOR_176_C(_S1BOOSTPP_BOOL(p(177, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_177(s, p, o, m) _S1BOOSTPP_FOR_177_C(_S1BOOSTPP_BOOL(p(178, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_178(s, p, o, m) _S1BOOSTPP_FOR_178_C(_S1BOOSTPP_BOOL(p(179, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_179(s, p, o, m) _S1BOOSTPP_FOR_179_C(_S1BOOSTPP_BOOL(p(180, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_180(s, p, o, m) _S1BOOSTPP_FOR_180_C(_S1BOOSTPP_BOOL(p(181, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_181(s, p, o, m) _S1BOOSTPP_FOR_181_C(_S1BOOSTPP_BOOL(p(182, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_182(s, p, o, m) _S1BOOSTPP_FOR_182_C(_S1BOOSTPP_BOOL(p(183, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_183(s, p, o, m) _S1BOOSTPP_FOR_183_C(_S1BOOSTPP_BOOL(p(184, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_184(s, p, o, m) _S1BOOSTPP_FOR_184_C(_S1BOOSTPP_BOOL(p(185, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_185(s, p, o, m) _S1BOOSTPP_FOR_185_C(_S1BOOSTPP_BOOL(p(186, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_186(s, p, o, m) _S1BOOSTPP_FOR_186_C(_S1BOOSTPP_BOOL(p(187, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_187(s, p, o, m) _S1BOOSTPP_FOR_187_C(_S1BOOSTPP_BOOL(p(188, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_188(s, p, o, m) _S1BOOSTPP_FOR_188_C(_S1BOOSTPP_BOOL(p(189, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_189(s, p, o, m) _S1BOOSTPP_FOR_189_C(_S1BOOSTPP_BOOL(p(190, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_190(s, p, o, m) _S1BOOSTPP_FOR_190_C(_S1BOOSTPP_BOOL(p(191, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_191(s, p, o, m) _S1BOOSTPP_FOR_191_C(_S1BOOSTPP_BOOL(p(192, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_192(s, p, o, m) _S1BOOSTPP_FOR_192_C(_S1BOOSTPP_BOOL(p(193, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_193(s, p, o, m) _S1BOOSTPP_FOR_193_C(_S1BOOSTPP_BOOL(p(194, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_194(s, p, o, m) _S1BOOSTPP_FOR_194_C(_S1BOOSTPP_BOOL(p(195, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_195(s, p, o, m) _S1BOOSTPP_FOR_195_C(_S1BOOSTPP_BOOL(p(196, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_196(s, p, o, m) _S1BOOSTPP_FOR_196_C(_S1BOOSTPP_BOOL(p(197, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_197(s, p, o, m) _S1BOOSTPP_FOR_197_C(_S1BOOSTPP_BOOL(p(198, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_198(s, p, o, m) _S1BOOSTPP_FOR_198_C(_S1BOOSTPP_BOOL(p(199, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_199(s, p, o, m) _S1BOOSTPP_FOR_199_C(_S1BOOSTPP_BOOL(p(200, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_200(s, p, o, m) _S1BOOSTPP_FOR_200_C(_S1BOOSTPP_BOOL(p(201, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_201(s, p, o, m) _S1BOOSTPP_FOR_201_C(_S1BOOSTPP_BOOL(p(202, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_202(s, p, o, m) _S1BOOSTPP_FOR_202_C(_S1BOOSTPP_BOOL(p(203, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_203(s, p, o, m) _S1BOOSTPP_FOR_203_C(_S1BOOSTPP_BOOL(p(204, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_204(s, p, o, m) _S1BOOSTPP_FOR_204_C(_S1BOOSTPP_BOOL(p(205, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_205(s, p, o, m) _S1BOOSTPP_FOR_205_C(_S1BOOSTPP_BOOL(p(206, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_206(s, p, o, m) _S1BOOSTPP_FOR_206_C(_S1BOOSTPP_BOOL(p(207, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_207(s, p, o, m) _S1BOOSTPP_FOR_207_C(_S1BOOSTPP_BOOL(p(208, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_208(s, p, o, m) _S1BOOSTPP_FOR_208_C(_S1BOOSTPP_BOOL(p(209, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_209(s, p, o, m) _S1BOOSTPP_FOR_209_C(_S1BOOSTPP_BOOL(p(210, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_210(s, p, o, m) _S1BOOSTPP_FOR_210_C(_S1BOOSTPP_BOOL(p(211, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_211(s, p, o, m) _S1BOOSTPP_FOR_211_C(_S1BOOSTPP_BOOL(p(212, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_212(s, p, o, m) _S1BOOSTPP_FOR_212_C(_S1BOOSTPP_BOOL(p(213, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_213(s, p, o, m) _S1BOOSTPP_FOR_213_C(_S1BOOSTPP_BOOL(p(214, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_214(s, p, o, m) _S1BOOSTPP_FOR_214_C(_S1BOOSTPP_BOOL(p(215, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_215(s, p, o, m) _S1BOOSTPP_FOR_215_C(_S1BOOSTPP_BOOL(p(216, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_216(s, p, o, m) _S1BOOSTPP_FOR_216_C(_S1BOOSTPP_BOOL(p(217, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_217(s, p, o, m) _S1BOOSTPP_FOR_217_C(_S1BOOSTPP_BOOL(p(218, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_218(s, p, o, m) _S1BOOSTPP_FOR_218_C(_S1BOOSTPP_BOOL(p(219, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_219(s, p, o, m) _S1BOOSTPP_FOR_219_C(_S1BOOSTPP_BOOL(p(220, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_220(s, p, o, m) _S1BOOSTPP_FOR_220_C(_S1BOOSTPP_BOOL(p(221, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_221(s, p, o, m) _S1BOOSTPP_FOR_221_C(_S1BOOSTPP_BOOL(p(222, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_222(s, p, o, m) _S1BOOSTPP_FOR_222_C(_S1BOOSTPP_BOOL(p(223, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_223(s, p, o, m) _S1BOOSTPP_FOR_223_C(_S1BOOSTPP_BOOL(p(224, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_224(s, p, o, m) _S1BOOSTPP_FOR_224_C(_S1BOOSTPP_BOOL(p(225, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_225(s, p, o, m) _S1BOOSTPP_FOR_225_C(_S1BOOSTPP_BOOL(p(226, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_226(s, p, o, m) _S1BOOSTPP_FOR_226_C(_S1BOOSTPP_BOOL(p(227, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_227(s, p, o, m) _S1BOOSTPP_FOR_227_C(_S1BOOSTPP_BOOL(p(228, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_228(s, p, o, m) _S1BOOSTPP_FOR_228_C(_S1BOOSTPP_BOOL(p(229, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_229(s, p, o, m) _S1BOOSTPP_FOR_229_C(_S1BOOSTPP_BOOL(p(230, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_230(s, p, o, m) _S1BOOSTPP_FOR_230_C(_S1BOOSTPP_BOOL(p(231, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_231(s, p, o, m) _S1BOOSTPP_FOR_231_C(_S1BOOSTPP_BOOL(p(232, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_232(s, p, o, m) _S1BOOSTPP_FOR_232_C(_S1BOOSTPP_BOOL(p(233, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_233(s, p, o, m) _S1BOOSTPP_FOR_233_C(_S1BOOSTPP_BOOL(p(234, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_234(s, p, o, m) _S1BOOSTPP_FOR_234_C(_S1BOOSTPP_BOOL(p(235, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_235(s, p, o, m) _S1BOOSTPP_FOR_235_C(_S1BOOSTPP_BOOL(p(236, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_236(s, p, o, m) _S1BOOSTPP_FOR_236_C(_S1BOOSTPP_BOOL(p(237, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_237(s, p, o, m) _S1BOOSTPP_FOR_237_C(_S1BOOSTPP_BOOL(p(238, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_238(s, p, o, m) _S1BOOSTPP_FOR_238_C(_S1BOOSTPP_BOOL(p(239, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_239(s, p, o, m) _S1BOOSTPP_FOR_239_C(_S1BOOSTPP_BOOL(p(240, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_240(s, p, o, m) _S1BOOSTPP_FOR_240_C(_S1BOOSTPP_BOOL(p(241, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_241(s, p, o, m) _S1BOOSTPP_FOR_241_C(_S1BOOSTPP_BOOL(p(242, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_242(s, p, o, m) _S1BOOSTPP_FOR_242_C(_S1BOOSTPP_BOOL(p(243, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_243(s, p, o, m) _S1BOOSTPP_FOR_243_C(_S1BOOSTPP_BOOL(p(244, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_244(s, p, o, m) _S1BOOSTPP_FOR_244_C(_S1BOOSTPP_BOOL(p(245, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_245(s, p, o, m) _S1BOOSTPP_FOR_245_C(_S1BOOSTPP_BOOL(p(246, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_246(s, p, o, m) _S1BOOSTPP_FOR_246_C(_S1BOOSTPP_BOOL(p(247, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_247(s, p, o, m) _S1BOOSTPP_FOR_247_C(_S1BOOSTPP_BOOL(p(248, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_248(s, p, o, m) _S1BOOSTPP_FOR_248_C(_S1BOOSTPP_BOOL(p(249, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_249(s, p, o, m) _S1BOOSTPP_FOR_249_C(_S1BOOSTPP_BOOL(p(250, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_250(s, p, o, m) _S1BOOSTPP_FOR_250_C(_S1BOOSTPP_BOOL(p(251, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_251(s, p, o, m) _S1BOOSTPP_FOR_251_C(_S1BOOSTPP_BOOL(p(252, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_252(s, p, o, m) _S1BOOSTPP_FOR_252_C(_S1BOOSTPP_BOOL(p(253, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_253(s, p, o, m) _S1BOOSTPP_FOR_253_C(_S1BOOSTPP_BOOL(p(254, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_254(s, p, o, m) _S1BOOSTPP_FOR_254_C(_S1BOOSTPP_BOOL(p(255, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_255(s, p, o, m) _S1BOOSTPP_FOR_255_C(_S1BOOSTPP_BOOL(p(256, s)), s, p, o, m)
# define _S1BOOSTPP_FOR_256(s, p, o, m) _S1BOOSTPP_FOR_256_C(_S1BOOSTPP_BOOL(p(257, s)), s, p, o, m)
#
# define _S1BOOSTPP_FOR_1_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(2, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_2, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(2, s), p, o, m)
# define _S1BOOSTPP_FOR_2_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(3, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_3, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(3, s), p, o, m)
# define _S1BOOSTPP_FOR_3_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(4, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_4, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(4, s), p, o, m)
# define _S1BOOSTPP_FOR_4_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(5, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_5, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(5, s), p, o, m)
# define _S1BOOSTPP_FOR_5_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(6, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_6, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(6, s), p, o, m)
# define _S1BOOSTPP_FOR_6_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(7, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_7, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(7, s), p, o, m)
# define _S1BOOSTPP_FOR_7_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(8, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_8, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(8, s), p, o, m)
# define _S1BOOSTPP_FOR_8_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(9, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_9, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(9, s), p, o, m)
# define _S1BOOSTPP_FOR_9_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(10, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_10, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(10, s), p, o, m)
# define _S1BOOSTPP_FOR_10_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(11, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_11, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(11, s), p, o, m)
# define _S1BOOSTPP_FOR_11_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(12, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_12, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(12, s), p, o, m)
# define _S1BOOSTPP_FOR_12_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(13, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_13, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(13, s), p, o, m)
# define _S1BOOSTPP_FOR_13_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(14, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_14, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(14, s), p, o, m)
# define _S1BOOSTPP_FOR_14_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(15, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_15, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(15, s), p, o, m)
# define _S1BOOSTPP_FOR_15_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(16, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_16, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(16, s), p, o, m)
# define _S1BOOSTPP_FOR_16_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(17, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_17, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(17, s), p, o, m)
# define _S1BOOSTPP_FOR_17_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(18, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_18, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(18, s), p, o, m)
# define _S1BOOSTPP_FOR_18_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(19, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_19, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(19, s), p, o, m)
# define _S1BOOSTPP_FOR_19_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(20, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_20, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(20, s), p, o, m)
# define _S1BOOSTPP_FOR_20_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(21, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_21, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(21, s), p, o, m)
# define _S1BOOSTPP_FOR_21_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(22, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_22, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(22, s), p, o, m)
# define _S1BOOSTPP_FOR_22_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(23, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_23, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(23, s), p, o, m)
# define _S1BOOSTPP_FOR_23_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(24, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_24, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(24, s), p, o, m)
# define _S1BOOSTPP_FOR_24_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(25, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_25, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(25, s), p, o, m)
# define _S1BOOSTPP_FOR_25_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(26, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_26, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(26, s), p, o, m)
# define _S1BOOSTPP_FOR_26_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(27, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_27, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(27, s), p, o, m)
# define _S1BOOSTPP_FOR_27_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(28, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_28, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(28, s), p, o, m)
# define _S1BOOSTPP_FOR_28_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(29, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_29, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(29, s), p, o, m)
# define _S1BOOSTPP_FOR_29_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(30, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_30, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(30, s), p, o, m)
# define _S1BOOSTPP_FOR_30_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(31, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_31, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(31, s), p, o, m)
# define _S1BOOSTPP_FOR_31_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(32, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_32, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(32, s), p, o, m)
# define _S1BOOSTPP_FOR_32_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(33, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_33, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(33, s), p, o, m)
# define _S1BOOSTPP_FOR_33_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(34, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_34, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(34, s), p, o, m)
# define _S1BOOSTPP_FOR_34_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(35, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_35, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(35, s), p, o, m)
# define _S1BOOSTPP_FOR_35_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(36, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_36, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(36, s), p, o, m)
# define _S1BOOSTPP_FOR_36_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(37, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_37, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(37, s), p, o, m)
# define _S1BOOSTPP_FOR_37_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(38, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_38, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(38, s), p, o, m)
# define _S1BOOSTPP_FOR_38_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(39, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_39, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(39, s), p, o, m)
# define _S1BOOSTPP_FOR_39_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(40, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_40, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(40, s), p, o, m)
# define _S1BOOSTPP_FOR_40_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(41, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_41, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(41, s), p, o, m)
# define _S1BOOSTPP_FOR_41_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(42, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_42, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(42, s), p, o, m)
# define _S1BOOSTPP_FOR_42_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(43, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_43, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(43, s), p, o, m)
# define _S1BOOSTPP_FOR_43_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(44, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_44, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(44, s), p, o, m)
# define _S1BOOSTPP_FOR_44_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(45, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_45, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(45, s), p, o, m)
# define _S1BOOSTPP_FOR_45_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(46, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_46, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(46, s), p, o, m)
# define _S1BOOSTPP_FOR_46_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(47, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_47, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(47, s), p, o, m)
# define _S1BOOSTPP_FOR_47_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(48, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_48, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(48, s), p, o, m)
# define _S1BOOSTPP_FOR_48_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(49, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_49, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(49, s), p, o, m)
# define _S1BOOSTPP_FOR_49_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(50, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_50, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(50, s), p, o, m)
# define _S1BOOSTPP_FOR_50_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(51, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_51, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(51, s), p, o, m)
# define _S1BOOSTPP_FOR_51_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(52, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_52, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(52, s), p, o, m)
# define _S1BOOSTPP_FOR_52_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(53, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_53, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(53, s), p, o, m)
# define _S1BOOSTPP_FOR_53_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(54, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_54, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(54, s), p, o, m)
# define _S1BOOSTPP_FOR_54_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(55, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_55, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(55, s), p, o, m)
# define _S1BOOSTPP_FOR_55_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(56, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_56, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(56, s), p, o, m)
# define _S1BOOSTPP_FOR_56_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(57, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_57, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(57, s), p, o, m)
# define _S1BOOSTPP_FOR_57_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(58, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_58, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(58, s), p, o, m)
# define _S1BOOSTPP_FOR_58_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(59, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_59, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(59, s), p, o, m)
# define _S1BOOSTPP_FOR_59_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(60, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_60, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(60, s), p, o, m)
# define _S1BOOSTPP_FOR_60_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(61, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_61, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(61, s), p, o, m)
# define _S1BOOSTPP_FOR_61_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(62, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_62, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(62, s), p, o, m)
# define _S1BOOSTPP_FOR_62_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(63, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_63, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(63, s), p, o, m)
# define _S1BOOSTPP_FOR_63_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(64, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_64, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(64, s), p, o, m)
# define _S1BOOSTPP_FOR_64_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(65, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_65, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(65, s), p, o, m)
# define _S1BOOSTPP_FOR_65_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(66, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_66, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(66, s), p, o, m)
# define _S1BOOSTPP_FOR_66_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(67, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_67, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(67, s), p, o, m)
# define _S1BOOSTPP_FOR_67_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(68, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_68, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(68, s), p, o, m)
# define _S1BOOSTPP_FOR_68_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(69, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_69, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(69, s), p, o, m)
# define _S1BOOSTPP_FOR_69_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(70, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_70, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(70, s), p, o, m)
# define _S1BOOSTPP_FOR_70_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(71, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_71, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(71, s), p, o, m)
# define _S1BOOSTPP_FOR_71_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(72, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_72, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(72, s), p, o, m)
# define _S1BOOSTPP_FOR_72_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(73, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_73, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(73, s), p, o, m)
# define _S1BOOSTPP_FOR_73_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(74, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_74, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(74, s), p, o, m)
# define _S1BOOSTPP_FOR_74_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(75, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_75, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(75, s), p, o, m)
# define _S1BOOSTPP_FOR_75_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(76, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_76, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(76, s), p, o, m)
# define _S1BOOSTPP_FOR_76_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(77, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_77, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(77, s), p, o, m)
# define _S1BOOSTPP_FOR_77_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(78, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_78, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(78, s), p, o, m)
# define _S1BOOSTPP_FOR_78_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(79, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_79, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(79, s), p, o, m)
# define _S1BOOSTPP_FOR_79_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(80, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_80, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(80, s), p, o, m)
# define _S1BOOSTPP_FOR_80_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(81, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_81, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(81, s), p, o, m)
# define _S1BOOSTPP_FOR_81_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(82, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_82, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(82, s), p, o, m)
# define _S1BOOSTPP_FOR_82_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(83, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_83, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(83, s), p, o, m)
# define _S1BOOSTPP_FOR_83_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(84, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_84, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(84, s), p, o, m)
# define _S1BOOSTPP_FOR_84_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(85, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_85, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(85, s), p, o, m)
# define _S1BOOSTPP_FOR_85_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(86, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_86, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(86, s), p, o, m)
# define _S1BOOSTPP_FOR_86_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(87, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_87, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(87, s), p, o, m)
# define _S1BOOSTPP_FOR_87_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(88, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_88, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(88, s), p, o, m)
# define _S1BOOSTPP_FOR_88_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(89, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_89, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(89, s), p, o, m)
# define _S1BOOSTPP_FOR_89_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(90, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_90, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(90, s), p, o, m)
# define _S1BOOSTPP_FOR_90_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(91, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_91, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(91, s), p, o, m)
# define _S1BOOSTPP_FOR_91_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(92, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_92, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(92, s), p, o, m)
# define _S1BOOSTPP_FOR_92_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(93, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_93, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(93, s), p, o, m)
# define _S1BOOSTPP_FOR_93_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(94, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_94, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(94, s), p, o, m)
# define _S1BOOSTPP_FOR_94_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(95, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_95, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(95, s), p, o, m)
# define _S1BOOSTPP_FOR_95_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(96, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_96, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(96, s), p, o, m)
# define _S1BOOSTPP_FOR_96_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(97, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_97, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(97, s), p, o, m)
# define _S1BOOSTPP_FOR_97_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(98, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_98, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(98, s), p, o, m)
# define _S1BOOSTPP_FOR_98_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(99, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_99, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(99, s), p, o, m)
# define _S1BOOSTPP_FOR_99_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(100, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_100, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(100, s), p, o, m)
# define _S1BOOSTPP_FOR_100_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(101, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_101, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(101, s), p, o, m)
# define _S1BOOSTPP_FOR_101_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(102, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_102, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(102, s), p, o, m)
# define _S1BOOSTPP_FOR_102_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(103, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_103, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(103, s), p, o, m)
# define _S1BOOSTPP_FOR_103_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(104, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_104, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(104, s), p, o, m)
# define _S1BOOSTPP_FOR_104_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(105, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_105, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(105, s), p, o, m)
# define _S1BOOSTPP_FOR_105_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(106, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_106, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(106, s), p, o, m)
# define _S1BOOSTPP_FOR_106_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(107, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_107, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(107, s), p, o, m)
# define _S1BOOSTPP_FOR_107_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(108, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_108, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(108, s), p, o, m)
# define _S1BOOSTPP_FOR_108_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(109, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_109, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(109, s), p, o, m)
# define _S1BOOSTPP_FOR_109_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(110, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_110, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(110, s), p, o, m)
# define _S1BOOSTPP_FOR_110_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(111, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_111, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(111, s), p, o, m)
# define _S1BOOSTPP_FOR_111_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(112, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_112, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(112, s), p, o, m)
# define _S1BOOSTPP_FOR_112_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(113, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_113, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(113, s), p, o, m)
# define _S1BOOSTPP_FOR_113_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(114, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_114, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(114, s), p, o, m)
# define _S1BOOSTPP_FOR_114_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(115, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_115, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(115, s), p, o, m)
# define _S1BOOSTPP_FOR_115_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(116, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_116, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(116, s), p, o, m)
# define _S1BOOSTPP_FOR_116_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(117, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_117, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(117, s), p, o, m)
# define _S1BOOSTPP_FOR_117_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(118, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_118, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(118, s), p, o, m)
# define _S1BOOSTPP_FOR_118_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(119, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_119, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(119, s), p, o, m)
# define _S1BOOSTPP_FOR_119_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(120, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_120, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(120, s), p, o, m)
# define _S1BOOSTPP_FOR_120_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(121, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_121, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(121, s), p, o, m)
# define _S1BOOSTPP_FOR_121_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(122, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_122, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(122, s), p, o, m)
# define _S1BOOSTPP_FOR_122_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(123, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_123, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(123, s), p, o, m)
# define _S1BOOSTPP_FOR_123_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(124, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_124, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(124, s), p, o, m)
# define _S1BOOSTPP_FOR_124_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(125, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_125, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(125, s), p, o, m)
# define _S1BOOSTPP_FOR_125_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(126, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_126, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(126, s), p, o, m)
# define _S1BOOSTPP_FOR_126_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(127, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_127, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(127, s), p, o, m)
# define _S1BOOSTPP_FOR_127_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(128, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_128, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(128, s), p, o, m)
# define _S1BOOSTPP_FOR_128_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(129, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_129, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(129, s), p, o, m)
# define _S1BOOSTPP_FOR_129_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(130, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_130, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(130, s), p, o, m)
# define _S1BOOSTPP_FOR_130_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(131, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_131, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(131, s), p, o, m)
# define _S1BOOSTPP_FOR_131_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(132, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_132, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(132, s), p, o, m)
# define _S1BOOSTPP_FOR_132_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(133, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_133, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(133, s), p, o, m)
# define _S1BOOSTPP_FOR_133_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(134, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_134, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(134, s), p, o, m)
# define _S1BOOSTPP_FOR_134_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(135, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_135, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(135, s), p, o, m)
# define _S1BOOSTPP_FOR_135_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(136, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_136, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(136, s), p, o, m)
# define _S1BOOSTPP_FOR_136_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(137, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_137, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(137, s), p, o, m)
# define _S1BOOSTPP_FOR_137_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(138, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_138, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(138, s), p, o, m)
# define _S1BOOSTPP_FOR_138_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(139, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_139, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(139, s), p, o, m)
# define _S1BOOSTPP_FOR_139_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(140, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_140, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(140, s), p, o, m)
# define _S1BOOSTPP_FOR_140_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(141, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_141, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(141, s), p, o, m)
# define _S1BOOSTPP_FOR_141_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(142, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_142, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(142, s), p, o, m)
# define _S1BOOSTPP_FOR_142_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(143, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_143, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(143, s), p, o, m)
# define _S1BOOSTPP_FOR_143_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(144, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_144, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(144, s), p, o, m)
# define _S1BOOSTPP_FOR_144_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(145, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_145, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(145, s), p, o, m)
# define _S1BOOSTPP_FOR_145_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(146, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_146, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(146, s), p, o, m)
# define _S1BOOSTPP_FOR_146_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(147, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_147, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(147, s), p, o, m)
# define _S1BOOSTPP_FOR_147_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(148, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_148, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(148, s), p, o, m)
# define _S1BOOSTPP_FOR_148_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(149, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_149, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(149, s), p, o, m)
# define _S1BOOSTPP_FOR_149_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(150, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_150, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(150, s), p, o, m)
# define _S1BOOSTPP_FOR_150_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(151, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_151, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(151, s), p, o, m)
# define _S1BOOSTPP_FOR_151_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(152, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_152, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(152, s), p, o, m)
# define _S1BOOSTPP_FOR_152_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(153, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_153, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(153, s), p, o, m)
# define _S1BOOSTPP_FOR_153_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(154, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_154, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(154, s), p, o, m)
# define _S1BOOSTPP_FOR_154_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(155, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_155, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(155, s), p, o, m)
# define _S1BOOSTPP_FOR_155_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(156, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_156, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(156, s), p, o, m)
# define _S1BOOSTPP_FOR_156_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(157, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_157, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(157, s), p, o, m)
# define _S1BOOSTPP_FOR_157_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(158, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_158, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(158, s), p, o, m)
# define _S1BOOSTPP_FOR_158_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(159, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_159, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(159, s), p, o, m)
# define _S1BOOSTPP_FOR_159_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(160, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_160, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(160, s), p, o, m)
# define _S1BOOSTPP_FOR_160_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(161, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_161, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(161, s), p, o, m)
# define _S1BOOSTPP_FOR_161_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(162, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_162, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(162, s), p, o, m)
# define _S1BOOSTPP_FOR_162_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(163, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_163, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(163, s), p, o, m)
# define _S1BOOSTPP_FOR_163_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(164, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_164, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(164, s), p, o, m)
# define _S1BOOSTPP_FOR_164_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(165, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_165, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(165, s), p, o, m)
# define _S1BOOSTPP_FOR_165_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(166, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_166, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(166, s), p, o, m)
# define _S1BOOSTPP_FOR_166_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(167, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_167, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(167, s), p, o, m)
# define _S1BOOSTPP_FOR_167_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(168, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_168, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(168, s), p, o, m)
# define _S1BOOSTPP_FOR_168_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(169, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_169, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(169, s), p, o, m)
# define _S1BOOSTPP_FOR_169_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(170, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_170, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(170, s), p, o, m)
# define _S1BOOSTPP_FOR_170_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(171, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_171, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(171, s), p, o, m)
# define _S1BOOSTPP_FOR_171_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(172, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_172, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(172, s), p, o, m)
# define _S1BOOSTPP_FOR_172_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(173, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_173, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(173, s), p, o, m)
# define _S1BOOSTPP_FOR_173_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(174, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_174, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(174, s), p, o, m)
# define _S1BOOSTPP_FOR_174_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(175, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_175, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(175, s), p, o, m)
# define _S1BOOSTPP_FOR_175_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(176, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_176, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(176, s), p, o, m)
# define _S1BOOSTPP_FOR_176_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(177, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_177, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(177, s), p, o, m)
# define _S1BOOSTPP_FOR_177_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(178, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_178, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(178, s), p, o, m)
# define _S1BOOSTPP_FOR_178_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(179, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_179, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(179, s), p, o, m)
# define _S1BOOSTPP_FOR_179_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(180, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_180, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(180, s), p, o, m)
# define _S1BOOSTPP_FOR_180_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(181, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_181, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(181, s), p, o, m)
# define _S1BOOSTPP_FOR_181_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(182, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_182, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(182, s), p, o, m)
# define _S1BOOSTPP_FOR_182_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(183, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_183, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(183, s), p, o, m)
# define _S1BOOSTPP_FOR_183_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(184, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_184, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(184, s), p, o, m)
# define _S1BOOSTPP_FOR_184_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(185, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_185, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(185, s), p, o, m)
# define _S1BOOSTPP_FOR_185_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(186, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_186, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(186, s), p, o, m)
# define _S1BOOSTPP_FOR_186_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(187, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_187, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(187, s), p, o, m)
# define _S1BOOSTPP_FOR_187_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(188, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_188, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(188, s), p, o, m)
# define _S1BOOSTPP_FOR_188_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(189, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_189, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(189, s), p, o, m)
# define _S1BOOSTPP_FOR_189_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(190, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_190, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(190, s), p, o, m)
# define _S1BOOSTPP_FOR_190_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(191, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_191, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(191, s), p, o, m)
# define _S1BOOSTPP_FOR_191_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(192, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_192, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(192, s), p, o, m)
# define _S1BOOSTPP_FOR_192_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(193, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_193, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(193, s), p, o, m)
# define _S1BOOSTPP_FOR_193_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(194, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_194, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(194, s), p, o, m)
# define _S1BOOSTPP_FOR_194_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(195, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_195, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(195, s), p, o, m)
# define _S1BOOSTPP_FOR_195_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(196, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_196, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(196, s), p, o, m)
# define _S1BOOSTPP_FOR_196_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(197, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_197, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(197, s), p, o, m)
# define _S1BOOSTPP_FOR_197_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(198, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_198, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(198, s), p, o, m)
# define _S1BOOSTPP_FOR_198_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(199, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_199, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(199, s), p, o, m)
# define _S1BOOSTPP_FOR_199_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(200, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_200, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(200, s), p, o, m)
# define _S1BOOSTPP_FOR_200_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(201, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_201, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(201, s), p, o, m)
# define _S1BOOSTPP_FOR_201_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(202, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_202, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(202, s), p, o, m)
# define _S1BOOSTPP_FOR_202_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(203, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_203, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(203, s), p, o, m)
# define _S1BOOSTPP_FOR_203_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(204, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_204, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(204, s), p, o, m)
# define _S1BOOSTPP_FOR_204_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(205, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_205, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(205, s), p, o, m)
# define _S1BOOSTPP_FOR_205_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(206, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_206, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(206, s), p, o, m)
# define _S1BOOSTPP_FOR_206_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(207, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_207, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(207, s), p, o, m)
# define _S1BOOSTPP_FOR_207_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(208, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_208, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(208, s), p, o, m)
# define _S1BOOSTPP_FOR_208_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(209, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_209, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(209, s), p, o, m)
# define _S1BOOSTPP_FOR_209_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(210, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_210, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(210, s), p, o, m)
# define _S1BOOSTPP_FOR_210_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(211, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_211, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(211, s), p, o, m)
# define _S1BOOSTPP_FOR_211_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(212, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_212, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(212, s), p, o, m)
# define _S1BOOSTPP_FOR_212_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(213, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_213, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(213, s), p, o, m)
# define _S1BOOSTPP_FOR_213_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(214, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_214, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(214, s), p, o, m)
# define _S1BOOSTPP_FOR_214_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(215, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_215, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(215, s), p, o, m)
# define _S1BOOSTPP_FOR_215_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(216, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_216, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(216, s), p, o, m)
# define _S1BOOSTPP_FOR_216_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(217, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_217, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(217, s), p, o, m)
# define _S1BOOSTPP_FOR_217_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(218, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_218, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(218, s), p, o, m)
# define _S1BOOSTPP_FOR_218_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(219, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_219, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(219, s), p, o, m)
# define _S1BOOSTPP_FOR_219_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(220, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_220, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(220, s), p, o, m)
# define _S1BOOSTPP_FOR_220_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(221, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_221, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(221, s), p, o, m)
# define _S1BOOSTPP_FOR_221_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(222, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_222, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(222, s), p, o, m)
# define _S1BOOSTPP_FOR_222_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(223, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_223, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(223, s), p, o, m)
# define _S1BOOSTPP_FOR_223_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(224, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_224, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(224, s), p, o, m)
# define _S1BOOSTPP_FOR_224_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(225, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_225, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(225, s), p, o, m)
# define _S1BOOSTPP_FOR_225_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(226, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_226, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(226, s), p, o, m)
# define _S1BOOSTPP_FOR_226_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(227, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_227, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(227, s), p, o, m)
# define _S1BOOSTPP_FOR_227_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(228, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_228, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(228, s), p, o, m)
# define _S1BOOSTPP_FOR_228_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(229, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_229, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(229, s), p, o, m)
# define _S1BOOSTPP_FOR_229_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(230, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_230, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(230, s), p, o, m)
# define _S1BOOSTPP_FOR_230_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(231, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_231, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(231, s), p, o, m)
# define _S1BOOSTPP_FOR_231_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(232, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_232, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(232, s), p, o, m)
# define _S1BOOSTPP_FOR_232_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(233, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_233, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(233, s), p, o, m)
# define _S1BOOSTPP_FOR_233_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(234, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_234, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(234, s), p, o, m)
# define _S1BOOSTPP_FOR_234_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(235, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_235, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(235, s), p, o, m)
# define _S1BOOSTPP_FOR_235_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(236, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_236, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(236, s), p, o, m)
# define _S1BOOSTPP_FOR_236_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(237, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_237, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(237, s), p, o, m)
# define _S1BOOSTPP_FOR_237_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(238, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_238, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(238, s), p, o, m)
# define _S1BOOSTPP_FOR_238_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(239, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_239, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(239, s), p, o, m)
# define _S1BOOSTPP_FOR_239_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(240, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_240, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(240, s), p, o, m)
# define _S1BOOSTPP_FOR_240_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(241, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_241, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(241, s), p, o, m)
# define _S1BOOSTPP_FOR_241_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(242, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_242, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(242, s), p, o, m)
# define _S1BOOSTPP_FOR_242_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(243, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_243, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(243, s), p, o, m)
# define _S1BOOSTPP_FOR_243_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(244, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_244, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(244, s), p, o, m)
# define _S1BOOSTPP_FOR_244_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(245, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_245, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(245, s), p, o, m)
# define _S1BOOSTPP_FOR_245_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(246, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_246, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(246, s), p, o, m)
# define _S1BOOSTPP_FOR_246_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(247, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_247, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(247, s), p, o, m)
# define _S1BOOSTPP_FOR_247_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(248, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_248, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(248, s), p, o, m)
# define _S1BOOSTPP_FOR_248_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(249, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_249, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(249, s), p, o, m)
# define _S1BOOSTPP_FOR_249_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(250, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_250, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(250, s), p, o, m)
# define _S1BOOSTPP_FOR_250_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(251, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_251, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(251, s), p, o, m)
# define _S1BOOSTPP_FOR_251_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(252, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_252, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(252, s), p, o, m)
# define _S1BOOSTPP_FOR_252_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(253, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_253, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(253, s), p, o, m)
# define _S1BOOSTPP_FOR_253_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(254, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_254, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(254, s), p, o, m)
# define _S1BOOSTPP_FOR_254_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(255, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_255, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(255, s), p, o, m)
# define _S1BOOSTPP_FOR_255_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(256, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_256, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(256, s), p, o, m)
# define _S1BOOSTPP_FOR_256_C(c, s, p, o, m) _S1BOOSTPP_IIF(c, m, _S1BOOSTPP_TUPLE_EAT_2)(257, s) _S1BOOSTPP_IIF(c, _S1BOOSTPP_FOR_257, _S1BOOSTPP_TUPLE_EAT_4)(_S1BOOSTPP_EXPR_IIF(c, o)(257, s), p, o, m)
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_REPETITION_FOR_HPP
# define _S1BOOSTPP_REPETITION_FOR_HPP
#
#
# /* _S1BOOSTPP_FOR */
#
# if 0
#    define _S1BOOSTPP_FOR(state, pred, op, macro)
# endif
#
# define _S1BOOSTPP_FOR _S1BOOSTPP_CAT(_S1BOOSTPP_FOR_, _S1BOOSTPP_AUTO_REC(_S1BOOSTPP_FOR_P, 256))
#
# define _S1BOOSTPP_FOR_P(n) _S1BOOSTPP_CAT(_S1BOOSTPP_FOR_CHECK_, _S1BOOSTPP_FOR_ ## n(1, _S1BOOSTPP_FOR_SR_P, _S1BOOSTPP_FOR_SR_O, _S1BOOSTPP_FOR_SR_M))
#
# define _S1BOOSTPP_FOR_SR_P(r, s) s
# define _S1BOOSTPP_FOR_SR_O(r, s) 0
# define _S1BOOSTPP_FOR_SR_M(r, s) _S1BOOSTPP_NIL
#
# if _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
# elif _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC()
# elif _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_DMC()
# else
# endif
#
# define _S1BOOSTPP_FOR_257(s, p, o, m) _S1BOOSTPP_ERROR(0x0002)
#
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_NIL 1
#
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_1(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_2(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_3(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_4(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_5(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_6(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_7(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_8(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_9(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_10(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_11(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_12(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_13(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_14(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_15(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_16(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_17(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_18(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_19(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_20(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_21(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_22(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_23(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_24(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_25(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_26(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_27(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_28(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_29(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_30(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_31(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_32(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_33(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_34(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_35(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_36(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_37(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_38(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_39(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_40(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_41(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_42(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_43(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_44(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_45(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_46(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_47(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_48(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_49(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_50(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_51(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_52(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_53(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_54(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_55(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_56(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_57(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_58(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_59(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_60(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_61(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_62(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_63(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_64(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_65(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_66(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_67(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_68(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_69(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_70(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_71(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_72(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_73(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_74(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_75(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_76(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_77(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_78(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_79(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_80(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_81(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_82(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_83(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_84(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_85(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_86(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_87(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_88(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_89(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_90(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_91(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_92(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_93(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_94(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_95(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_96(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_97(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_98(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_99(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_100(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_101(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_102(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_103(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_104(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_105(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_106(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_107(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_108(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_109(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_110(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_111(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_112(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_113(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_114(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_115(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_116(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_117(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_118(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_119(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_120(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_121(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_122(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_123(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_124(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_125(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_126(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_127(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_128(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_129(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_130(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_131(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_132(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_133(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_134(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_135(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_136(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_137(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_138(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_139(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_140(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_141(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_142(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_143(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_144(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_145(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_146(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_147(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_148(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_149(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_150(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_151(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_152(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_153(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_154(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_155(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_156(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_157(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_158(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_159(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_160(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_161(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_162(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_163(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_164(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_165(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_166(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_167(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_168(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_169(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_170(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_171(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_172(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_173(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_174(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_175(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_176(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_177(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_178(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_179(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_180(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_181(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_182(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_183(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_184(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_185(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_186(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_187(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_188(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_189(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_190(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_191(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_192(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_193(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_194(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_195(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_196(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_197(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_198(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_199(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_200(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_201(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_202(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_203(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_204(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_205(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_206(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_207(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_208(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_209(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_210(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_211(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_212(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_213(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_214(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_215(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_216(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_217(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_218(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_219(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_220(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_221(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_222(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_223(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_224(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_225(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_226(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_227(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_228(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_229(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_230(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_231(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_232(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_233(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_234(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_235(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_236(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_237(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_238(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_239(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_240(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_241(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_242(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_243(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_244(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_245(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_246(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_247(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_248(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_249(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_250(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_251(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_252(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_253(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_254(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_255(s, p, o, m) 0
# define _S1BOOSTPP_FOR_CHECK__S1BOOSTPP_FOR_256(s, p, o, m) 0
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_VARIADIC_SIZE_HPP
# define _S1BOOSTPP_VARIADIC_SIZE_HPP
#
#
# /* _S1BOOSTPP_VARIADIC_SIZE */
#
# if _S1BOOSTPP_VARIADICS
#    if _S1BOOSTPP_VARIADICS_MSVC
#        define _S1BOOSTPP_VARIADIC_SIZE(...) _S1BOOSTPP_CAT(_S1BOOSTPP_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,),)
#    else
#        define _S1BOOSTPP_VARIADIC_SIZE(...) _S1BOOSTPP_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)
#    endif
#    define _S1BOOSTPP_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size
# endif
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_FACILITIES_OVERLOAD_HPP
# define _S1BOOSTPP_FACILITIES_OVERLOAD_HPP
#
#
# /* _S1BOOSTPP_OVERLOAD */
#
# if _S1BOOSTPP_VARIADICS
#    define _S1BOOSTPP_OVERLOAD(prefix, ...) _S1BOOSTPP_CAT(prefix, _S1BOOSTPP_VARIADIC_SIZE(__VA_ARGS__))
# endif
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002-2011.                             *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_TUPLE_REM_HPP
# define _S1BOOSTPP_TUPLE_REM_HPP
#
#
# /* _S1BOOSTPP_REM */
#
# if _S1BOOSTPP_VARIADICS
#    define _S1BOOSTPP_REM(...) __VA_ARGS__
# else
#    define _S1BOOSTPP_REM(x) x
# endif
#
# /* _S1BOOSTPP_TUPLE_REM */
#
# if _S1BOOSTPP_VARIADICS
#    define _S1BOOSTPP_TUPLE_REM(size) _S1BOOSTPP_REM
# else
#    if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#        define _S1BOOSTPP_TUPLE_REM(size) _S1BOOSTPP_TUPLE_REM_I(size)
#    else
#        define _S1BOOSTPP_TUPLE_REM(size) _S1BOOSTPP_TUPLE_REM_OO((size))
#        define _S1BOOSTPP_TUPLE_REM_OO(par) _S1BOOSTPP_TUPLE_REM_I ## par
#    endif
#    define _S1BOOSTPP_TUPLE_REM_I(size) _S1BOOSTPP_TUPLE_REM_ ## size
# endif
# define _S1BOOSTPP_TUPLE_REM_1(e0) e0
# define _S1BOOSTPP_TUPLE_REM_2(e0, e1) e0, e1
# define _S1BOOSTPP_TUPLE_REM_3(e0, e1, e2) e0, e1, e2
# define _S1BOOSTPP_TUPLE_REM_4(e0, e1, e2, e3) e0, e1, e2, e3
# define _S1BOOSTPP_TUPLE_REM_5(e0, e1, e2, e3, e4) e0, e1, e2, e3, e4
# define _S1BOOSTPP_TUPLE_REM_6(e0, e1, e2, e3, e4, e5) e0, e1, e2, e3, e4, e5
# define _S1BOOSTPP_TUPLE_REM_7(e0, e1, e2, e3, e4, e5, e6) e0, e1, e2, e3, e4, e5, e6
# define _S1BOOSTPP_TUPLE_REM_8(e0, e1, e2, e3, e4, e5, e6, e7) e0, e1, e2, e3, e4, e5, e6, e7
# define _S1BOOSTPP_TUPLE_REM_9(e0, e1, e2, e3, e4, e5, e6, e7, e8) e0, e1, e2, e3, e4, e5, e6, e7, e8
# define _S1BOOSTPP_TUPLE_REM_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9
# define _S1BOOSTPP_TUPLE_REM_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10
# define _S1BOOSTPP_TUPLE_REM_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11
# define _S1BOOSTPP_TUPLE_REM_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12
# define _S1BOOSTPP_TUPLE_REM_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13
# define _S1BOOSTPP_TUPLE_REM_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14
# define _S1BOOSTPP_TUPLE_REM_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15
# define _S1BOOSTPP_TUPLE_REM_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16
# define _S1BOOSTPP_TUPLE_REM_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17
# define _S1BOOSTPP_TUPLE_REM_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18
# define _S1BOOSTPP_TUPLE_REM_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19
# define _S1BOOSTPP_TUPLE_REM_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20
# define _S1BOOSTPP_TUPLE_REM_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21
# define _S1BOOSTPP_TUPLE_REM_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22
# define _S1BOOSTPP_TUPLE_REM_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23
# define _S1BOOSTPP_TUPLE_REM_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24
# define _S1BOOSTPP_TUPLE_REM_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25
# define _S1BOOSTPP_TUPLE_REM_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26
# define _S1BOOSTPP_TUPLE_REM_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27
# define _S1BOOSTPP_TUPLE_REM_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28
# define _S1BOOSTPP_TUPLE_REM_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29
# define _S1BOOSTPP_TUPLE_REM_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30
# define _S1BOOSTPP_TUPLE_REM_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31
# define _S1BOOSTPP_TUPLE_REM_33(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32
# define _S1BOOSTPP_TUPLE_REM_34(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33
# define _S1BOOSTPP_TUPLE_REM_35(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34
# define _S1BOOSTPP_TUPLE_REM_36(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35
# define _S1BOOSTPP_TUPLE_REM_37(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36
# define _S1BOOSTPP_TUPLE_REM_38(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37
# define _S1BOOSTPP_TUPLE_REM_39(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38
# define _S1BOOSTPP_TUPLE_REM_40(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39
# define _S1BOOSTPP_TUPLE_REM_41(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40
# define _S1BOOSTPP_TUPLE_REM_42(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41
# define _S1BOOSTPP_TUPLE_REM_43(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42
# define _S1BOOSTPP_TUPLE_REM_44(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43
# define _S1BOOSTPP_TUPLE_REM_45(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44
# define _S1BOOSTPP_TUPLE_REM_46(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45
# define _S1BOOSTPP_TUPLE_REM_47(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46
# define _S1BOOSTPP_TUPLE_REM_48(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47
# define _S1BOOSTPP_TUPLE_REM_49(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48
# define _S1BOOSTPP_TUPLE_REM_50(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49
# define _S1BOOSTPP_TUPLE_REM_51(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50
# define _S1BOOSTPP_TUPLE_REM_52(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51
# define _S1BOOSTPP_TUPLE_REM_53(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52
# define _S1BOOSTPP_TUPLE_REM_54(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53
# define _S1BOOSTPP_TUPLE_REM_55(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54
# define _S1BOOSTPP_TUPLE_REM_56(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55
# define _S1BOOSTPP_TUPLE_REM_57(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56
# define _S1BOOSTPP_TUPLE_REM_58(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57
# define _S1BOOSTPP_TUPLE_REM_59(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58
# define _S1BOOSTPP_TUPLE_REM_60(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59
# define _S1BOOSTPP_TUPLE_REM_61(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60
# define _S1BOOSTPP_TUPLE_REM_62(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61
# define _S1BOOSTPP_TUPLE_REM_63(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62
# define _S1BOOSTPP_TUPLE_REM_64(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63
#
# /* _S1BOOSTPP_TUPLE_REM_CTOR */
#
# if _S1BOOSTPP_VARIADICS
#    if _S1BOOSTPP_VARIADICS_MSVC
#        define _S1BOOSTPP_TUPLE_REM_CTOR(...) _S1BOOSTPP_TUPLE_REM_CTOR_I(_S1BOOSTPP_OVERLOAD(_S1BOOSTPP_TUPLE_REM_CTOR_O_, __VA_ARGS__), (__VA_ARGS__))
#        define _S1BOOSTPP_TUPLE_REM_CTOR_I(m, args) _S1BOOSTPP_TUPLE_REM_CTOR_II(m, args)
#        define _S1BOOSTPP_TUPLE_REM_CTOR_II(m, args) _S1BOOSTPP_CAT(m ## args,)
#    else
#        define _S1BOOSTPP_TUPLE_REM_CTOR(...) _S1BOOSTPP_OVERLOAD(_S1BOOSTPP_TUPLE_REM_CTOR_O_, __VA_ARGS__)(__VA_ARGS__)
#    endif
#    define _S1BOOSTPP_TUPLE_REM_CTOR_O_1(tuple) _S1BOOSTPP_REM tuple
#    define _S1BOOSTPP_TUPLE_REM_CTOR_O_2(size, tuple) _S1BOOSTPP_TUPLE_REM_CTOR_O_1(tuple)
# else
#    if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#        define _S1BOOSTPP_TUPLE_REM_CTOR(size, tuple) _S1BOOSTPP_TUPLE_REM_CTOR_I(_S1BOOSTPP_TUPLE_REM(size), tuple)
#    else
#        define _S1BOOSTPP_TUPLE_REM_CTOR(size, tuple) _S1BOOSTPP_TUPLE_REM_CTOR_D(size, tuple)
#        define _S1BOOSTPP_TUPLE_REM_CTOR_D(size, tuple) _S1BOOSTPP_TUPLE_REM_CTOR_I(_S1BOOSTPP_TUPLE_REM(size), tuple)
#    endif
#    if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#        define _S1BOOSTPP_TUPLE_REM_CTOR_I(ext, tuple) ext tuple
#    else
#        define _S1BOOSTPP_TUPLE_REM_CTOR_I(ext, tuple) _S1BOOSTPP_TUPLE_REM_CTOR_OO((ext, tuple))
#        define _S1BOOSTPP_TUPLE_REM_CTOR_OO(par) _S1BOOSTPP_TUPLE_REM_CTOR_II ## par
#        define _S1BOOSTPP_TUPLE_REM_CTOR_II(ext, tuple) ext ## tuple
#    endif
# endif
#
# endif
# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_VARIADIC_ELEM_HPP
# define _S1BOOSTPP_VARIADIC_ELEM_HPP
#
#
# /* _S1BOOSTPP_VARIADIC_ELEM */
#
# if _S1BOOSTPP_VARIADICS
#    if _S1BOOSTPP_VARIADICS_MSVC
#        define _S1BOOSTPP_VARIADIC_ELEM(n, ...) _S1BOOSTPP_VARIADIC_ELEM_I(n,__VA_ARGS__)
#        define _S1BOOSTPP_VARIADIC_ELEM_I(n, ...) _S1BOOSTPP_CAT(_S1BOOSTPP_CAT(_S1BOOSTPP_VARIADIC_ELEM_, n)(__VA_ARGS__,),)
#    else
#        define _S1BOOSTPP_VARIADIC_ELEM(n, ...) _S1BOOSTPP_CAT(_S1BOOSTPP_VARIADIC_ELEM_, n)(__VA_ARGS__,)
#    endif
#    define _S1BOOSTPP_VARIADIC_ELEM_0(e0, ...) e0
#    define _S1BOOSTPP_VARIADIC_ELEM_1(e0, e1, ...) e1
#    define _S1BOOSTPP_VARIADIC_ELEM_2(e0, e1, e2, ...) e2
#    define _S1BOOSTPP_VARIADIC_ELEM_3(e0, e1, e2, e3, ...) e3
#    define _S1BOOSTPP_VARIADIC_ELEM_4(e0, e1, e2, e3, e4, ...) e4
#    define _S1BOOSTPP_VARIADIC_ELEM_5(e0, e1, e2, e3, e4, e5, ...) e5
#    define _S1BOOSTPP_VARIADIC_ELEM_6(e0, e1, e2, e3, e4, e5, e6, ...) e6
#    define _S1BOOSTPP_VARIADIC_ELEM_7(e0, e1, e2, e3, e4, e5, e6, e7, ...) e7
#    define _S1BOOSTPP_VARIADIC_ELEM_8(e0, e1, e2, e3, e4, e5, e6, e7, e8, ...) e8
#    define _S1BOOSTPP_VARIADIC_ELEM_9(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ...) e9
#    define _S1BOOSTPP_VARIADIC_ELEM_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, ...) e10
#    define _S1BOOSTPP_VARIADIC_ELEM_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, ...) e11
#    define _S1BOOSTPP_VARIADIC_ELEM_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, ...) e12
#    define _S1BOOSTPP_VARIADIC_ELEM_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, ...) e13
#    define _S1BOOSTPP_VARIADIC_ELEM_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, ...) e14
#    define _S1BOOSTPP_VARIADIC_ELEM_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, ...) e15
#    define _S1BOOSTPP_VARIADIC_ELEM_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, ...) e16
#    define _S1BOOSTPP_VARIADIC_ELEM_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, ...) e17
#    define _S1BOOSTPP_VARIADIC_ELEM_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, ...) e18
#    define _S1BOOSTPP_VARIADIC_ELEM_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, ...) e19
#    define _S1BOOSTPP_VARIADIC_ELEM_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, ...) e20
#    define _S1BOOSTPP_VARIADIC_ELEM_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, ...) e21
#    define _S1BOOSTPP_VARIADIC_ELEM_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, ...) e22
#    define _S1BOOSTPP_VARIADIC_ELEM_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, ...) e23
#    define _S1BOOSTPP_VARIADIC_ELEM_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, ...) e24
#    define _S1BOOSTPP_VARIADIC_ELEM_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, ...) e25
#    define _S1BOOSTPP_VARIADIC_ELEM_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, ...) e26
#    define _S1BOOSTPP_VARIADIC_ELEM_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, ...) e27
#    define _S1BOOSTPP_VARIADIC_ELEM_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, ...) e28
#    define _S1BOOSTPP_VARIADIC_ELEM_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, ...) e29
#    define _S1BOOSTPP_VARIADIC_ELEM_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, ...) e30
#    define _S1BOOSTPP_VARIADIC_ELEM_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, ...) e31
#    define _S1BOOSTPP_VARIADIC_ELEM_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, ...) e32
#    define _S1BOOSTPP_VARIADIC_ELEM_33(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, ...) e33
#    define _S1BOOSTPP_VARIADIC_ELEM_34(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, ...) e34
#    define _S1BOOSTPP_VARIADIC_ELEM_35(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, ...) e35
#    define _S1BOOSTPP_VARIADIC_ELEM_36(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, ...) e36
#    define _S1BOOSTPP_VARIADIC_ELEM_37(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, ...) e37
#    define _S1BOOSTPP_VARIADIC_ELEM_38(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, ...) e38
#    define _S1BOOSTPP_VARIADIC_ELEM_39(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, ...) e39
#    define _S1BOOSTPP_VARIADIC_ELEM_40(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, ...) e40
#    define _S1BOOSTPP_VARIADIC_ELEM_41(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, ...) e41
#    define _S1BOOSTPP_VARIADIC_ELEM_42(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, ...) e42
#    define _S1BOOSTPP_VARIADIC_ELEM_43(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, ...) e43
#    define _S1BOOSTPP_VARIADIC_ELEM_44(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, ...) e44
#    define _S1BOOSTPP_VARIADIC_ELEM_45(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, ...) e45
#    define _S1BOOSTPP_VARIADIC_ELEM_46(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, ...) e46
#    define _S1BOOSTPP_VARIADIC_ELEM_47(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, ...) e47
#    define _S1BOOSTPP_VARIADIC_ELEM_48(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, ...) e48
#    define _S1BOOSTPP_VARIADIC_ELEM_49(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, ...) e49
#    define _S1BOOSTPP_VARIADIC_ELEM_50(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, ...) e50
#    define _S1BOOSTPP_VARIADIC_ELEM_51(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, ...) e51
#    define _S1BOOSTPP_VARIADIC_ELEM_52(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, ...) e52
#    define _S1BOOSTPP_VARIADIC_ELEM_53(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, ...) e53
#    define _S1BOOSTPP_VARIADIC_ELEM_54(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, ...) e54
#    define _S1BOOSTPP_VARIADIC_ELEM_55(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, ...) e55
#    define _S1BOOSTPP_VARIADIC_ELEM_56(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, ...) e56
#    define _S1BOOSTPP_VARIADIC_ELEM_57(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, ...) e57
#    define _S1BOOSTPP_VARIADIC_ELEM_58(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, ...) e58
#    define _S1BOOSTPP_VARIADIC_ELEM_59(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, ...) e59
#    define _S1BOOSTPP_VARIADIC_ELEM_60(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, ...) e60
#    define _S1BOOSTPP_VARIADIC_ELEM_61(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, ...) e61
#    define _S1BOOSTPP_VARIADIC_ELEM_62(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, ...) e62
#    define _S1BOOSTPP_VARIADIC_ELEM_63(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, ...) e63
# endif
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002-2011) */
# /* Revised by Edward Diener (2011) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_TUPLE_ELEM_HPP
# define _S1BOOSTPP_TUPLE_ELEM_HPP
#
#
# if _S1BOOSTPP_VARIADICS
#    if _S1BOOSTPP_VARIADICS_MSVC
#        define _S1BOOSTPP_TUPLE_ELEM(...) _S1BOOSTPP_TUPLE_ELEM_I(_S1BOOSTPP_OVERLOAD(_S1BOOSTPP_TUPLE_ELEM_O_, __VA_ARGS__), (__VA_ARGS__))
#        define _S1BOOSTPP_TUPLE_ELEM_I(m, args) _S1BOOSTPP_TUPLE_ELEM_II(m, args)
#        define _S1BOOSTPP_TUPLE_ELEM_II(m, args) _S1BOOSTPP_CAT(m ## args,)
#    else
#        define _S1BOOSTPP_TUPLE_ELEM(...) _S1BOOSTPP_OVERLOAD(_S1BOOSTPP_TUPLE_ELEM_O_, __VA_ARGS__)(__VA_ARGS__)
#    endif
#    define _S1BOOSTPP_TUPLE_ELEM_O_2(n, tuple) _S1BOOSTPP_VARIADIC_ELEM(n, _S1BOOSTPP_REM tuple)
#    define _S1BOOSTPP_TUPLE_ELEM_O_3(size, n, tuple) _S1BOOSTPP_TUPLE_ELEM_O_2(n, tuple)
# else
#    if _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC()
#        define _S1BOOSTPP_TUPLE_ELEM(size, n, tuple) _S1BOOSTPP_TUPLE_ELEM_I(_S1BOOSTPP_CAT(_S1BOOSTPP_TUPLE_ELEM_, n), _S1BOOSTPP_CAT(_S1BOOSTPP_CAT(_S1BOOSTPP_TUPLE_ELEM_E_, size), tuple))
#        define _S1BOOSTPP_TUPLE_ELEM_I(m, args) _S1BOOSTPP_TUPLE_ELEM_II(m, args)
#        define _S1BOOSTPP_TUPLE_ELEM_II(m, args) _S1BOOSTPP_CAT(m ## args,)
#    elif _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#        define _S1BOOSTPP_TUPLE_ELEM(size, n, tuple) _S1BOOSTPP_TUPLE_ELEM_I_OO((size, n, tuple))
#        define _S1BOOSTPP_TUPLE_ELEM_I_OO(par) _S1BOOSTPP_TUPLE_ELEM_I ## par
#        define _S1BOOSTPP_TUPLE_ELEM_I(size, n, tuple) _S1BOOSTPP_TUPLE_ELEM_II((n, _S1BOOSTPP_TUPLE_ELEM_E_ ## size ## tuple))
#        define _S1BOOSTPP_TUPLE_ELEM_II(par) _S1BOOSTPP_TUPLE_ELEM_III_OO(par)
#        define _S1BOOSTPP_TUPLE_ELEM_III_OO(par) _S1BOOSTPP_TUPLE_ELEM_III ## par
#        define _S1BOOSTPP_TUPLE_ELEM_III(n, etuple) _S1BOOSTPP_TUPLE_ELEM_ ## n ## etuple
#    else
#        define _S1BOOSTPP_TUPLE_ELEM(size, n, tuple) _S1BOOSTPP_TUPLE_ELEM_I(_S1BOOSTPP_CAT(_S1BOOSTPP_TUPLE_ELEM_, n) _S1BOOSTPP_CAT(_S1BOOSTPP_TUPLE_ELEM_E_, size) tuple)
#        define _S1BOOSTPP_TUPLE_ELEM_I(x) x
#    endif
#    define _S1BOOSTPP_TUPLE_ELEM_E_1(e0) (e0, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_2(e0, e1) (e0, e1, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_3(e0, e1, e2) (e0, e1, e2, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_4(e0, e1, e2, e3) (e0, e1, e2, e3, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_5(e0, e1, e2, e3, e4) (e0, e1, e2, e3, e4, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_6(e0, e1, e2, e3, e4, e5) (e0, e1, e2, e3, e4, e5, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_7(e0, e1, e2, e3, e4, e5, e6) (e0, e1, e2, e3, e4, e5, e6, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_8(e0, e1, e2, e3, e4, e5, e6, e7) (e0, e1, e2, e3, e4, e5, e6, e7, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_9(e0, e1, e2, e3, e4, e5, e6, e7, e8) (e0, e1, e2, e3, e4, e5, e6, e7, e8, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_33(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_34(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_35(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_36(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_37(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_38(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_39(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_40(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_41(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_42(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_43(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_44(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_45(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_46(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_47(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_48(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_49(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_50(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_51(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_52(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_53(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_54(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_55(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, ?, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_56(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, ?, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_57(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, ?, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_58(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, ?, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_59(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, ?, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_60(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, ?, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_61(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, ?, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_62(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, ?, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_63(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62) (e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, ?)
#    define _S1BOOSTPP_TUPLE_ELEM_E_64
#    define _S1BOOSTPP_TUPLE_ELEM_0(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e0
#    define _S1BOOSTPP_TUPLE_ELEM_1(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e1
#    define _S1BOOSTPP_TUPLE_ELEM_2(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e2
#    define _S1BOOSTPP_TUPLE_ELEM_3(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e3
#    define _S1BOOSTPP_TUPLE_ELEM_4(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e4
#    define _S1BOOSTPP_TUPLE_ELEM_5(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e5
#    define _S1BOOSTPP_TUPLE_ELEM_6(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e6
#    define _S1BOOSTPP_TUPLE_ELEM_7(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e7
#    define _S1BOOSTPP_TUPLE_ELEM_8(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e8
#    define _S1BOOSTPP_TUPLE_ELEM_9(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e9
#    define _S1BOOSTPP_TUPLE_ELEM_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e10
#    define _S1BOOSTPP_TUPLE_ELEM_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e11
#    define _S1BOOSTPP_TUPLE_ELEM_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e12
#    define _S1BOOSTPP_TUPLE_ELEM_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e13
#    define _S1BOOSTPP_TUPLE_ELEM_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e14
#    define _S1BOOSTPP_TUPLE_ELEM_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e15
#    define _S1BOOSTPP_TUPLE_ELEM_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e16
#    define _S1BOOSTPP_TUPLE_ELEM_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e17
#    define _S1BOOSTPP_TUPLE_ELEM_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e18
#    define _S1BOOSTPP_TUPLE_ELEM_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e19
#    define _S1BOOSTPP_TUPLE_ELEM_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e20
#    define _S1BOOSTPP_TUPLE_ELEM_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e21
#    define _S1BOOSTPP_TUPLE_ELEM_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e22
#    define _S1BOOSTPP_TUPLE_ELEM_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e23
#    define _S1BOOSTPP_TUPLE_ELEM_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e24
#    define _S1BOOSTPP_TUPLE_ELEM_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e25
#    define _S1BOOSTPP_TUPLE_ELEM_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e26
#    define _S1BOOSTPP_TUPLE_ELEM_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e27
#    define _S1BOOSTPP_TUPLE_ELEM_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e28
#    define _S1BOOSTPP_TUPLE_ELEM_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e29
#    define _S1BOOSTPP_TUPLE_ELEM_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e30
#    define _S1BOOSTPP_TUPLE_ELEM_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e31
#    define _S1BOOSTPP_TUPLE_ELEM_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e32
#    define _S1BOOSTPP_TUPLE_ELEM_33(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e33
#    define _S1BOOSTPP_TUPLE_ELEM_34(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e34
#    define _S1BOOSTPP_TUPLE_ELEM_35(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e35
#    define _S1BOOSTPP_TUPLE_ELEM_36(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e36
#    define _S1BOOSTPP_TUPLE_ELEM_37(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e37
#    define _S1BOOSTPP_TUPLE_ELEM_38(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e38
#    define _S1BOOSTPP_TUPLE_ELEM_39(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e39
#    define _S1BOOSTPP_TUPLE_ELEM_40(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e40
#    define _S1BOOSTPP_TUPLE_ELEM_41(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e41
#    define _S1BOOSTPP_TUPLE_ELEM_42(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e42
#    define _S1BOOSTPP_TUPLE_ELEM_43(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e43
#    define _S1BOOSTPP_TUPLE_ELEM_44(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e44
#    define _S1BOOSTPP_TUPLE_ELEM_45(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e45
#    define _S1BOOSTPP_TUPLE_ELEM_46(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e46
#    define _S1BOOSTPP_TUPLE_ELEM_47(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e47
#    define _S1BOOSTPP_TUPLE_ELEM_48(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e48
#    define _S1BOOSTPP_TUPLE_ELEM_49(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e49
#    define _S1BOOSTPP_TUPLE_ELEM_50(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e50
#    define _S1BOOSTPP_TUPLE_ELEM_51(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e51
#    define _S1BOOSTPP_TUPLE_ELEM_52(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e52
#    define _S1BOOSTPP_TUPLE_ELEM_53(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e53
#    define _S1BOOSTPP_TUPLE_ELEM_54(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e54
#    define _S1BOOSTPP_TUPLE_ELEM_55(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e55
#    define _S1BOOSTPP_TUPLE_ELEM_56(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e56
#    define _S1BOOSTPP_TUPLE_ELEM_57(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e57
#    define _S1BOOSTPP_TUPLE_ELEM_58(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e58
#    define _S1BOOSTPP_TUPLE_ELEM_59(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e59
#    define _S1BOOSTPP_TUPLE_ELEM_60(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e60
#    define _S1BOOSTPP_TUPLE_ELEM_61(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e61
#    define _S1BOOSTPP_TUPLE_ELEM_62(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e62
#    define _S1BOOSTPP_TUPLE_ELEM_63(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63) e63
# endif
#
# /* directly used elsewhere in Boost... */
#
# define _S1BOOSTPP_TUPLE_ELEM_1_0(a) a
#
# define _S1BOOSTPP_TUPLE_ELEM_2_0(a, b) a
# define _S1BOOSTPP_TUPLE_ELEM_2_1(a, b) b
#
# define _S1BOOSTPP_TUPLE_ELEM_3_0(a, b, c) a
# define _S1BOOSTPP_TUPLE_ELEM_3_1(a, b, c) b
# define _S1BOOSTPP_TUPLE_ELEM_3_2(a, b, c) c
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LIST_LIST_FOR_EACH_I_HPP
# define _S1BOOSTPP_LIST_LIST_FOR_EACH_I_HPP
#
#
# /* _S1BOOSTPP_LIST_FOR_EACH_I */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG() && ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MSVC()
#    define _S1BOOSTPP_LIST_FOR_EACH_I(macro, data, list) _S1BOOSTPP_FOR((macro, data, list, 0), _S1BOOSTPP_LIST_FOR_EACH_I_P, _S1BOOSTPP_LIST_FOR_EACH_I_O, _S1BOOSTPP_LIST_FOR_EACH_I_M)
# else
#    define _S1BOOSTPP_LIST_FOR_EACH_I(macro, data, list) _S1BOOSTPP_LIST_FOR_EACH_I_I(macro, data, list)
#    define _S1BOOSTPP_LIST_FOR_EACH_I_I(macro, data, list) _S1BOOSTPP_FOR((macro, data, list, 0), _S1BOOSTPP_LIST_FOR_EACH_I_P, _S1BOOSTPP_LIST_FOR_EACH_I_O, _S1BOOSTPP_LIST_FOR_EACH_I_M)
# endif
#
# if _S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_STRICT()
#    define _S1BOOSTPP_LIST_FOR_EACH_I_P(r, x) _S1BOOSTPP_LIST_FOR_EACH_I_P_D x
#    define _S1BOOSTPP_LIST_FOR_EACH_I_P_D(m, d, l, i) _S1BOOSTPP_LIST_IS_CONS(l)
# else
#    define _S1BOOSTPP_LIST_FOR_EACH_I_P(r, x) _S1BOOSTPP_LIST_IS_CONS(_S1BOOSTPP_TUPLE_ELEM(4, 2, x))
# endif
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_MWCC()
#    define _S1BOOSTPP_LIST_FOR_EACH_I_O(r, x) _S1BOOSTPP_LIST_FOR_EACH_I_O_D x
#    define _S1BOOSTPP_LIST_FOR_EACH_I_O_D(m, d, l, i) (m, d, _S1BOOSTPP_LIST_REST(l), _S1BOOSTPP_INC(i))
# else
#    define _S1BOOSTPP_LIST_FOR_EACH_I_O(r, x) (_S1BOOSTPP_TUPLE_ELEM(4, 0, x), _S1BOOSTPP_TUPLE_ELEM(4, 1, x), _S1BOOSTPP_LIST_REST(_S1BOOSTPP_TUPLE_ELEM(4, 2, x)), _S1BOOSTPP_INC(_S1BOOSTPP_TUPLE_ELEM(4, 3, x)))
# endif
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#    define _S1BOOSTPP_LIST_FOR_EACH_I_M(r, x) _S1BOOSTPP_LIST_FOR_EACH_I_M_D(r, _S1BOOSTPP_TUPLE_ELEM(4, 0, x), _S1BOOSTPP_TUPLE_ELEM(4, 1, x), _S1BOOSTPP_TUPLE_ELEM(4, 2, x), _S1BOOSTPP_TUPLE_ELEM(4, 3, x))
# else
#    define _S1BOOSTPP_LIST_FOR_EACH_I_M(r, x) _S1BOOSTPP_LIST_FOR_EACH_I_M_I(r, _S1BOOSTPP_TUPLE_REM_4 x)
#    define _S1BOOSTPP_LIST_FOR_EACH_I_M_I(r, x_e) _S1BOOSTPP_LIST_FOR_EACH_I_M_D(r, x_e)
# endif
#
# define _S1BOOSTPP_LIST_FOR_EACH_I_M_D(r, m, d, l, i) m(r, d, i, _S1BOOSTPP_LIST_FIRST(l))
#
# /* _S1BOOSTPP_LIST_FOR_EACH_I_R */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#    define _S1BOOSTPP_LIST_FOR_EACH_I_R(r, macro, data, list) _S1BOOSTPP_FOR_ ## r((macro, data, list, 0), _S1BOOSTPP_LIST_FOR_EACH_I_P, _S1BOOSTPP_LIST_FOR_EACH_I_O, _S1BOOSTPP_LIST_FOR_EACH_I_M)
# else
#    define _S1BOOSTPP_LIST_FOR_EACH_I_R(r, macro, data, list) _S1BOOSTPP_LIST_FOR_EACH_I_R_I(r, macro, data, list)
#    define _S1BOOSTPP_LIST_FOR_EACH_I_R_I(r, macro, data, list) _S1BOOSTPP_FOR_ ## r((macro, data, list, 0), _S1BOOSTPP_LIST_FOR_EACH_I_P, _S1BOOSTPP_LIST_FOR_EACH_I_O, _S1BOOSTPP_LIST_FOR_EACH_I_M)
# endif
#
# endif
# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef _S1BOOSTPP_LIST_FOR_EACH_HPP
# define _S1BOOSTPP_LIST_FOR_EACH_HPP
#
#
# /* _S1BOOSTPP_LIST_FOR_EACH */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#    define _S1BOOSTPP_LIST_FOR_EACH(macro, data, list) _S1BOOSTPP_LIST_FOR_EACH_I(_S1BOOSTPP_LIST_FOR_EACH_O, (macro, data), list)
# else
#    define _S1BOOSTPP_LIST_FOR_EACH(macro, data, list) _S1BOOSTPP_LIST_FOR_EACH_X(macro, data, list)
#    define _S1BOOSTPP_LIST_FOR_EACH_X(macro, data, list) _S1BOOSTPP_LIST_FOR_EACH_I(_S1BOOSTPP_LIST_FOR_EACH_O, (macro, data), list)
# endif
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#    define _S1BOOSTPP_LIST_FOR_EACH_O(r, md, i, elem) _S1BOOSTPP_LIST_FOR_EACH_O_D(r, _S1BOOSTPP_TUPLE_ELEM(2, 0, md), _S1BOOSTPP_TUPLE_ELEM(2, 1, md), elem)
# else
#    define _S1BOOSTPP_LIST_FOR_EACH_O(r, md, i, elem) _S1BOOSTPP_LIST_FOR_EACH_O_I(r, _S1BOOSTPP_TUPLE_REM_2 md, elem)
#    define _S1BOOSTPP_LIST_FOR_EACH_O_I(r, im, elem) _S1BOOSTPP_LIST_FOR_EACH_O_D(r, im, elem)
# endif
#
# define _S1BOOSTPP_LIST_FOR_EACH_O_D(r, m, d, elem) m(r, d, elem)
#
# /* _S1BOOSTPP_LIST_FOR_EACH_R */
#
# if ~_S1BOOSTPP_CONFIG_FLAGS() & _S1BOOSTPP_CONFIG_EDG()
#    define _S1BOOSTPP_LIST_FOR_EACH_R(r, macro, data, list) _S1BOOSTPP_LIST_FOR_EACH_I_R(r, _S1BOOSTPP_LIST_FOR_EACH_O, (macro, data), list)
# else
#    define _S1BOOSTPP_LIST_FOR_EACH_R(r, macro, data, list) _S1BOOSTPP_LIST_FOR_EACH_R_X(r, macro, data, list)
#    define _S1BOOSTPP_LIST_FOR_EACH_R_X(r, macro, data, list) _S1BOOSTPP_LIST_FOR_EACH_I_R(r, _S1BOOSTPP_LIST_FOR_EACH_O, (macro, data), list)
# endif
#
# endif
/**\file
 * Shader1 Public API definitions.
 */
#ifndef __S1_DEFS_H__
#define __S1_DEFS_H__

#ifdef __cplusplus
#  define S1_EXTERN_C           extern "C"
#  if defined(__GNUC__)
#    define S1_NOTHROW          __attribute__((nothrow))
#  elif defined(_MSC_VER)
#    define S1_NOTHROW          __declspec(nothrow)
#  else
#    define S1_NOTHROW
#  endif
#else
#  define S1_EXTERN_C           extern
#  define S1_NOTHROW
#endif

// TODO Later: dllexport/visibility declspec
#define S1_DECLSPEC

#define S1_API          S1_EXTERN_C S1_DECLSPEC S1_NOTHROW

#endif // __S1_DEFS_H__
