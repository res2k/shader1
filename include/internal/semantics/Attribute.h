/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_ATTRIBUTE_H_
#define SEMANTICS_ATTRIBUTE_H_

namespace s1
{
  namespace semantics
  {
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

      static Attribute Identify (const uc::String& attributeStr);
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_ATTRIBUTE_H_
