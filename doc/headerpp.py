#!/usr/bin/env python
import fileinput
import re
import string
import sys

re_define = re.compile(r'^#define\s+(S1TYPE_INFO_\w*)\s+(.*)')
patterns = [(re.compile(r'^S1TYPE_DECLARE_FWD\((\w*)\);$'), r'struct \1;'),
            (re.compile(r'^S1TYPE_DECLARE\((\w*)\);$'), r'struct S1TYPE_TYPEINFO_NAME\1 {};')]

for line in fileinput.input():
  i = -1
  while (i >= -len(line)) and (line[i] in string.whitespace):
    i = i - 1
  trailing_space = line[i+1:]
  line = line[:i+1]
  i = 0
  while (i < len(line)) and (line[i] in string.whitespace):
    i = i + 1
  leading_space = line[:i]
  line = line[i:]
  defn_match = re_define.match(line)
  if defn_match:
    patterns.append ((re.compile (defn_match.group(1)), defn_match.group(2)))
  else:
    for pat in patterns:
      line = pat[0].sub (pat[1], line)
  sys.stdout.write(leading_space + line + trailing_space)
