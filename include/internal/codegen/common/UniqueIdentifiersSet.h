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
 * Helper class to ensure that unique identifiers are produced in case
 * of possible conflicts
 */
#ifndef __CODEGEN_COMMON_UNIQUEIDENTIFIERSSET_H__
#define __CODEGEN_COMMON_UNIQUEIDENTIFIERSSET_H__

#include <boost/unordered_set.hpp>

namespace s1
{
  namespace codegen
  {
    class UniqueIdentifiersSet
    {
      boost::unordered_set<uc::String> seenIdentifiers;
    public:
      /// Get a unique identifier, based on the given identifier.
      uc::String Get (const uc::String& identifier);
      /// Make a identifier unique
      void MakeUnique (uc::String& identifier) { identifier = Get (identifier); }
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_COMMON_UNIQUEIDENTIFIERSSET_H__
