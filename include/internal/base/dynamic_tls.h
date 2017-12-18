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
 * Dynamic thread-local (aka thread-specific) storage allocation
 */
#ifndef __BASE_DYNAMIC_TLS_H__
#define __BASE_DYNAMIC_TLS_H__

#include <stdint.h>

namespace s1
{
  namespace dynamic_tls
  {
    typedef unsigned int Handle;
    static const Handle InvalidHandle = ~0u;

    /**
     * Destructor function for a thread's value in a TLS slot.
     * \param value Value contained in TLS slot for a thread.
     * \param context Destructor context value provided together with
     *   destructor function.
     * \remarks There is no guaranteed ordering between the
     *  destructor function called per-thread and the destructor
     *  function passed to Free(); per-thread destructors may be
     *  called even after Free() returned! You can, however, rely on
     *  only one of the destructors being called for a value.
     */
    typedef void (*DestructorFunc)(void* value, uintptr_t context);

    /**
     * Allocate a TLS slot.
     * \param dtor Optional destructor function. Called upon thread exit with
     *  the thread's value for the slot (if some was set using SetValue()).
     *  The destructor is executed on the thread that is exiting.
     * \param dtorContext Optional "context" argument for destructor function.
     * \throws If registration failed.
     * \remarks See DestructorFunc remarks.
     */
    Handle Alloc (DestructorFunc dtor = nullptr, uintptr_t dtorContext = 0);

    /**
     * Free a TLS slot.
     * \param slot The handle value identifying the slot.
     * \param dtor Optional destructor function. Called with any remaining TLS
     *  slot values set using SetValue(), including those of \em other threads.
     *  However, the destructor is executed on the \em current thread.
     * \param dtorContext Optional "context" argument for destructor function.
     * \remarks See DestructorFunc remarks.
     */
    void Free (Handle slot, DestructorFunc dtor = nullptr, uintptr_t dtorContext = 0);

    /**
     * Set TLS slot value for current thread.
     * \param slot The handle value identifying the slot.
     * \param value New value for current thread.
     */
    void SetValue (Handle slot, void* value);

    /**
     * Get TLS slot value for current thread.
     * \param slot The handle value identifying the slot.
     * \returns \nullptr if no value was set previously.
     */
    void* GetValue (Handle slot);
  } // namespace dynamic_tls
} // namespace s1

#endif // __BASE_DYNAMIC_TLS_H__
