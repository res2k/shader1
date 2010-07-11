#include "base/common.h"

#include <fstream>
#include <iostream>

#include "base/UnicodeStream.h"
#include "codegen/LatexGenerator.h"
#include "intermediate/Exception.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"

#include "ErrorHandler.h"

using namespace s1;

int main (const int argc, const char* const argv[])
{
  if (argc < 2)
  {
    std::cout << "Syntax: " << argv[0] << " [input filename]" << std::endl;
    return 1;
  }
  
  ErrorHandler errorHandler;
  std::ifstream inputFile (argv[1], std::ios_base::in | std::ios_base::binary);
  UnicodeStream uniStream (inputFile, "utf-8");
  // Skip BOM
  if (*uniStream == 0xFEFF) ++uniStream;
  
  Lexer lexer (uniStream, errorHandler);
  
  intermediate::IntermediateGeneratorSemanticsHandler intermediateHandler;
  intermediateHandler.SetEntryFunction ("main");
  Parser parser (lexer, intermediateHandler, errorHandler);
  try
  {
    parser.Parse ();
  }
  catch (intermediate::Exception& e)
  {
    errorHandler.IntermediateError (e.GetCode());
  }
  
  codegen::LatexGenerator codegen;
  codegen::StringsArrayPtr progOutput (codegen.Generate (intermediateHandler.GetProgram ()));
  
  for (size_t i = 0; i < progOutput->Size(); i++)
  {
    std::cout << progOutput->Get (i) << std::endl;
  }
  
  return 0;
}
