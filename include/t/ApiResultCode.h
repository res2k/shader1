/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <cxxtest/TestSuite.h>

#include "s1/ResultCode.h"

class ApiResultCodeTestSuite : public CxxTest::TestSuite 
{
public:
  void testResultCodeDescr (void)
  {
    // Some actual result codes
    TS_ASSERT_DIFFERS(s1_get_result_code_str (S1_SUCCESS), nullptr);
    TS_ASSERT_DIFFERS(s1_get_result_code_str (S1_E_FAILURE), nullptr);
    TS_ASSERT_DIFFERS(s1_get_result_code_str (S1_E_OUT_OF_MEMORY), nullptr);
    // Non-existing result code
    TS_ASSERT_EQUALS(s1_get_result_code_str (0x12345678), nullptr);
  }
};
