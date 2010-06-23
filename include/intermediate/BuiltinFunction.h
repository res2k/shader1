#ifndef __INTERMEDIATE_BUILTINFUNCTION_H__
#define __INTERMEDIATE_BUILTINFUNCTION_H__

namespace s1
{
  namespace intermediate
  {
    enum BuiltinFunction
    {
      // Vector builtins
      dot,
      cross,
      normalize,
      length,
      // Matrix builtins
      mul,
      // Texture builtins
      tex1D,
      tex2D,
      tex3D,
      texCUBE,
      // Misc
      min,
      max,
      pow
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_BUILTINFUNCTION_H__
