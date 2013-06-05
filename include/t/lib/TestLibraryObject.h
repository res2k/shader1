#ifndef __TESTLIBRARYOBJECT_H__
#define __TESTLIBRARYOBJECT_H__

#include "s1/Library.h"
#include "s1/LibraryObject.h"
#include "s1/Object.h"
#include "s1/Ptr.h"

#define TEST_MAKE_NAME(N) (N, (namespace cxxapi { class N; }, ::cxxapi::N))
#define TYPE_INFO_TestLO   (TEST_MAKE_NAME(TestLO), S1TYPE_INFO_s1_LibraryObject)

S1TYPE_DECLARE(TYPE_INFO_TestLO);

TestLO* CreateTestLO (s1_Library* lib);

#include "s1/warn_off.h"

namespace cxxapi
{
  class TestLO : public s1::cxxapi::LibraryObject
  {
  public:
    typedef s1::Ptr<TestLO> Pointer;

    static Pointer Create (s1::cxxapi::Library* lib)
    {
      ::TestLO* p (CreateTestLO (lib));
      return s1::TransferRefPtr<TestLO> (p);
    }
  };
} // namespace cxxapi

#include "s1/warn_on.h"

#endif // __TESTLIBRARYOBJECT_H__
