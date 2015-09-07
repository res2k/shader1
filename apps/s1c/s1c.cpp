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
#include "s1/CompiledProgram.h"
#include "s1/Error.h"
#include "s1/Library.h"
#include "s1/Options.h"

#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>
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

class CommandLineOptions
{
public:
  typedef std::string arg_string_type;
  typedef std::vector<arg_string_type> arg_string_vec;

  arg_string_type inputFileName;
  arg_string_type entryName;
  arg_string_type backendStr;
  bool noSplit;
  typedef boost::unordered_map<std::string, s1::InputFrequency> ParamMap;
  ParamMap paramFlags;
  typedef boost::unordered_map<std::string, size_t> ParamArraySizeMap;
  ParamArraySizeMap paramArraySizes;

  CommandLineOptions () : noSplit(false) {}
  boost::optional<int> Parse (const int argc, const char* const argv[],
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
        bpo::value<arg_string_vec> ()->composing()->value_name("<flag>")
        ->implicit_value(arg_string_vec (1), *defaultOptStr),
        "Specify optimization options")
      ("entry", bpo::value<arg_string_type> (&entryName)->value_name("<function>")->default_value (defaultEntry),
        "Set program entry function")
      ("backend", bpo::value<arg_string_type> (&backendStr)->value_name("<backend>")->default_value (defaultBackend),
        "Select backend (target language)");
    bpo::options_description param_desc ("Input parameter options");
    param_desc.add_options ()
      ("param-uniform", bpo::value<arg_string_vec> ()->composing()->value_name("<param>"),
        "Specify a parameter to be uniform input")
      ("param-vertex", bpo::value<arg_string_vec> ()->composing()->value_name("<param>"),
        "Specify a parameter to be per-vertex input")
      ("param-size", (new ParamArraySizeOptionValue)->value_name("<param> <size>"),
        "Specify size of an array parameter");
    bpo::options_description special_desc ("Special options");
    special_desc.add_options ()
      ("nosplit", bpo::bool_switch(&noSplit),
        "Skip program splitting step");
    bpo::options_description input_files_desc;
    input_files_desc.add_options ()
      ("input-file", bpo::value<arg_string_type> (&inputFileName)->required());

    bpo::options_description all_options;
    all_options.add (input_desc).add (compile_desc).add (param_desc).add (special_desc).add (input_files_desc);
    bpo::options_description visible_options;
    visible_options.add (input_desc).add (compile_desc).add (param_desc).add (special_desc);

    bpo::positional_options_description p;
    p.add("input-file", -1);

    boost::optional<bpo::parsed_options> options;
    bpo::variables_map vm;
    try
    {
      options = boost::in_place<bpo::parsed_options> (bpo::command_line_parser (argc, argv).
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
      boost::unordered_set<std::string> paramsWarnedFlag;
      boost::unordered_set<std::string> paramsWarnedSize;
      BOOST_FOREACH (const bpo::option& option, options->options)
      {
        {
          const std::string* paramName = 0;
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
                && (paramsWarnedFlag.find (*paramName) != paramsWarnedFlag.end ()))
            {
              std::cerr << "Multiple type specifications for parameter: " << *paramName << std::endl;
              paramsWarnedFlag.insert (*paramName);
            }
            paramFlags[*paramName] = paramFreq;
          }
        }
        if (option.string_key == "param-size")
        {
          const std::string& paramName = option.value[0];
          ParamArraySizeMap::const_iterator prevSize = paramArraySizes.find (paramName);
          if ((prevSize != paramArraySizes.end ())
              && (paramsWarnedSize.find (paramName) != paramsWarnedSize.end ()))
          {
            std::cerr << "Multiple array size specifications for parameter: " << paramName << std::endl;
            paramsWarnedSize.insert (paramName);
          }
          boost::optional<unsigned long> arraySize (boost::convert<unsigned long> (option.value[1].c_str(), boost::cnv::spirit ()));
          if (!arraySize)
          {
            throw std::runtime_error ((boost::format ("Invalid array size '%2%' for parameter '%1%'")
                                       % option.value[0] % option.value[1]).str ());
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
              compilerOpts->SetOptFlagFromStr (optArg.c_str ());
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

  static void PrintSyntax (const char* execName, const bpo::options_description& options)
  {
    std::cout << "Usage: " << execName << " [options] <input filename>" << std::endl;
    std::cout << options << std::endl;
  }
private:
  class ParamArraySizeOptionValue : public bpo::typed_value<arg_string_vec>
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

int main (const int argc, const char* const argv[])
{
  Ptr<Library> lib;
  ResultCode libErr (Library::Create (lib));
  if (!S1_SUCCESSFUL(libErr))
  {
    // TODO: Print error code
    return 2;
  }
  
  Options::Pointer compilerOpts = lib->CreateOptions ();
  compilerOpts->SetOptLevel (defaultOptimizationLevel);

  CommandLineOptions options;
  boost::optional<int> result = options.Parse (argc, argv, compilerOpts);
  if (result) return *result;
  
  Backend::Pointer compilerBackend (lib->CreateBackend (options.backendStr.c_str()));
  if (!compilerBackend)
  {
    std::cerr << "Invalid backend: " << options.backendStr << std::endl;
    return 2;
  }
  
  std::string sourceStr;
  try
  {
    ifstream inputFile (options.inputFileName);
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
    std::cerr << "Error creating program" << std::endl;
    return 3;
  }
  compilerProg->SetEntryFunction (options.entryName.c_str());
  compilerProg->SetOptions (compilerOpts);
  
  BOOST_FOREACH(const CommandLineOptions::ParamMap::value_type& paramFlag, options.paramFlags)
  {
    compilerProg->SetInputFrequency (paramFlag.first.c_str(), paramFlag.second);
    // TODO: Error checking
  }
  BOOST_FOREACH(const CommandLineOptions::ParamArraySizeMap::value_type& paramSize, options.paramArraySizes)
  {
    compilerProg->SetInputArraySize (paramSize.first.c_str(), paramSize.second);
    // TODO: Error checking
  }

  if (options.noSplit)
  {
    CompiledProgram::Pointer compiled (
      compilerBackend->GenerateProgram (compilerProg, S1_TARGET_UNSPLIT));
    std::cout << compiled->GetString() << std::endl;
  }
  else
  {
    CompiledProgram::Pointer compiledVP (
      compilerBackend->GenerateProgram (compilerProg, S1_TARGET_VP));
    std::cout << compiledVP->GetString() << std::endl;
    CompiledProgram::Pointer compiledFP (
      compilerBackend->GenerateProgram (compilerProg, S1_TARGET_FP));
    std::cout << compiledFP->GetString () << std::endl;
  }
  return 0;
}
