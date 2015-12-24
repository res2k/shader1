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

#include "base/common.h"
#include "base/ResultCode_internal.h"

#include "compiler/Options.h"

#include <string.h>
#include <boost/make_shared.hpp>

namespace s1
{
  Compiler::Options::Options (Library* lib) : LibraryObject (lib)
  {
    memset (optimizeFlags, 0, sizeof (optimizeFlags));
  }
  
  Compiler::Options::Options (Library* lib, const Options& other) : LibraryObject (lib)
  {
    memcpy (optimizeFlags, other.optimizeFlags, sizeof (optimizeFlags));
  }

  void Compiler::Options::SetOptimizationFlag (Optimization opt, bool enable)
  {
    assert ((opt >= 0) && (opt < numOptimizations));
    optimizeFlags[opt] = enable;
  }
  
  bool Compiler::Options::GetOptimizationFlag (Optimization opt) const
  {
    assert ((opt >= 0) && (opt < numOptimizations));
    return optimizeFlags[opt];
  }
  
  void Compiler::Options::SetOptimizationLevel (int level)
  {
    optimizeFlags[optBlockInlining] = level > 0;
    optimizeFlags[optDeadCodeElimination] = level > 0;
    optimizeFlags[optConstantFolding] = level > 0;
  }
  
  bool Compiler::Options::ParseOptimizationFlagString (const char* flagStr, Optimization& opt, bool& flag)
  {
    bool flagVal = true;
    if (strncmp (flagStr, "no-", 3) == 0)
    {
      flagVal = false;
      flagStr += 3;
    }
    
    if (strcmp (flagStr, "inline-blocks") == 0)
    {
      opt = optBlockInlining;
      flag = flagVal;
      return true;
    }
    else if (strcmp (flagStr, "dce") == 0)
    {
      opt = optDeadCodeElimination;
      flag = flagVal;
      return true;
    }
    else if (strcmp (flagStr, "constant-folding") == 0)
    {
      opt = optConstantFolding;
      flag = flagVal;
      return true;
    }
    else
    {
      return false;
    }
  }
} // namespace s1
