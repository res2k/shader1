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

#include <boost/smart_ptr/intrusive_ptr.hpp>

namespace s1
{
  namespace semantics
  {
    /// Base types
    enum struct BaseType { Invalid, Void, Bool, Int, UInt, Float };
    /// Sampler dimensions
    enum struct SamplerType { _1D, _2D, _3D, CUBE };

    class Type;
    typedef boost::intrusive_ptr<Type> TypePtr;

    struct Attribute;

    struct Name;
    typedef boost::intrusive_ptr<Name> NamePtr;

    struct Expression;
    typedef boost::intrusive_ptr<Expression> ExpressionPtr;

    struct Function;
    typedef boost::intrusive_ptr<Function> FunctionPtr;

    /**
     * Levels of scope.
     * Not all declarations are allowed in all levels.
     */
    enum struct ScopeLevel
    { 
      /// Builtin functions (and possibly variables+types)
      Builtin,
      /// Global functions, variables and types
      Global,
      /// Function-local variables and types
      Function
    };

    struct Scope;
    typedef boost::intrusive_ptr<Scope> ScopePtr;

    struct Block;
    typedef boost::intrusive_ptr<Block> BlockPtr;
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_FORWARDDECL_H_
