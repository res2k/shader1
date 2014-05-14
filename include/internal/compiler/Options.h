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

#ifndef __COMPILER_OPTIONS_H__
#define __COMPILER_OPTIONS_H__

#include "s1/Options.h"

#include "base/LibraryObject.h"
#include "Compiler.h"

namespace s1
{
  class Compiler::Options : public LibraryObject
  {
  public:
    enum Optimization
    {
      optBlockInlining = S1_OPT_BLOCK_INLINING,
      optDeadCodeElimination = S1_OPT_DEAD_CODE_ELIMINATION,
      optConstantFolding = S1_OPT_CONSTANT_FOLDING,
      
      numOptimizations
    };
    
    Options (Library* lib);
    Options (Library* lib, const Options& other);
    
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
