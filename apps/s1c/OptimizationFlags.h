#ifndef __OPTIMIZATIONFLAGS_H__
#define __OPTIMIZATIONFLAGS_H__

namespace s1
{
  namespace optimize
  {
    class Optimizer;
  } // namespace optimize
} // namespace s1

class OptimizationFlags
{
  bool doInlineBlocks;
  bool doDeadCodeElimination;
public:
  OptimizationFlags ();
  
  bool ParseFlag (const char* flagString);
  void ApplyFlags (s1::optimize::Optimizer& opt);
};

#endif // __OPTIMIZATIONFLAGS_H__
