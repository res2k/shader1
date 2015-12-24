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

#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>

#include <memory>

#include "base/format/Formatter.tpp"
#include "base/format/std_string.h"

namespace s1
{
  namespace detail
  {
    struct ExtraDescrCache
    {
      boost::mutex mutex;
      boost::unordered_map<s1_ResultCode, std::unique_ptr<char[]>> descr;
    };

    static boost::once_flag extraDescrCacheInit;
    static std::unique_ptr<ExtraDescrCache> extraDescrCache;

    format::StaticFormatter FormatExtDescr ("{0} ({1})");

    const char* GetExtendedErrorDescr (s1_ResultCode code, const char* baseDescr)
    {
      unsigned int extra = S1_GET_EXTRA (code);
      if (extra == 0)
      {
        return baseDescr;
      }
      else
      {
        boost::call_once (extraDescrCacheInit, []() { extraDescrCache.reset (new ExtraDescrCache); });
        boost::lock_guard<boost::mutex> lock_extraDescrCache (extraDescrCache->mutex);
        auto cacheIt = extraDescrCache->descr.find (code);
        if (cacheIt == extraDescrCache->descr.end ())
        {
          std::string formattedDescr;
          FormatExtDescr (formattedDescr, baseDescr, extra);
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
