#ifndef __BASE_NULLPTR_H__
#define __BASE_NULLPTR_H__

#if !defined(CXX_HAS_NULLPTR)

/*
  Based on "library implementation" in C++ Standards Committee paper 2431
  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf
*/
namespace std
{ 
  class nullptr_t
  {
  private:
    void operator&() const;

  public:
    template<typename T>
    operator T*() const
    {
      return 0;
    }

    template<class C, typename T>
    operator T C::*() const
    {
      return 0;
    }
  };
}

/* Hack around the "identifier 'nullptr' will become a keyword in C++0x"
 * warnings gcc 4.6 emits.
 * The "proper" way - using a #pragma GCC diagnostic - is, unfortunately,
 * broken: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=48914
 * Instead, to avoid use of that identifier, rename that identifier.
 */
#define nullptr s1_nullptr

using ::std::nullptr_t;
const nullptr_t nullptr = {};

#endif // !defined(CXX_HAS_NULLPTR)

#endif // __BASE_NULLPTR_H__
