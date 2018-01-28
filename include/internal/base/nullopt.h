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

/**\file
 * Definition of nullopt_t and nullopt
 */
#ifndef BASE_NULLOPT_H_
#define BASE_NULLOPT_H_

#include <boost/none.hpp>

namespace s1
{
  typedef boost::none_t nullopt_t;
  const nullopt_t nullopt = boost::none;
} // namespace s1

#endif // BASE_NULLOPT_H_
