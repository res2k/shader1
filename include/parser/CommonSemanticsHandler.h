#ifndef __PARSER_COMMONSEMANTICSHANDLER_H__
#define __PARSER_COMMONSEMANTICSHANDLER_H__

#include <boost/enable_shared_from_this.hpp>

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
	bool CompatibleLossless (const CommonType& to) const;
	bool CompatibleLossy (const s1::parser::CommonSemanticsHandler::CommonType& to) const;
	bool IsEqual (const CommonType& other) const;
	bool IsPrecisionHigherEqual (const CommonType& other) const;
      };
      
      /**\name Type utilities
       * @{ */
      static boost::shared_ptr<CommonType> GetHigherPrecisionType (
	const boost::shared_ptr<CommonType>& t1, const boost::shared_ptr<CommonType>& t2);
      static BaseType DetectNumericType (const UnicodeString& numericStr);
      /**@}*/
      
      /**\name Attribute utilities
       * @{ */
      struct Attribute
      {
	enum AttrClass
	{
	  Unknown,
	  
	  arrayLength,
	  
	  matrixRow,
	  matrixCol,
	  matrixTranspose,
	  matrixInvert,
	  
	  vectorSwizzle
	};
	AttrClass attrClass;
	unsigned char swizzleCompNum;
	unsigned char swizzleComps;
	
	Attribute (AttrClass ac) : attrClass (ac), swizzleCompNum (0), swizzleComps (0) {}
	Attribute (unsigned char swizNum, unsigned char comp1 = 0, unsigned char comp2 = 0,
		   unsigned char comp3 = 0, unsigned char comp4 = 0)
	 : attrClass (vectorSwizzle), swizzleCompNum (swizNum),
	   swizzleComps ((comp1 & 3) | ((comp2 & 3) << 2) | ((comp3 & 3) << 4) | ((comp4 & 3) << 6))
	{}
      };
      static Attribute IdentifyAttribute (const UnicodeString& attributeStr);
      TypePtr GetAttributeType (const boost::shared_ptr<CommonType>& expressionType,
				const Attribute& attr);
      /** @} */
      
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
	TypePtr GetAliasedType()
	{ return type == TypeAlias ? valueType : TypePtr (); }
      };

      class CommonScope : public Scope,
			  public boost::enable_shared_from_this<CommonScope>
      {
	friend class CommonSemanticsHandler;
	
	typedef std::tr1::unordered_map<UnicodeString, NamePtr> IdentifierMap;
	IdentifierMap identifiers;
	
	void CheckIdentifierUnique (const UnicodeString& identifier);
	
	CommonSemanticsHandler* handler;
	boost::shared_ptr<CommonScope> parent;
	ScopeLevel level;
      public:
	CommonScope (CommonSemanticsHandler* handler,
		     const boost::shared_ptr<CommonScope>& parent, ScopeLevel level);
	
	NamePtr AddVariable (TypePtr type,
	  const UnicodeString& identifier,
	  ExpressionPtr initialValue,
	  bool constant);
	  
	NamePtr AddTypeAlias (TypePtr aliasedType,
	  const UnicodeString& identifier);
	  
	BlockPtr AddFunction (TypePtr returnType,
	  const UnicodeString& identifier,
	  const FunctionFormalParameters& params);
      
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
