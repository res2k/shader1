#include "OptimizationFlags.h"

#include "optimize/Optimizer.h"

#include <string.h>

OptimizationFlags::OptimizationFlags ()
 : doInlineBlocks (true), doDeadCodeElimination (true)
{
}

bool OptimizationFlags::ParseFlag (const char* flagString)
{
  if (strcmp (flagString, "0") == 0)
  {
    // Disable all optimizations
    doInlineBlocks = false;
    doDeadCodeElimination = false;
    return true;
  }
  
  bool flagVal = true;
  if (strncmp (flagString, "no-", 3) == 0)
  {
    flagVal = false;
    flagString += 3;
  }
  
  if (strcmp (flagString, "inline-blocks") == 0)
  {
    doInlineBlocks = flagVal;
    return true;
  }
  else if (strcmp (flagString, "dce") == 0)
  {
    doDeadCodeElimination = flagVal;
    return true;
  }
  else
  {
    return false;
  }
}

void OptimizationFlags::ApplyFlags (s1::optimize::Optimizer& opt)
{
  opt.SetInlineBlocks (doInlineBlocks);
  opt.SetDeadCodeElimination (doDeadCodeElimination);
}
