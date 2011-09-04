#ifndef __COMPILER_COMPILER_H__
#define __COMPILER_COMPILER_H__

#include <boost/shared_ptr.hpp>

namespace s1
{
  // "Friendly" class for compiler controls
  class Compiler
  {
  public:
    class Options;
    typedef boost::shared_ptr<Options> OptionsPtr;
    
    OptionsPtr CreateOptions ();
    
    class Backend;
    typedef boost::shared_ptr<Backend> BackendPtr;
    
    BackendPtr CreateBackendCg ();
    
    class Program;
    typedef boost::shared_ptr<Program> ProgramPtr;
    
    ProgramPtr CreateProgram (const OptionsPtr& compilerOptions,
			      std::istream& input);
    
    // Methods to specify input
    // Optimization getters/setters
    // Methods to query messages/warnings/errors
    // Methods to set input parameter frequency
    // (Command line parser?)
    // Probably also: query input parameters
    // Getting output (using a "Backend"?)
    // Also need to query: main function names,
    //   mappings between original and output param names,
    //   (...Backend dependent)
  
    // -> Also "Program" object for these aspects?
  };
} // namespace s1

#endif // __COMPILER_COMPILER_H__
