#include "base/common.h"

#include "TestLibraryObject.h"

#include "base/Library.h"
#include "base/LibraryObject.h"
#include "base/Object.h"

namespace impl
{
  class TestLO : public s1::LibraryObject
  {
  public:
    TestLO (s1::Library* lib) : LibraryObject (lib) {}
  };
} // namespace impl

TestLO* CreateTestLO (s1_Library* lib)
{
  impl::TestLO* new_obj (new impl::TestLO (s1::EvilUpcast<s1::Library> (lib)));
  new_obj->AddRef();
  return new_obj->DowncastEvil< ::TestLO> ();
}
