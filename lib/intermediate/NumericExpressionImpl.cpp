#include "NumericExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpConst.h"
#include "TypeImpl.h"

#include <unicode/ustdio.h>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::NumericExpressionImpl::NumericExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      const UnicodeString& valueStr)
       : ExpressionImpl (handler), valueStr (valueStr),
	 numberType (IntermediateGeneratorSemanticsHandler::DetectNumericType (valueStr))
    {
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::NumericExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> valueType =
	boost::shared_static_cast<TypeImpl> (handler->CreateType (numberType));
	
      return valueType;
    }
    
    namespace
    {
      class ICUError : public U_NAMESPACE_QUALIFIER ErrorCode
      {
      public:
	virtual void handleFailure() const
	{
	  throw Exception (NumberParseError);
	}
      };
    }
    
    void IntermediateGeneratorSemanticsHandler::NumericExpressionImpl::AddToSequence (BlockImpl& block,
										      RegisterID destination)
    {
      U_NAMESPACE_USE
      
      // Mark 'destination' as constant or something?
      
      SequenceOpPtr seqOp;
      // Extract value from string
      if (valueStr.startsWith (UnicodeString ("0x")) || valueStr.startsWith (UnicodeString ("0X")))
      {
	// Parse hex number
	unsigned int val = 0;
	for (int i = 2; i < valueStr.length(); i++)
	{
	  unsigned int digitVal = 0;
	  UChar digit = valueStr[i];
	  if ((digit >= '0') && (digit <= '9'))
	  {
	    digitVal = digit - '0';
	  }
	  else if ((digit >= 'a') && (digit <= 'f'))
	  {
	    digitVal = digit - 'a' + 10;
	  }
	  else if ((digit >= 'A') && (digit <= 'F'))
	  {
	    digitVal = digit - 'A' + 10;
	  }
	  val = (val << 4) + digitVal;
	}
	seqOp = SequenceOpPtr (new SequenceOpConst (destination, val));
      }
      else
      {
	// Parse decimal number
	switch (numberType)
	{
	case Int:
	  {
	    // Create actual sequence operation
	    int n;
	    char dummy;
	    if (u_sscanf (valueStr.getTerminatedBuffer(), "%d%c", &n, &dummy) != 1)
	      throw Exception (NumberParseError);
	    seqOp = SequenceOpPtr (new SequenceOpConst (destination, n));
	  }
	  break;
	case UInt:
	  {
	    // Create actual sequence operation
	    unsigned int n;
	    char dummy;
	    if (u_sscanf (valueStr.getTerminatedBuffer(), "%u%c", &n, &dummy) != 1)
	      throw Exception (NumberParseError);
	    seqOp = SequenceOpPtr (new SequenceOpConst (destination, n));
	  }
	  break;
	case Float:
	  {
	    // Create actual sequence operation
	    float n;
	    char dummy;
	    if (u_sscanf (valueStr.getTerminatedBuffer(), "%f%c", &n, &dummy) != 1)
	      throw Exception (NumberParseError);
	    seqOp = SequenceOpPtr (new SequenceOpConst (destination, n));
	  }
	  break;
	case Void:
	case Bool:
	  break;
	}
      }
      assert (seqOp);
      block.GetSequence()->AddOp (seqOp);
    }
  } // namespace intermediate
} // namespace s1
