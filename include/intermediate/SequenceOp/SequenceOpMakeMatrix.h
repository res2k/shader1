#ifndef __INTERMEDIATE_SEQUENCEOPMAKEMATRIX_H__
#define __INTERMEDIATE_SEQUENCEOPMAKEMATRIX_H__

#include "../BasicType.h"
#include "SequenceOpWithResult.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpMakeMatrix : public SequenceOpWithResult
    {
      BasicType matrixBaseType;
      unsigned int matrixRows;
      unsigned int matrixCols;
      std::vector<RegisterID> sources;
    public:
      SequenceOpMakeMatrix (RegisterID destination,
			    BasicType matrixBaseType,
			    unsigned int matrixRows, unsigned int matrixCols,
			    const std::vector<RegisterID>& sources);
			    
      RegisterIDSet GetReadRegisters () const;

      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPMAKEMATRIX_H__
