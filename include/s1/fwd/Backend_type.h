/**\file
 * S1 compiler backend (type definition)
 */
#ifndef __S1_BACKEND_TYPE_H__
#define __S1_BACKEND_TYPE_H__

#include "s1/warn_off.h"

#include "s1/LibraryObject.h"

#define S1TYPE_INFO_s1_Backend   (s1_Backend, S1TYPE_INFO_s1_LibraryObject)
S1TYPE_DECLARE(S1TYPE_INFO_s1_Backend);

#include "s1/warn_on.h"

#endif // __S1_BACKEND_TYPE_H__
