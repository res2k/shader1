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
