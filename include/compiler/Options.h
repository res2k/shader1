#ifndef __COMPILER_OPTIONS_H__
#define __COMPILER_OPTIONS_H__

#include "Compiler.h"

namespace s1
{
  class Compiler::Options
  {
  protected:
    friend class Compiler;
    
    static Compiler::OptionsPtr Create ();
    
    Options ();
  public:
    enum Optimization
    {
      optBlockInlining,
      optDeadCodeElimination,
      optConstantFolding,
      
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
