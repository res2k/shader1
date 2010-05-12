#ifndef __INTERMEDIATE_FORWARDDECL_H__
#define __INTERMEDIATE_FORWARDDECL_H__

#include <boost/shared_ptr.hpp>

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler;
    
    class Sequence;
    typedef boost::shared_ptr<Sequence> SequencePtr;
    class Program;
    typedef boost::shared_ptr<Program> ProgramPtr;
    class ProgramFunction;
    typedef boost::shared_ptr<ProgramFunction> ProgramFunctionPtr;
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_FORWARDDECL_H__