/*
    Shader1
    Copyright (c) 2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_FUNCTIONFORMALPARAMETER_H_
#define SEMANTICS_FUNCTIONFORMALPARAMETER_H_

#include "forwarddecl.h"

#include "base/uc/String.h"

#include <vector>

namespace s1
{
  namespace semantics
  {
    struct FunctionFormalParameter
    {
      /// Type of a formal parameter
      enum Type
      {
        /// User-defined parameter
        ptUser,
        /// Automatically generated parameter, representing a global var
        ptAutoGlobal,
        /// Automatically generated parameter for a transfer value
        ptAutoTransfer,

        /// All types greater or equal than this identify auto-added parameters
        ptFirstAutoType = ptAutoGlobal
      };
      enum Direction
      {
        dirDefault = 0,
        dirIn = 1,
        dirOut = 2,
        dirInOut = 3
      };
      /// User-assigned 'input frequency' qualifier for a parameter
      enum Frequency
      {
        /// No particular frequency
        freqAuto = 0,
        /// Uniform qualifier
        freqUniform = 1,
        /// Attribute (Vertex input) qualifier
        freqAttribute = 2
      };

      /// Type of parameter
      Type paramType = ptUser;
      TypePtr type;
      uc::String identifier;
      ExpressionPtr defaultValue;
      Direction dir;
      Frequency freqQualifier;
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_FUNCTIONFORMALPARAMETER_H_
