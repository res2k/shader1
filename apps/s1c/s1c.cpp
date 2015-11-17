/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

      1. The origin of this software must not be misrepresented; you must not
      claim that you wrote the original software. If you use this software
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

      2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original software.

      3. This notice may not be removed or altered from any source
      distribution.
*/

#include <iostream>

#include "s1/Backend.h"
#include "s1/BackendOptions.h"
#include "s1/CompiledProgram.h"
#include "s1/Library.h"
#include "s1/Options.h"
#include "s1/ResultCode.h"

#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/program_options.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/utility/in_place_factory.hpp>

#include <string.h>

using namespace s1;
namespace bpo = boost::program_options;

enum
{
  // Optimization level compiler picks by default
  defaultOptimizationLevel = 1,
  // Optimization level compiler picks if optimization is requested, but no particular options (-O)s
  defaultEnableOptimizationLevel = 2
};

/* Helper: Convert a wide string to UTF-8.
 * Hijack some program_options functions for that. */
static inline std::string to_utf (const std::wstring& orig)
{
  return boost::to_utf8 (orig);
}

static inline std::string to_local (const std::wstring& orig)
{
  return boost::to_local_8_bit (orig);
}

static inline std::string to_local (const char* orig)
{
  return orig;
}

static inline std::wstring to_wide (const std::string& orig)
{
  return boost::from_local_8_bit (orig);
}

static inline std::wstring to_wide (const wchar_t* orig)
{
  return orig;
}

// Get string for a result code
std::string ResultCodeString (ResultCode code)
{
  const char* s = GetResultCodeStr (code);
  if (s) return s;

  boost::cnv::spirit cnv;
  return std::string ("0x")
    + boost::convert<std::string> (static_cast<int> (code), // cast to work around convert<> build failure
        cnv (boost::cnv::parameter::base = boost::cnv::base::hex)).value();
}
// Get result string for last library error
std::string LastErrorString (Library* lib)
{
  std::string s (ResultCodeString (lib->GetLastError ()));
  const char* info = lib->GetLastErrorInfo ();
  if (info) s.append ((boost::format (": %1%") % info).str ());
  return s;
}

// Some abstractions for char vs wchar_t
#if defined(UNICODE)
#define MainFunc  wmain
typedef wchar_t ArgChar;
#else
#define MainFunc  main
typedef char ArgChar;
#endif

class CommandLineOptions
{
public:
  typedef std::wstring arg_string_type;
  typedef std::vector<arg_string_type> arg_string_vec;

  arg_string_type inputFileName;
  arg_string_type entryName;
  arg_string_type backendStr;
  arg_string_vec backendOptions;
  bool noSplit;
  typedef boost::unordered_map<arg_string_type, s1::InputFrequency> ParamMap;
  ParamMap paramFlags;
  typedef boost::unordered_map<arg_string_type, size_t> ParamArraySizeMap;
  ParamArraySizeMap paramArraySizes;

