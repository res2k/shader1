#ifndef __INTERMEDIATE_SEQUENCEOP_H__
#define __INTERMEDIATE_SEQUENCEOP_H__

#include <boost/shared_ptr.hpp>

namespace s1
{
  namespace intermediate
  {
    class SequenceOp
    {
    public:
      virtual ~SequenceOp() {}
    };
    typedef boost::shared_ptr<SequenceOp> SequenceOpPtr;
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOP_H__
