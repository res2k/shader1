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

#ifndef __STRINGSUBSTITUTE_H__
#define __STRINGSUBSTITUTE_H__

typedef boost::unordered_map<std::string, std::string> StringStringMap;

static inline std::string StringSubstitute (const char* templ,
					    const StringStringMap& substMap)
{
  std::string result;
  char ch;
  while ((ch = *templ) != 0)
  {
    templ++;
    if (ch == '$')
    {
      ch = *templ++;
      if (ch == '$')
	result.append ("$");
      else
      {
	std::string varName;
	if (ch == '{')
	{
	  ch = *templ++;
	  while ((ch != '}') && (ch != 0))
	  {
	    char s[2] = {ch, 0};
	    varName.append (s);
	    ch = *templ++;
	  }
	  if (ch == '}') templ++;
	}
	else
	{
	  while ((ch != 0) && (isalnum (ch) || (ch == '_')))
	  {
	    char s[2] = {ch, 0};
	    varName.append (s);
	    ch = *templ++;
	  }
	  if (ch != 0) templ--;
	}
	if (!varName.empty())
	{
	  StringStringMap::const_iterator it = substMap.find (varName);
	  if (it != substMap.end())
	    result.append (it->second);
	}
      }
    }
    else
    {
      char s[2] = {ch, 0};
      result.append (s);
    }
  }
  return result;
}

#endif // __STRINGSUBSTITUTE_H__
