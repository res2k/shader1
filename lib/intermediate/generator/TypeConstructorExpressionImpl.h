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

#ifndef __INTERMEDIATE_TYPECONSTRUCTOREXPRESSIONIMPL_H__
#define __INTERMEDIATE_TYPECONSTRUCTOREXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl : public ExpressionImpl
    {
      TypeImplPtr type;
      ExpressionVector params;
      
      typedef std::vector<std::pair<boost::intrusive_ptr<ExpressionImpl>, RegisterPtr> > PostActionList;
      bool ExtractBaseExpressionRegs (BlockImpl& block, std::vector<RegisterPtr>& reg,
                                      PostActionList& postActions);
    public:
      TypeConstructorExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
                                     ExpressionContext&& context,
                                     TypeImpl* type,
                                     const ExpressionVector& params);
      
      TypeImplPtr GetValueType () { return type; }
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
                                 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_TYPECONSTRUCTOREXPRESSIONIMPL_H__
