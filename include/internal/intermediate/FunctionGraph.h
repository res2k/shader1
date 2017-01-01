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

#ifndef __INTERMEDIATE_FUNCTIONGRAPH_H__
#define __INTERMEDIATE_FUNCTIONGRAPH_H__

#include "forwarddecl.h"
#include "base/uc/String.h"

#include <memory>

namespace s1
{
  namespace intermediate
  {
    /**
     * Function (call) graph for programs.
     */
    class FunctionGraph
    {
    public:
      /// Build a function graph for the given program.
      FunctionGraph (const Program& prog);
      ~FunctionGraph ();

      /**\name Information methods
       * @{ */
      /**
       * Returns whether a function is recursive
       * (ie it's in a function graph cycle).
       */
      bool IsRecursive (const uc::String& ident) const;
      /** @} */
    private:
      struct Data;
      std::unique_ptr<Data> data;

      void BuildGraph (const Program& prog);
      void CollectRecursive () const;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_FUNCTIONGRAPH_H__
