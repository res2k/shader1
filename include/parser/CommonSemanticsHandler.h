#ifndef __PARSER_COMMONSEMANTICSHANDLER_H__
#define __PARSER_COMMONSEMANTICSHANDLER_H__

#include "base/unordered_map"
#include "SemanticsHandler.h"

namespace s1
{
  namespace parser
  {
    /**
     * Common implementation of semantics handler behaviour that is the same across
     * most semantics handlers (such as name or scope handling).
     */
    class CommonSemanticsHandler : public SemanticsHandler
    {
    protected:
      struct CommonType : public Type
      {
	enum Class
	{
	  Base, Sampler, Array, Vector, Matrix
	};
	Class typeClass;
	BaseType base;
	SamplerType sampler;
	TypePtr avmBase;
	unsigned int vectorDim;
	unsigned int matrixCols;
	unsigned int matrixRows;
	
	CommonType (BaseType base) : typeClass (Base), base (base) {}
	CommonType (SamplerType sampler) : typeClass (Sampler), sampler (sampler) {}
	CommonType (TypePtr aBase) : typeClass (Array), avmBase (aBase) {}
	CommonType (TypePtr vBase, unsigned int d)
	 : typeClass (Vector), avmBase (vBase), vectorDim (d) {}
	CommonType (TypePtr mBase, unsigned int c, unsigned int r)
	 : typeClass (Matrix), avmBase (mBase), matrixCols (c), matrixRows (r) {}
	
	/// Returns whether this type is losslessly assignable to \a to.
	bool CompatibleLossless (const CommonType& to);
	bool CompatibleLossy (const s1::parser::CommonSemanticsHandler::CommonType& to);
	bool IsEqual (const CommonType& other);
      };
      
      /**\name Type utilities
       * @{ */
      boost::shared_ptr<CommonType> GetHigherPrecision (
	const boost::shared_ptr<CommonType>& t1, const boost::shared_ptr<CommonType>& t2);
      /**@}*/
      
      struct CommonName : public Name
      {
	UnicodeString identifier;
	NameType type;
	
	/* Variables/Constants: type of variable/constant
	 * Functions: type of return value
	 * Type aliases: aliased type
	 */
	TypePtr valueType;
	// Variables/Constants: value
	ExpressionPtr varValue;
	// Distinguish between variable/constant
	bool varConstant;
	
	CommonName (const UnicodeString& identifier, NameType type, TypePtr typeOfName)
	 : identifier (identifier), type (type), valueType (typeOfName) {}
	CommonName (const UnicodeString& identifier, TypePtr typeOfName,
		    ExpressionPtr value, bool constant)
	 : identifier (identifier), type (Variable), valueType (typeOfName),
	   varValue (value), varConstant (constant) {}
	
	NameType GetType() { return type; }
      };

      class CommonScope : public Scope
      {
	typedef std::tr1::unordered_map<UnicodeString, NamePtr> IdentifierMap;
	IdentifierMap identifiers;
	
	void CheckIdentifierUnique (const UnicodeString& identifier);
	
	CommonScope* parent;
	ScopeLevel level;
      public:
	CommonScope (CommonScope* parent, ScopeLevel level);
	
	NamePtr AddVariable (TypePtr type,
	  const UnicodeString& identifier,
	  ExpressionPtr initialValue,
	  bool constant);
	  
	NamePtr AddTypeAlias (TypePtr aliasedType,
	  const UnicodeString& identifier);
	  
	NamePtr AddFunction (TypePtr returnType,
	  const UnicodeString& identifier);
      
	NamePtr ResolveIdentifier (const UnicodeString& identifier);
      };
      
    public:  
      TypePtr CreateType (BaseType type);
      TypePtr CreateSamplerType (SamplerType dim);
      TypePtr CreateArrayType (TypePtr baseType);
      TypePtr CreateVectorType (TypePtr baseType,
				unsigned int components);
      TypePtr CreateMatrixType (TypePtr baseType,
				unsigned int columns,
				unsigned int rows);
      
      ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel);
    };
    
  } // namespace parser
} // namespace s1
    
#endif // __PARSER_COMMONSEMANTICSHANDLER_H__
