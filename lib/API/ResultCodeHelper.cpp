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

#include "base/common.h"
#include "ResultCodeHelper.h"

#include "base/format/Formatter.h"
#include "base/Mutex.h"

#include <boost/unordered_map.hpp>

#include <memory>

#include "base/format/Formatter.ipp"
#include "base/format/std_string.h"

namespace s1
{
  namespace detail
  {
    struct ExtraDescrCache
    {
      Mutex mutex;
      boost::unordered_map<s1_ResultCode, std::unique_ptr<char[]>> descr;
    };

    static std::once_flag extraDescrCacheInit;
    static std::unique_ptr<ExtraDescrCache> extraDescrCache;

    const char* GetExtendedErrorDescr (s1_ResultCode code, const char* baseDescr, const char* extDescr)
    {
      unsigned int extra = S1_GET_EXTRA (code);
      if (extra == 0)
      {
        return baseDescr;
      }
      else
      {
        std::call_once (extraDescrCacheInit, []() { extraDescrCache.reset (new ExtraDescrCache); });
        std::lock_guard<Mutex> lock_extraDescrCache (extraDescrCache->mutex);
        auto cacheIt = extraDescrCache->descr.find (code);
        if (cacheIt == extraDescrCache->descr.end ())
        {
          format::Formatter formatExtDescr (extDescr);
          std::string formattedDescr = formatExtDescr.to<std::string> (extra - 1); // The _N macro generator always adds 1
          size_t n = formattedDescr.size ();
          std::unique_ptr<char[]> storeDescr (new char[n + 1]);
          memcpy (storeDescr.get (), formattedDescr.c_str (), n + 1);
          cacheIt = extraDescrCache->descr.emplace (code, std::move (storeDescr)).first;
        }
        return cacheIt->second.get ();
      }
    }
  } // namespace detail
} // namespace s1
