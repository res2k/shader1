#ifndef __BASE_COMMON_H__
#define __BASE_COMMON_H__

/**\file
 * Common definitions
 */

#include "base/config.h"
#include <boost/cstdint.hpp> // must be include before any unicode/ headers to avoid macro conflict

/* (sizeof(x)*25)/10+1 is an approximation of the number of characters
  * needed to display x in decimal system. (x can be at most 256^sizeof(x).
  * You need log10(256^sizeof(x)) characters, becoming
  * sizeof(x)*log10(256). 25/10 is an (over-)approximation of log10(256).) */
static const size_t charsToFormatUint = (sizeof(unsigned int) * 25) / 10;
// Add 1 for '-'
static const size_t charsToFormatInt = (sizeof(unsigned int) * 25) / 10 + 1;

#include <unicode/unistr.h>

// Boost hash function for UnicodeString
namespace boost
{
  inline std::size_t hash_value (const UnicodeString& s)
  {
    return s.hashCode();
  }
}

/* Work around differing identifiers on MSVC
 * FIXME: Ensure to be private (i.e. invisible outside build) */
#ifdef _MSC_VER
  #define snprintf    _snprintf
  #define strcasecmp  _stricmp
#endif

// Debugging helpers
#include <assert.h>
#define S1_ASSERT_RET_VOID
#define S1_ASSERT(x, ret)            { assert (x); return ret; }
// TODO: Implement custom assertion messages
#define S1_ASSERT_MSG(x, msg, ret)   S1_ASSERT(x, ret)

#include "nullptr.h"

#endif // __BASE_COMMON_H__
