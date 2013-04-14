#ifndef __COMPILER_OPTIONS_H__
#define __COMPILER_OPTIONS_H__

#include "s1/Options.h"

#include "base/LibraryObject.h"
#include "Compiler.h"

namespace s1
{
  class Compiler::Options : public LibraryObject
  {
  protected:
    friend class Compiler;
    Options (Library* lib);
  public:
    enum Optimization
    {
      optBlockInlining = S1_OPT_BLOCK_INLINING,
      optDeadCodeElimination = S1_OPT_DEAD_CODE_ELIMINATION,
      optConstantFolding = S1_OPT_CONSTANT_FOLDING,
      
      numOptimizations
    };
    
    void SetOptimizationFlag (Optimization opt, bool enable);
    bool GetOptimizationFlag (Optimization opt) const;
    
    void SetOptimizationLevel (int level);
    
    bool ParseOptimizationFlagString (const char* flagStr, Optimization& opt, bool& flag);
    
    bool SetOptimizationFlagFromStr (const char* flagStr)
    {
      Optimization opt;
      bool flag;
      if (ParseOptimizationFlagString (flagStr, opt, flag))
      {
	SetOptimizationFlag (opt, flag);
	return true;
      }
      return false;
    }
  protected:
    bool optimizeFlags[numOptimizations];
  };
} //namespace s1

#endif // __COMPILER_OPTIONS_H__
