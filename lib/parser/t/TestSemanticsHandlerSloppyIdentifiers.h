#ifndef __TESTSEMANTICSHANDLERSLOPPYIDENTIFIERS_H__
#define __TESTSEMANTICSHANDLERSLOPPYIDENTIFIERS_H__

#include "TestSemanticsHandler.h"

class TestSemanticsHandlerSloppyIdentifiers : public TestSemanticsHandler
{
public:
  class SloppyScope : public TestScope
  {
  public:
    SloppyScope (TestSemanticsHandlerSloppyIdentifiers* owner,
		 const boost::shared_ptr<SloppyScope>& parent,
		 ScopeLevel level)
     : TestScope (owner, parent, level) {}
    
    NamePtr ResolveIdentifier (const UnicodeString& identifier)
    {
      return NamePtr (new CommonName (identifier, TypePtr (), ExpressionPtr (), false));
    }
  };
  
  ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel)
  {
    return ScopePtr (new SloppyScope (this,
      boost::shared_static_cast<SloppyScope> (parentScope),
      scopeLevel));
  }
};

#endif // __TESTSEMANTICSHANDLERSLOPPYIDENTIFIERS_H__
