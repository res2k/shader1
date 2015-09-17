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

#ifndef __INTERMEDIATE_FORWARDDECL_H__
#define __INTERMEDIATE_FORWARDDECL_H__

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler;
    
    class Sequence;
    typedef boost::shared_ptr<Sequence> SequencePtr;
    class SequenceBuilder;
    typedef boost::shared_ptr<SequenceBuilder> SequenceBuilderPtr;
    class SequenceOp;
    typedef boost::intrusive_ptr<SequenceOp> SequenceOpPtr;
    class Program;
    typedef boost::shared_ptr<Program> ProgramPtr;
    class ProgramFunction;
    typedef boost::shared_ptr<ProgramFunction> ProgramFunctionPtr;
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_FORWARDDECL_H__