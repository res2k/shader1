#ifndef __INTERMEDIATE_SEQUENCEOPUNARYOP_H__
#define __INTERMEDIATE_SEQUENCEOPUNARYOP_H__

#include "../SequenceVisitor.h"
#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpUnaryOp : public SequenceOpUnary
    {
      SequenceVisitor::UnaryOp op;
    public:
      SequenceOpUnaryOp (RegisterPtr destination,
			 SequenceVisitor::UnaryOp op,
			 RegisterPtr source);
		       
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPUNARYOP_H__
