/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef ASSERT_EQUALS_CH_H_
#define ASSERT_EQUALS_CH_H_

namespace
{
    template<typename T>
    static inline typename std::make_unsigned<T>::type as_unsigned (T x)
    {
        return static_cast<typename std::make_unsigned<T>::type> (x);
    }
} // anonymous namespace

#define TS_ASSERT_EQUALS_CH(A, B)                                     \
  TS_ASSERT_EQUALS(static_cast<uint_least32_t> (as_unsigned (A)),     \
                   static_cast<uint_least32_t> (B))

#endif // ASSERT_EQUALS_CH_H_
