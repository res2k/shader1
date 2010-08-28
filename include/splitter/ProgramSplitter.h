#ifndef __SPLITTER_PROGRAMSPLITTER_H__
#define __SPLITTER_PROGRAMSPLITTER_H__

#include "base/unordered_map"
#include "intermediate/forwarddecl.h"

#include <unicode/unistr.h>

namespace s1
{
  namespace splitter
  {
    class ProgramSplitter
    {
      intermediate::ProgramPtr inputProgram;
      intermediate::ProgramPtr outputProgram;
      
      typedef std::tr1::unordered_map<UnicodeString, unsigned int> ParamMap;
      ParamMap paramFlags;
    public:
      void SetInputProgram (const intermediate::ProgramPtr& program);
      void SetInputFreqFlags (const UnicodeString& inpName, unsigned int flags);
      
      void PerformSplit ();
      
      const intermediate::ProgramPtr& GetOutputProgram ();
    };
  } // namespace splitter
} // namespace s1
    
#endif // __SPLITTER_PROGRAMSPLITTER_H__
