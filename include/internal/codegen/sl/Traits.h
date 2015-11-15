/*
Shader1
Copyright (c) 2015 Frank Richter


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
 * SL code generation language traits
 */
#ifndef __CODEGEN_SL_TRAITS_H__
#define __CODEGEN_SL_TRAITS_H__

#include "base/uc/String.h"
#include "intermediate/BasicType.h"
#include "parser/SemanticsHandler.h"

namespace s1
{
  namespace codegen
  {
    namespace sl
    {
      struct Traits
      {
        virtual ~Traits () {}

        /**
         * Encode an identifier such that it only consists of ASCII alphanumeric chars
         * and underscores (and does not begin with a number).
         */
        static uc::String IdentifierToASCII (const uc::String& identifier);

        /// Convert identifier to a string suitable in the target language
        virtual uc::String ConvertIdentifier (const uc::String& identifier) const;

        /// Return type string for a basic type.
        virtual uc::String TypeString (intermediate::BasicType type) const = 0;
        /**
         * Return type string for a type.
         * \param type Type object.
         * \param arraySize (Optional) size of an array.
         * \returns A string pair. The first string is the type string, the second string
         *   is any suffix (for arrays).
         */
        virtual std::pair<uc::String, uc::String> TypeString (const parser::SemanticsHandler::TypePtr& type,
                                                              const size_t* arraySize) const = 0;
      };
    } // namespace sl
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SL_TRAITS_H__
