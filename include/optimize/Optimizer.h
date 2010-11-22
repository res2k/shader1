#ifndef __OPTIMIZE_OPTIMIZER_H__
#define __OPTIMIZE_OPTIMIZER_H__

#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace optimize
  {
    class Optimizer
    {
      unsigned int optimizations;
      void SetOptFlag (unsigned int flag, bool val)
      {
	if (val)
	  optimizations |= flag;
	else
	  optimizations &= ~flag;
      }
    public:
      enum
      {
	optInlineBlocks = 1 << 0,
	optDeadCodeElimination = 1 << 1,
      };
      Optimizer() : optimizations (0) {}
      
      void SetInlineBlocks (bool flag) { SetOptFlag (optInlineBlocks, flag); }
      void SetDeadCodeElimination (bool flag) { SetOptFlag (optDeadCodeElimination, flag); }
      
      intermediate::ProgramPtr ApplyOptimizations (const intermediate::ProgramPtr& program);
    };
  } // namespace optimize
} // namespace s1

#endif // __OPTIMIZE_OPTIMIZER_H__
