/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

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
    
    result_NamePtr ResolveIdentifier (const s1::uc::String& identifier)
    {
      return NamePtr (new CommonName (identifier, TypePtr (), ExpressionPtr (), false));
    }
  };
  
  ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel)
  {
    return ScopePtr (new SloppyScope (this,
      boost::static_pointer_cast<SloppyScope> (parentScope),
      scopeLevel));
  }
};

#endif // __TESTSEMANTICSHANDLERSLOPPYIDENTIFIERS_H__
