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

/**\file
 * std::hash<> specializations for various types used in library
 */
#ifndef __BASE_STD_HASH_H__
#define __BASE_STD_HASH_H__

#include <functional>

#include <boost/functional/hash/extensions.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace std
{
  template<typename T> struct hash<boost::intrusive_ptr<T>>
  {
    typedef boost::intrusive_ptr<T> argument_type;
    typedef std::size_t result_type;

    result_type operator()(const argument_type& s) const
    {
      return std::hash<const T*>() (s.get());
    }
  };

  template<typename T> struct hash<boost::shared_ptr<T>>
  {
    typedef boost::shared_ptr<T> argument_type;
    typedef std::size_t result_type;

    result_type operator()(const argument_type& s) const
    {
      return std::hash<const T*>() (s.get());
    }
  };

  template<typename T1, typename T2>
  struct hash<std::pair<T1, T2>>
  {
    typedef std::pair<T1, T2> argument_type;
    typedef std::size_t result_type;

    result_type operator()(const argument_type& s) const
    {
      return boost::hash_value (s);
    }
  };
} // namespace std

#endif // __BASE_STD_HASH_H__
