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
      SequenceOpConst (RegisterPtr destination, bool val)
       : SequenceOpWithResult (destination), type (Bool), boolVal (val) {}
      SequenceOpConst (RegisterPtr destination, int val)
       : SequenceOpWithResult (destination), type (Int), intVal (val) {}
      SequenceOpConst (RegisterPtr destination, unsigned int val)
       : SequenceOpWithResult (destination), type (UInt), uintVal (val) {}
      SequenceOpConst (RegisterPtr destination, float val)
       : SequenceOpWithResult (destination), type (Float), floatVal (val) {}

      RegisterSet GetReadRegisters () const { return RegisterSet(); }
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPCONST_H__
