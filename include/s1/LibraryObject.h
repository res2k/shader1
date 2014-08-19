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
 * Base class for objects created by a Library
 */
#ifndef __S1_LIBRARYOBJECT_H__
#define __S1_LIBRARYOBJECT_H__

#include "s1/warn_off.h"

#include "s1/Ptr.h"

S1TYPE_DECLARE_FWD(Library);

#define S1TYPE_INFO_s1_LibraryObject   (S1_TYPE_MAKE_NAME(LibraryObject), S1TYPE_INFO_s1_Object)
/**
 * Base class for all objects created by a Library directly or
 * some LibraryObject descendant.
 * Can be used to query to which library an object belongs to.
 * \extends s1_Object
 */
S1TYPE_DECLARE(S1TYPE_INFO_s1_LibraryObject);

/**
 * Query the library used to create an object.
 * \remarks Does \em not add a reference to the returned object.
 * \memberof s1_LibraryObject
 */
S1_API(s1_Library*) s1_libraryobject_get_library (s1_LibraryObject* obj);

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    /**
     * Base class for all objects created by a Library directly or
     * some LibraryObject descendant.
     * Can be used to query to which library an object belongs to.
     */
    class LibraryObject : public Object
    {
    public:
      /// Smart pointer class for LibraryObject instances.
      typedef Ptr<LibraryObject> Pointer;

      /**
       * Query the library used to create an object.
       */
      Library* GetLibrary()
      {
        return s1_libraryobject_get_library (this);
      }
    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::LibraryObject LibraryObject;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_LIBRARYOBJECT_H__
