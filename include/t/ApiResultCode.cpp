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

#include "s1/ResultCode.h"

BOOST_AUTO_TEST_SUITE(ApiResultCode)

BOOST_AUTO_TEST_CASE(ResultCodeDescr)
{
  // Some actual result codes
  BOOST_CHECK_NE(s1_get_result_code_str (S1_SUCCESS), nullptr);
  BOOST_CHECK_NE(s1_get_result_code_str (S1_E_FAILURE), nullptr);
  BOOST_CHECK_NE(s1_get_result_code_str (S1_E_OUT_OF_MEMORY), nullptr);
  // Non-existing result code
  BOOST_CHECK_EQUAL(s1_get_result_code_str (0x12345678), nullptr);
}

BOOST_AUTO_TEST_CASE(ResultCodeExtDescr)
{
  std::string base_descr (s1_get_result_code_str (S1_E_INVALID_ARG));
  std::string ext_descr (s1_get_result_code_str (S1_E_INVALID_ARG_N (0)));
  BOOST_CHECK_NE(base_descr, ext_descr);
}

BOOST_AUTO_TEST_SUITE_END()
