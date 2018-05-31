/*
    Shader1
    Copyright (c) 2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_FORWARDDECL_H_
#define SEMANTICS_FORWARDDECL_H_

#include <boost/shared_ptr.hpp>

namespace s1
{
  namespace semantics
  {
    /// Base types
    enum struct BaseType { Invalid, Void, Bool, Int, UInt, Float };
    /// Sampler dimensions
    enum struct SamplerType { _1D, _2D, _3D, CUBE };

    struct Type;
    typedef boost::shared_ptr<Type> TypePtr;

    struct Name;
    typedef boost::shared_ptr<Name> NamePtr;
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_FORWARDDECL_H_
