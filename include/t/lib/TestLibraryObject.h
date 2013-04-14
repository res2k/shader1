#ifndef __TESTLIBRARYOBJECT_H__
#define __TESTLIBRARYOBJECT_H__

#include "s1/Library.h"
#include "s1/LibraryObject.h"
#include "s1/Object.h"
#include "s1/Ptr.h"

#define TYPE_INFO_TestLO   (TestLO, S1TYPE_INFO_s1_LibraryObject)

S1TYPE_DECLARE(TYPE_INFO_TestLO);

TestLO* CreateTestLO (s1_Library* lib);

#include "s1/warn_off.h"

namespace cxxapi
{
  class TestLO : public s1::cxxapi::Rebadge<TestLO, ::TestLO, s1::cxxapi::LibraryObject>
  {
  public:
    typedef s1::Ptr<TestLO> Pointer;

    static Pointer Create (s1::cxxapi::Library* lib)
    {
      ::TestLO* p (CreateTestLO (lib->Cpointer()));
      return Pointer (FromC (p), Pointer::TakeReference ());
    }
  };
} // namespace cxxapi

#include "s1/warn_on.h"

#endif // __TESTLIBRARYOBJECT_H__
