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
      
      typedef std::vector<std::pair<boost::shared_ptr<ExpressionImpl>, RegisterPtr> > PostActionList;
      void ExtractBaseExpressionRegs (BlockImpl& block, std::vector<RegisterPtr>& reg,
                                      PostActionList& postActions);
    public:
      TypeConstructorExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
                                     const TypeImplPtr& type,
                                     const ExpressionVector& params);
      
      TypeImplPtr GetValueType () { return type; }
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
                                 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_TYPECONSTRUCTOREXPRESSIONIMPL_H__
