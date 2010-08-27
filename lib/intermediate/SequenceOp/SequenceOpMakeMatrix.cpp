#include "intermediate/SequenceOp/SequenceOpMakeMatrix.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpMakeMatrix::SequenceOpMakeMatrix (RegisterID destination,
						BasicType matrixBaseType,
						unsigned int matrixRows, unsigned int matrixCols,
						const std::vector<RegisterID>& sources)
      : SequenceOpWithResult (destination), matrixBaseType (matrixBaseType),
	matrixRows (matrixRows), matrixCols (matrixCols), sources (sources)
    {}
			  
    RegisterIDSet SequenceOpMakeMatrix::GetReadRegisters () const
    {
      RegisterIDSet set;
      set.insert (sources.begin(), sources.end());
      return set;
    }

    void SequenceOpMakeMatrix::Visit (SequenceVisitor& visitor)
    {
      switch (matrixBaseType)
      {
      case Bool:
	visitor.OpMakeMatrix (destination, SequenceVisitor::Bool, matrixRows, matrixCols, sources);
	break;
      case Int:
	visitor.OpMakeMatrix (destination, SequenceVisitor::Int, matrixRows, matrixCols, sources);
	break;
      case UInt:
	visitor.OpMakeMatrix (destination, SequenceVisitor::UInt, matrixRows, matrixCols, sources);
	break;
      case Float:
	visitor.OpMakeMatrix (destination, SequenceVisitor::Float, matrixRows, matrixCols, sources);
	break;
      }
    }
  } // namespace intermediate
} // namespace s1