  CommandLineOptions () : noSplit(false) {}
  boost::optional<int> Parse (const int argc, const ArgChar* const argv[],
                              Options* compilerOpts)
  {
    const char* defaultEntry = "main";
    const char* defaultBackend = "cg";

    bpo::options_description input_desc ("General usage");
    input_desc.add_options ()
      // TODO: --help-backends
      // TODO: --help-optimization
      ("help", "print command line help");
    bpo::options_description compile_desc ("Compilation options");
    boost::optional<std::string> defaultOptStr (boost::convert<std::string> (int (defaultEnableOptimizationLevel), boost::cnv::spirit ()));
    compile_desc.add_options ()
      ("opt,O", 
        bpo::wvalue<arg_string_vec> ()->composing()->value_name("<flag>")
        ->implicit_value(arg_string_vec (1), *defaultOptStr),
        "Specify optimization options")
      ("entry", bpo::wvalue<arg_string_type> (&entryName)->value_name("<function>")->default_value (to_wide (defaultEntry), defaultEntry),
        "Set program entry function")
      ("backend", bpo::wvalue<arg_string_type> (&backendStr)->value_name("<backend>")->default_value (to_wide (defaultBackend), defaultBackend),
        "Select backend (target language)")
      ("backend-option", bpo::wvalue<arg_string_vec> (&backendOptions)->composing()->value_name("<option>"),
       "Specify backend-specific options");
      bpo::options_description param_desc ("Input parameter options");
    param_desc.add_options ()
      ("param-uniform", bpo::wvalue<arg_string_vec> ()->composing()->value_name("<param>"),
        "Specify a parameter to be uniform input")
      ("param-vertex", bpo::wvalue<arg_string_vec> ()->composing()->value_name("<param>"),
        "Specify a parameter to be per-vertex input")
      ("param-size", (new ParamArraySizeOptionValue)->value_name("<param> <size>"),
        "Specify size of an array parameter");
    bpo::options_description special_desc ("Special options");
    special_desc.add_options ()
      ("nosplit", bpo::bool_switch(&noSplit),
        "Skip program splitting step");
    bpo::options_description input_files_desc;
    input_files_desc.add_options ()
      ("input-file", bpo::wvalue<arg_string_type> (&inputFileName)->required());

    bpo::options_description all_options;
    all_options.add (input_desc).add (compile_desc).add (param_desc).add (special_desc).add (input_files_desc);
    bpo::options_description visible_options;
    visible_options.add (input_desc).add (compile_desc).add (param_desc).add (special_desc);

    bpo::positional_options_description p;
    p.add("input-file", -1);

    boost::optional<bpo::wparsed_options> options;
    bpo::variables_map vm;
    try
    {
      std::vector<std::wstring> args_vec;
      args_vec.reserve (argc-1);
      for (int i = 1; i < argc; i++)
      {
        args_vec.push_back (to_wide (argv[i]));
      }
      options = boost::in_place<bpo::wparsed_options> (bpo::wcommand_line_parser (args_vec).
                                                       options (all_options).positional (p).run ());
      bpo::store (*options, vm);
      bpo::notify (vm);
    }
    catch (bpo::required_option& /*e*/)
    {
      std::cerr << "No input file specified" << std::endl << std::endl;
      PrintSyntax (argv[0], visible_options);
      return 1;
    }
    catch (bpo::error& e)
    {
      std::cerr << e.what() << std::endl << std::endl;
      PrintSyntax (argv[0], visible_options);
      return 1;
    }

    if (vm.count("help"))
    {
      PrintSyntax (argv[0], visible_options);
      return 0;
    }

    try
    {
      boost::unordered_set<arg_string_type> paramsWarnedFlag;
      boost::unordered_set<arg_string_type> paramsWarnedSize;
      BOOST_FOREACH (const bpo::woption& option, options->options)
      {
        {
          const arg_string_type* paramName = 0;
          s1::InputFrequency paramFreq = S1_FREQ_INVALID;
          if (option.string_key == "param-uniform")
          {
            paramName = &(option.value[0]);
            paramFreq = S1_FREQ_UNIFORM;
          }
          else if (option.string_key == "param-vertex")
          {
            paramName = &(option.value[0]);
            paramFreq = S1_FREQ_VERTEX;
          }

          if (paramName != 0)
          {
            ParamMap::const_iterator prevFlag = paramFlags.find (*paramName);
            if ((prevFlag != paramFlags.end ()) && (prevFlag->second != paramFreq)
                && (paramsWarnedFlag.find (*paramName) == paramsWarnedFlag.end ()))
            {
              std::cerr << "Multiple type specifications for parameter: " << to_local (*paramName) << std::endl;
              paramsWarnedFlag.insert (*paramName);
            }
            paramFlags[*paramName] = paramFreq;
          }
        }
        if (option.string_key == "param-size")
        {
          const arg_string_type& paramName = option.value[0];
          ParamArraySizeMap::const_iterator prevSize = paramArraySizes.find (paramName);
          if ((prevSize != paramArraySizes.end ())
              && (paramsWarnedSize.find (paramName) == paramsWarnedSize.end ()))
          {
            std::cerr << "Multiple array size specifications for parameter: " << to_local (paramName) << std::endl;
            paramsWarnedSize.insert (paramName);
          }
          boost::optional<unsigned long> arraySize (boost::convert<unsigned long> (option.value[1].c_str(), boost::cnv::spirit ()));
          if (!arraySize)
          {
            throw std::runtime_error ((boost::format ("Invalid array size '%2%' for parameter '%1%'")
                                       % to_local (paramName) % to_local (option.value[1])).str ());
          }
          paramArraySizes[paramName] = boost::numeric_cast<size_t> (*arraySize);
        }
      }

      if (vm.count ("opt"))
      {
        arg_string_vec optArgs = vm["opt"].as<arg_string_vec> ();
        BOOST_FOREACH (const arg_string_type& optArg, optArgs)
        {
          if (optArg.empty ())
          {
            compilerOpts->SetOptLevel (defaultEnableOptimizationLevel);
          }
          else
          {
            boost::optional<int> optLevel (boost::convert<int> (optArg, boost::cnv::spirit ()));
            if (optLevel)
              compilerOpts->SetOptLevel (*optLevel);
            else
              compilerOpts->SetOptFlagFromStr (to_utf (optArg).c_str ());
          }
        }
      }
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl << std::endl;
      PrintSyntax (argv[0], visible_options);
      return 1;
    }

    return boost::optional<int> ();
  }

  static void PrintSyntax (const ArgChar* execName, const bpo::options_description& options)
  {
    std::cout << "Usage: " << to_local (execName) << " [options] <input filename>" << std::endl;
    std::cout << options << std::endl;
  }
private:
  class ParamArraySizeOptionValue : public bpo::typed_value<arg_string_vec, wchar_t>
  {
  public:
    ParamArraySizeOptionValue (std::vector<arg_string_type>* store_to = 0) : typed_value (store_to) {}
    unsigned min_tokens () const { return 2; }
    unsigned max_tokens () const { return 2; }
  };
};

/* Use this stream type since it provides better exception messages that
* the default stream ifstream */
typedef boost::iostreams::stream<boost::iostreams::file_descriptor_source> ifstream;

