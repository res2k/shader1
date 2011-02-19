#ifndef __DEPROTECTCTOR_H__
#define __DEPROTECTCTOR_H__

namespace s1
{
  template<typename Superclass>
  class DeprotectCtor : public Superclass
  {
  public:
    DeprotectCtor () {}
    
    template<typename T1>
    DeprotectCtor (T1 a1) : Superclass (a1) {}
    template<typename T1, typename T2>
    DeprotectCtor (T1 a1, T2 a2) : Superclass (a1, a2) {}
    template<typename T1, typename T2, typename T3>
    DeprotectCtor (T1 a1, T2 a2, T3 a3) : Superclass (a1, a2, a3) {}
  };
} // namespace s1

#endif // __DEPROTECTCTOR_H__
