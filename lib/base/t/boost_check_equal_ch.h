/*
    Shader1
    Copyright (c) 2017-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef BOOST_CHECK_EQUAL_CH_
#define BOOST_CHECK_EQUAL_CH_

namespace
{
    template<typename T>
    static inline typename std::make_unsigned<T>::type as_unsigned (T x)
    {
        return static_cast<typename std::make_unsigned<T>::type> (x);
    }
} // anonymous namespace

#define BOOST_CHECK_EQUAL_CH(A, B)                                     \
  BOOST_CHECK_EQUAL(static_cast<uint_least32_t> (as_unsigned (A)),     \
                    static_cast<uint_least32_t> (B))

#endif // BOOST_CHECK_EQUAL_CH_
