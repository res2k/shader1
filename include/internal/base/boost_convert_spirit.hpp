/*
Shader1
Copyright (c) 2015 Frank Richter


This library is free software; you can redistribute it and/or
modify it under the terms of the wxWindows Library Licence.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Please see the wxWindows Library Licence for more details.
The licence texts can be found in the source distribution files
LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

// Wrapper file to disable warning C4828
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4828 )
#endif

#include <boost/convert/spirit.hpp>

#if defined(_MSC_VER)
#pragma warning( pop )
#endif
