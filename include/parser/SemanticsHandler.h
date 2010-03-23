#ifndef __PARSER_SEMANTICSHANDLER_H__
#define __PARSER_SEMANTICSHANDLER_H__

#include <boost/shared_ptr.hpp>

namespace s1
{
  namespace parser
  {
    /**
    * Semantics handler.
    * The parser calls methods of this class while parsing the syntax.
    * The idea is that implementations build an internal representation from the
    * syntax. Also, semantic checking must be performed by the implementation.
    *
    * The syntax parser will ask the implementation to create objects in response
    * to syntactic constructs. Necessary context will be provided as well.
    *
    * In structure of objects is, in essence, tree-like (although the exact
    * organization is up the implementation).
    */
    struct SemanticsHandler
    {
      virtual ~SemanticsHandler() {}
      
      /**\name Types
      * @{ */
      /// Representation of a type
      struct Type
      {
      };
      typedef boost::shared_ptr<Type> TypePtr;
      
      /// Base types
      enum BaseType { Bool, Int, UInt, Float };
      /// Create a base type
      virtual TypePtr CreateType (BaseType type) = 0;
      /// Sampler dimensions
      enum SamplerType { _1D, _2D, _3D, CUBE };
      /// Create a sampler type
      virtual TypePtr CreateSamplerType (SamplerType dim) = 0;
      /// Create an array type
      virtual TypePtr CreateArrayType (TypePtr baseType) = 0;
      /// Create a vector type
      virtual TypePtr CreateVectorType (TypePtr baseType,
					unsigned int components) = 0;
      /// Create a matrix type
      virtual TypePtr CreateMatrixType (TypePtr baseType,
					unsigned int columns,
					unsigned int rows) = 0;
      /** @} */
    };
  } // namespace parser
} // namespace s1

#endif // __PARSER_SEMANTICSHANDLER_H__
