/*
    Shader1
    Copyright (c) 2016 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __UNUSEDFUNCTIONARGUMENTREMOVAL_H__
#define __UNUSEDFUNCTIONARGUMENTREMOVAL_H__

#include "base/uc/String.h"
#include "intermediate/forwarddecl.h"

#include <boost/unordered/unordered_map.hpp>

namespace s1
{
  namespace optimize
  {
    /**
     * Remove unused function arguments.
     * Works globally on all functions. Checks which input arguments each function actually
     * uses and which output arguments are actually defined, then removes the unused/unassigned
     * parameters from the function and call sites.
     */
    class UnusedFunctionArgumentRemoval
    {
    public:
      /// Initialize optimizer for given program.
      UnusedFunctionArgumentRemoval (intermediate::ProgramPtr inputProg);
      ~UnusedFunctionArgumentRemoval ();
      /// Apply optimization to a specific function. Returns same function if no changes were made.
      intermediate::ProgramFunctionPtr Apply (intermediate::ProgramFunctionPtr func);

      /// Apply to a whole program. Returns input pointer if no changes were made.
      static intermediate::ProgramPtr RemoveUnusedFunctionArguments (intermediate::ProgramPtr inputProg);
    private:
      class ArgRemovalVisitor;

      struct UsedArgsBits;
      /// Map: function name to used parameters
      boost::unordered_map<uc::String, UsedArgsBits> funcUsedArgs;

      /// Collect used input/written output args for function
      void CollectFunctionArguments (intermediate::ProgramFunctionPtr func);
    };
  } // namespace optimize
} // namespace s1

#endif // __UNUSEDFUNCTIONARGUMENTREMOVAL_H__
