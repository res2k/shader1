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

/**\file
 * Class to create and fill Sequences.
 */
#ifndef __INTERMEDIATE_SEQUENCEBUILDER_H__
#define __INTERMEDIATE_SEQUENCEBUILDER_H__

#include "forwarddecl.h"
#include "Sequence.h"

namespace s1
{
  namespace intermediate
  {
    /**
     * Class to create and fill Sequences.
     */
    class SequenceBuilder
    {
      /* TODO: Currently only a thin wrapper that has 'friend' access to
       * Sequence members (and can thus fill a sequence).
       * So that actually means a sequence returned by a builder is not
       * strictly immutable.
       * Better transition to become a "real" factory */
      SequencePtr sequence;
      
      typedef Sequence::Register Register;
    public:
      typedef Sequence::TypePtr TypePtr;
      
      SequenceBuilder();

      const SequencePtr& GetSequence();

      /** \name Sequence manipulation
       * @{ */
      /// Add an operation
      void AddOp (SequenceOpPtr op);
      /// Insert an operation
      void InsertOp (size_t before, SequenceOpPtr op);
      /// Get number of operations
      size_t GetNumOps() const;
      /// Get a specific op
      SequenceOpPtr GetOp(size_t index) const;
      /// Clear sequence
      void Clear ();
      /** @} */
      
      RegisterPtr AllocateRegister (const TypePtr& originalType,
                                    const UnicodeString& name);
      RegisterPtr AllocateRegister (const RegisterPtr& oldReg);
      
      void SetIdentifierRegister (const UnicodeString& identifier, const RegisterPtr& regID);
      RegisterPtr GetIdentifierRegister (const UnicodeString& identifier) const;
      
      typedef Sequence::IdentRegPair IdentRegPair;
      typedef Sequence::IdentifierToRegMap IdentifierToRegMap;
      /// Get current identifiers-to-register map
      const IdentifierToRegMap& GetIdentifierToRegisterMap () const;
      //{ return identToReg; }
      void SetIdentifierRegisters (const IdentifierToRegMap& map);
      
      typedef Sequence::RegisterImpMappings RegisterImpMappings;
      /**
       * Set an import mapping for a register.
       * \a localReg will be initialized with the value from \a parentRegName
       * in the parent scope. If a mapping for \a localReg already exists
       * it will be replaced.
       */
      void SetImport (const RegisterPtr& localReg,
                      const UnicodeString& parentRegName);
      void AddImports (const RegisterImpMappings& imports);
      const RegisterImpMappings& GetImports () const;
      typedef Sequence::RegisterExpMappings RegisterExpMappings;
      RegisterExpMappings& GetExports ();
      /**
       * Set an export mapping for a register.
       * When the block ends, the value of \a parentRegName in the parent scope
       * will be set to the value of \a localReg. If a mapping for \a parentRegName already
       * exists it will be replaced.
       */
      void SetExport (const UnicodeString& parentRegName,
                      const RegisterPtr& localReg);
      void AddExports (const RegisterExpMappings& exports);
      void CleanUnusedImportsExports (const RegisterSet& keepImports = RegisterSet (),
                                      const RegisterSet& keepExports = RegisterSet ());

      RegisterSet GetAllReadRegisters() const;
      RegisterSet GetAllWrittenRegisters() const;
    protected:
      IdentifierToRegMap identToReg;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEBUILDER_H__
