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

#ifndef __BASE_NULLPTR_H__
#define __BASE_NULLPTR_H__

#if !defined(CXX_HAS_NULLPTR)

/*
  Based on "library implementation" in C++ Standards Committee paper 2431
  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf
*/
namespace std
{ 
  class nullptr_t
  {
  private:
    void operator&() const;

  public:
    template<typename T>
    operator T*() const
    {
      return 0;
    }

    template<class C, typename T>
    operator T C::*() const
    {
      return 0;
    }
  };
}

/* Hack around the "identifier 'nullptr' will become a keyword in C++0x"
 * warnings gcc 4.6 emits.
 * The "proper" way - using a #pragma GCC diagnostic - is, unfortunately,
 * broken: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=48914
 * Instead, to avoid use of that identifier, rename that identifier.
 */
#define nullptr s1_nullptr

using ::std::nullptr_t;
const nullptr_t nullptr = {};

#endif // !defined(CXX_HAS_NULLPTR)

#endif // __BASE_NULLPTR_H__
