/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef TESTDIAGNOSTICSHANDLER_H_
#define TESTDIAGNOSTICSHANDLER_H_

#include "diagnostics/Handler.h"

class TestDiagnosticsHandler : public s1::diagnostics::Handler
{
public:
  void SemanticErrorImpl (unsigned int code) override { semanticError.code = code; }

  struct {
    unsigned int code = 0;
  } semanticError;
};

#endif // TESTDIAGNOSTICSHANDLER_H_
