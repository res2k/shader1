#ifndef __INTERMEDIATE_SEQUENCEOPCONST_H__
#define __INTERMEDIATE_SEQUENCEOPCONST_H__

#include "../BasicType.h"
#include "SequenceOpWithResult.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpConst : public SequenceOpWithResult
    {
      BasicType type;
      union
      {
	bool boolVal;
	int intVal;
	unsigned int uintVal;
	float floatVal;
      };
    public:
      SequenceOpConst (RegisterID destination, bool val)
       : SequenceOpWithResult (destination), type (Bool), boolVal (val) {}
      SequenceOpConst (RegisterID destination, int val)
       : SequenceOpWithResult (destination), type (Int), intVal (val) {}
      SequenceOpConst (RegisterID destination, unsigned int val)
       : SequenceOpWithResult (destination), type (UInt), uintVal (val) {}
      SequenceOpConst (RegisterID destination, float val)
       : SequenceOpWithResult (destination), type (Float), floatVal (val) {}

      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPCONST_H__
