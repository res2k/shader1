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

#ifndef __LEXER_LEXERERRORHANDLER_H__
#define __LEXER_LEXERERRORHANDLER_H__

namespace s1
{
  struct LexerErrorHandler
  {
    virtual ~LexerErrorHandler() {}
    
    /**
     * Handler called when an invalid input sequence is encountered.
     * An example would be an incomplete encoding sequence in UTF-8.
     */
    virtual void InputInvalidCharacter () {}
  };
} // namespace s1

#endif // __LEXER_LEXERERRORHANDLER_H__
