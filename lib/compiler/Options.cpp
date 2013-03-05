#include "base/common.h"

#include "compiler/Options.h"

#include <string.h>
#include <boost/make_shared.hpp>

namespace s1
{
  Compiler::Options::Options ()
  {
    memset (optimizeFlags, 0, sizeof (optimizeFlags));
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
