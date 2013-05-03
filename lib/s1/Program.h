/**\file
 * Wrapper to provide public API program objects
 */
#ifndef __S1_CXXAPI_PROGRAM_HPP__
#define __S1_CXXAPI_PROGRAM_HPP__

#include "s1/Program.h"

#include "base/LibraryObject.h"

#include "compiler/Backend.h"
#include "compiler/Program.h"
#include "compiler/Options.h"

#include <boost/unordered_map.hpp>

namespace s1
{
  namespace api_impl
  {
    class Program : public s1::LibraryObject
    {
    protected:
      Compiler& compiler;
      s1::Compiler::OptionsPtr options; // TODO: Ideally, this would be copy-on-write
      std::string source;
      std::string entryFunction;
      typedef boost::unordered_map<std::string, s1::splitter::Frequency> InputFreqMapType;
      InputFreqMapType inputFreqMap;
      typedef boost::unordered_map<std::string, size_t> InputSizeMapType;
      InputSizeMapType inputSizeMap;
      
      Compiler::ProgramPtr wrapped_program;
      void Dirty();
    public:
      Program(s1::Library* lib, Compiler& compiler, const std::string& source)
        : LibraryObject (lib),
          compiler (compiler),
          options (new s1::Compiler::Options (lib)),
          source (source),
          entryFunction ("main")
      {
      }

      Compiler::Backend::ProgramPtr GetCompiledProgram (const Compiler::BackendPtr& backend,
                                                        Compiler::Backend::CompileTarget target);

      /**\name Methods mapping to public API
       * @{ */
      s1_ErrorCode SetOptions (const s1::Compiler::OptionsPtr& options);
      const s1::Compiler::OptionsPtr& GetOptions() const;
      
      s1_ErrorCode SetEntry (const char* entry);
      s1_ErrorCode GetEntry (const char*& entry) const;
      
      s1_ErrorCode SetInputFrequency (const char* param, s1_InputFrequency freq);
      s1_ErrorCode GetInputFrequency (const char* param, s1_InputFrequency& freq) const;

      s1_ErrorCode SetInputArraySize (const char* param, size_t size);
      s1_ErrorCode GetInputArraySize (const char* param, size_t& size) const;
      /** @} */
    };
  } // namespace api_impl
} // namespace s1

#endif // __S1_CXXAPI_PROGRAM_HPP__