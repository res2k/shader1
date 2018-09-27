/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <boost/test/unit_test.hpp>

#include "base/common.h"
#include "base/uc/SimpleBufferStreamSource.h"
#include "base/uc/Stream.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceOp/SequenceOp.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "splitter/ProgramSplitter.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"
#include "../../intermediate/t/CommonChecks.h"

BOOST_AUTO_TEST_SUITE(SplitterNestedBlock)

BOOST_AUTO_TEST_CASE(NestedBlock)
{
  using namespace s1::parser;
  
  std::string inStr (
    "void main()"
    "{"
      "float x;"
      "{"
        "x = 0.5;"
      "}"
      "float y = x;"
    "}"
  );
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler  errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::intermediate::IntermediateGeneratorSemanticsHandler semanticsHandler;
  s1::Parser parser (lexer, semanticsHandler, errorHandler);
  semanticsHandler.CompleteProgram();
  
  s1::splitter::ProgramSplitter splitter;

  s1::intermediate::ProgramPtr prog = semanticsHandler.GetProgram ("main");
  splitter.SetInputProgram (prog);
  splitter.PerformSplit();

  for (int f = 0; f < s1::splitter::freqNum; f++)
  {
    s1::intermediate::ProgramPtr outProg = splitter.GetOutputProgram (f);
    for (size_t func = 0; func < outProg->GetNumFunctions(); func++)
    {
      s1::intermediate::ProgramFunctionPtr mainFunc = outProg->GetFunction (func);
      VerifyRegsAssignedOnce (mainFunc->GetBody());
      //VerifyRegsWrittenBeforeRead ();
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
