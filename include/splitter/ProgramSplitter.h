#ifndef __SPLITTER_PROGRAMSPLITTER_H__
#define __SPLITTER_PROGRAMSPLITTER_H__

#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace splitter
  {
    class ProgramSplitter
    {
      intermediate::ProgramPtr inputProgram;
      intermediate::ProgramPtr outputProgram;
    public:
      void SetInputProgram (const intermediate::ProgramPtr& program);
      
      void PerformSplit ();
      
      const intermediate::ProgramPtr& GetOutputProgram ();
    };
  } // namespace splitter
} // namespace s1
    
#endif // __SPLITTER_PROGRAMSPLITTER_H__