int MainFunc (const int argc, const ArgChar* const argv[])
{
  std::locale::global (std::locale (""));

  Ptr<Library> lib;
  ResultCode libErr (Library::Create (lib));
  if (!S1_SUCCESSFUL(libErr))
  {
    std::cerr << "Failed to initialize library: " << ResultCodeString (libErr) << std::endl;
    return 2;
  }
  
  Options::Pointer compilerOpts = lib->CreateOptions ();
  compilerOpts->SetOptLevel (defaultOptimizationLevel);

  CommandLineOptions options;
  boost::optional<int> result = options.Parse (argc, argv, compilerOpts);
  if (result) return *result;
  
  Backend::Pointer compilerBackend (lib->CreateBackend (to_utf (options.backendStr).c_str()));
  if (!compilerBackend)
  {
    std::cerr << "Failed to create backend '" << to_local (options.backendStr) << "': "
        << LastErrorString (lib) << std::endl;
    return 2;
  }

  BackendOptions::Pointer backendOptions;
  if (!options.backendOptions.empty ())
  {
    backendOptions = compilerBackend->CreateBackendOptions ();
    if (!backendOptions)
    {
      std::cerr << "Error creating backend options object: " << LastErrorString (lib) << std::endl;
    }
    else
    {
      BOOST_FOREACH (const CommandLineOptions::arg_string_type& backendOptStr,
                     options.backendOptions)
      {
        if (!backendOptions->SetFromStr (to_utf (backendOptStr).c_str ()))
        {
          std::cerr << "Error handling backend option '" << to_local (backendOptStr) << "': "
              << LastErrorString (lib) << std::endl;
        }
      }
    }
  }
  
  std::string sourceStr;
  try
  {
    ifstream inputFile (boost::filesystem::path (options.inputFileName));
    // Check for BOM
    {
      unsigned char bomBuf[3];
      inputFile.read ((char*)bomBuf, 3);
      if (inputFile.eof () || (bomBuf[0] != 0xEF) || (bomBuf[1] != 0xBB) || (bomBuf[2] != 0xBF))
      {
        // Not a BOM, reset
        inputFile.clear ();
        inputFile.seekg (0);
      }
    }
    inputFile.exceptions (std::ios_base::badbit | std::ios_base::failbit);
    {
      size_t startPos (inputFile.tellg ());
      inputFile.seekg (0, std::ios_base::end);
      size_t fileSize (inputFile.tellg ());
      inputFile.seekg (startPos, std::ios_base::beg);

      sourceStr.reserve (fileSize);
      while (fileSize > 0)
      {
        char buf[256];
        inputFile.read (buf, std::min (fileSize, sizeof (buf)));
        size_t nRead (inputFile.gcount ());
        if (nRead == 0) break;
        sourceStr.append (buf, nRead);
        fileSize -= nRead;
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 4;
  }
  
  Program::Pointer compilerProg (lib->CreateProgramFromString (sourceStr.c_str(), sourceStr.size()));
  if (!compilerProg)
  {
    std::cerr << "Error creating program: " << LastErrorString (lib) << std::endl;
    return 3;
  }
  compilerProg->SetEntryFunction (to_utf (options.entryName).c_str());
  compilerProg->SetOptions (compilerOpts);
  
  BOOST_FOREACH(const CommandLineOptions::ParamMap::value_type& paramFlag, options.paramFlags)
  {
    if (!compilerProg->SetInputFrequency (to_utf (paramFlag.first).c_str (), paramFlag.second))
    {
      std::cerr << "Error setting input type for '" << to_local (paramFlag.first) << "': "
        << LastErrorString (lib) << std::endl;
    }
  }
  BOOST_FOREACH(const CommandLineOptions::ParamArraySizeMap::value_type& paramSize, options.paramArraySizes)
  {
    if (!compilerProg->SetInputArraySize (to_utf (paramSize.first).c_str (), paramSize.second))
    {
      std::cerr << "Error setting input array size for '" << to_local (paramSize.first) << "': "
        << LastErrorString (lib) << std::endl;
    }
  }

  if (options.noSplit)
  {
    CompiledProgram::Pointer compiled (
      compilerBackend->GenerateProgram (compilerProg, S1_TARGET_UNSPLIT, backendOptions));
    if (!compiled)
    {
      std::cerr << "Failed to get unsplit program: " << LastErrorString (lib) << std::endl;
    }
    else
    {
      std::cout << compiled->GetString () << std::endl;
    }
  }
  else
  {
    CompiledProgram::Pointer compiledVP (
      compilerBackend->GenerateProgram (compilerProg, S1_TARGET_VP, backendOptions));
    if (!compiledVP)
    {
      std::cerr << "Failed to get vertex program: " << LastErrorString (lib) << std::endl;
    }
    else
    {
      std::cout << compiledVP->GetString () << std::endl;
    }
    CompiledProgram::Pointer compiledFP (
      compilerBackend->GenerateProgram (compilerProg, S1_TARGET_FP));
    if (!compiledFP)
    {
      std::cerr << "Failed to get fragment program: " << LastErrorString (lib) << std::endl;
    }
    else
    {
      std::cout << compiledFP->GetString () << std::endl;
    }
  }
  return 0;
}
