/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef BASE_INTRUSIVE_PTR_H_
#define BASE_INTRUSIVE_PTR_H_

/**\file
 * Convenience helpers for boost::intrusive_ptr<>
 */

#include <boost/smart_ptr/intrusive_ptr.hpp>

namespace s1
{
  //@{
  /// Combine a intrusive pointer get() with a static_cast
  template<typename T, typename U>
  T* get_static_ptr (boost::intrusive_ptr<U> const & p) { return static_cast<T*> (p.get()); }
  template<typename T, typename U>
  T* get_static_ptr (U* p) { return static_cast<T*> (p); }
  //@}

  //@{
  /// Get corresponding intrusive_ptr<> instance for a pointer (plain or intrusive)
  template<typename T>
  boost::intrusive_ptr<T> get_intrusive (boost::intrusive_ptr<T> const & p) { return p; }
  template<typename T>
  boost::intrusive_ptr<T> get_intrusive (T* p) { return p; }
  //@}

  /// Create a new instance of an intrusive_ptr<>
  template<typename T, typename... Args>
  boost::intrusive_ptr<T> make_intrusive (Args&&... args) { return new T (std::forward<Args> (args)...); }
} // namespace s1

#endif // BASE_INTRUSIVE_PTR_H_
