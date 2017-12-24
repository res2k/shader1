/*
    Shader1
    Copyright (c) 2017 Frank Richter


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
 * Container for program compilation diagnostics
 */
#ifndef COMPILER_PROGRAMDIAGNOSTICS_H__
#define COMPILER_PROGRAMDIAGNOSTICS_H__

#include "Compiler.h"

#include "base/LibraryObject.h"
#include "base/uc/String.h"
#include "diagnostics/Handler.h"

#include <boost/container/deque.hpp>
#include <boost/optional.hpp>

namespace s1
{
  /// Container for program compilation diagnostics
  class Compiler::ProgramDiagnostics : public LibraryObject,
                                       public diagnostics::Handler
  {
  public:
    ProgramDiagnostics (Library* lib);
    ~ProgramDiagnostics ();

    /// Return number of entries
    size_t GetCount() const;
    /// Diagnostic information class
    enum struct Class
    {
      Error, Warning
    };
    /// Obtain diagnostic information for an entry
    Result<Class> Classify (size_t index) const;
    /// Obtain diagnostic ID for an entry
    Result<const char*> Id (size_t index) const;
  protected:
    struct Entry;
    boost::container::deque<Entry> entries;

    void Add (unsigned int code,
              boost::optional<const uc::String&> info1 = boost::none,
              boost::optional<const uc::String&> info2 = boost::none);

    /**\name diagnostics::Handler implementation
     * @{ */
    void LexerErrorImpl (unsigned int code, const lexer::TokenLocation& location) override;
    void ParseErrorImpl (unsigned int code,
                        const lexer::Token& encounteredToken,
                        lexer::TokenType expectedToken) override;
    void SemanticErrorImpl (unsigned int code) override;
    /** @} */
  };
} // namespace s1

#endif // __COMPILER_PROGRAM_H__
