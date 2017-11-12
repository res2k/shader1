/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef COMPILER_BACKENDPROGRAM_H_
#define COMPILER_BACKENDPROGRAM_H_

#include "Backend.h"

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace s1
{
  class Compiler::Backend::Program : public boost::intrusive_ref_counter<Compiler::Backend::Program>
  {
  public:
    virtual ~Program () {}

    virtual const codegen::StringsArrayPtr& GetProgramLines () = 0;
    /// Discard pointer to lines
    virtual void DiscardLines () = 0;
  };
} // namespace s1

#endif // COMPILER_BACKENDPROGRAM_H_
