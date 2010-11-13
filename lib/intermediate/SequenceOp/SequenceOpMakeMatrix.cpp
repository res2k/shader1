#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpMakeMatrix.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpMakeMatrix::SequenceOpMakeMatrix (RegisterPtr destination,
						BasicType matrixBaseType,
						unsigned int matrixRows, unsigned int matrixCols,
						const std::vector<RegisterPtr>& sources)
      : SequenceOpWithResult (destination), matrixBaseType (matrixBaseType),
	matrixRows (matrixRows), matrixCols (matrixCols), sources (sources)
    {}
			  
    RegisterSet SequenceOpMakeMatrix::GetReadRegisters () const
    {
      RegisterSet set;
      set.insert (sources.begin(), sources.end());
      return set;
    }

    void SequenceOpMakeMatrix::Visit (SequenceVisitor& visitor)
    {
      visitor.OpMakeMatrix (destination, matrixBaseType, matrixRows, matrixCols, sources);
    }
  } // namespace intermediate
} // namespace s1
