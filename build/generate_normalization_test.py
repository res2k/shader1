#!/usr/bin/env python
from __future__ import division

import argparse
import datetime
import math
import os.path
import sys

def LocateUCDData(ucddir, subdirs, filename):
  fullpath = os.path.join (ucddir, '/'.join (subdirs), filename)
  candidate = fullpath
  while candidate:
    if len(subdirs) > 0:
      subdirs = subdirs[:-1]
      next_candidate = os.path.join (ucddir, '/'.join (subdirs), filename)
    else:
      next_candidate = None
    if os.path.isfile(candidate):
      return candidate
    candidate = next_candidate
  # If no candidate exists, return 'right' path (and let consumer complain)
  return fullpath


parser = argparse.ArgumentParser(description='Process UCD data',
                                 epilog='Generated C++ code is written to stdout')
parser.add_argument('-d', '--ucd', dest='ucd_dir', required=True, help='UCD directory')
parser.add_argument('-u', '--ucver', dest='ucver', required=True, help='Unicode version')
parser.add_argument('-o', '--out', dest='out_dir', required=True, help='Output directory')

args = parser.parse_args()

test_data = []
with open(LocateUCDData (args.ucd_dir, [], "NormalizationTest.txt"), "r") as f:
  for line in f:
    if line.startswith('@'): continue
    # Remove comment
    comment_pos = line.find ('#')
    if comment_pos >= 0: line = line[:comment_pos]
    line = line.strip()
    # Skip empty lines
    if not line: continue
    # Split line. Test data items are separated by ';'
    parts = line.split(';')
    sequences = [None] * len(parts)
    for i, p in enumerate(parts):
      seq = []
      for cp_str in p.split(' '):
        if not cp_str: continue
        cp = int(cp_str, 16)
        seq.append (cp)
      sequences[i] = seq
    test_data.append (sequences)

def FormatSeq(seq):
  s = ''
  for cp in seq:
    s = s + '{0}, '.format (hex (cp))
  return s + '0'

out_file = open (os.path.join (args.out_dir, 'NormalizationTestData.inc'), "w")
print >> out_file, '// Generated on {0} from Unicode {1} data'.format(datetime.datetime.now(), args.ucver)
test_items = []
n = 0
for item in test_data:
  # We only check NFD normalization, so skip any items were it's not different from the source
  if set(item[0]) == set(item[2]): continue
  print >> out_file, 'const Char32 normalizationTestData_source_{0}[] = {{ {1} }};'.format (n, FormatSeq (item[0]))
  print >> out_file, 'const Char32 normalizationTestData_nfd_{0}[] = {{ {1} }};'.format (n, FormatSeq (item[2]))
  test_items.append(n)
  n += 1
print >> out_file, 'struct NormalizationTestItem {\n\tconst Char32* source;\n\tconst Char32* nfd;\n};'
print >> out_file, 'static const NormalizationTestItem normalizationTestData[] = {'
for item in test_items:
  # We only check NFD normalization, so skip any items were it's not different from the source
  print >> out_file, '\t// {0}\n\t{{ normalizationTestData_source_{0}, normalizationTestData_nfd_{0} }},'.format (item)
print >> out_file, '};'
