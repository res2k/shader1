#!/usr/bin/env python
from __future__ import division

import argparse
import datetime
import math
import os.path
import sys

class Range(object):
  def __init__(self):
    self.ranges = []

  def append(self, cp_tuple):
    if len(self.ranges) > 0:
      last = self.ranges[-1]
      if last[1] + 1 == cp_tuple[0]:
        new_tuple = (last[0], cp_tuple[1])
        self.ranges[-1] = new_tuple
        return
      elif last[1] >= cp_tuple[0]:
        new_tuple = (min(last[0], cp_tuple[0]),
                     max(last[1], cp_tuple[1]))
        self.ranges[-1] = new_tuple
        return
    self.ranges.append(cp_tuple)

  def __iter__(self):
    return iter(self.ranges)

  def __str__(self):
    return str(self.ranges)

  def __len__(self):
    return len(self.ranges)

  def __getitem__(self, key):
    return self.ranges[key]

def CompressRanges(ranges, right_bits):
  mask = (1 << right_bits) - 1
  mask_l = ~mask
  new_ranges = Range()
  for r in ranges:
    new_r = (r[0] & mask_l, r[1] | mask)
    new_ranges.append(new_r)
  return new_ranges

def FindInSortedRange(ranges, x, lo=0, hi=None):
  if hi is None:
    hi = len(ranges)
  if lo == hi:
    return lo
  m = (lo + hi) // 2
  if x < ranges[m][0]:
    return FindInSortedRange (ranges, x, lo, m)
  elif (m+1 < hi) and (x >= ranges[m+1][0]):
    return FindInSortedRange (ranges, x, m+1, hi)
  else:
    return m

def ParseProperties(filename, handler):
  with open(filename, "r") as f:
    for line in f:
      # Remove comment
      comment_pos = line.find ('#')
      if comment_pos >= 0: line = line[:comment_pos]
      line = line.strip()
      # Skip empty lines
      if not line: continue
      # Split line. Code points and properties are separated by ';'
      parts = line.split(';')
      for i, p in enumerate(parts):
        parts[i] = p.strip()
      # Might be a single code point, might be a range...
      codepoints = parts[0].split('..')
      # ...so always make sure it's a range
      if len(codepoints) == 1:
        codepoints[0] = int(codepoints[0], 16)
        codepoints.append(codepoints[0])
      else:
        codepoints[0] = int(codepoints[0], 16)
        codepoints[1] = int(codepoints[1], 16)
      parts[0] = tuple(codepoints)
      handler (parts)

def SplitRanges(ranges, right_bits):
  split = {}
  key_ranges = CompressRanges(ranges, right_bits)
  for r in ranges:
    for cp in range(r[0], r[1]+1):
      range_idx = FindInSortedRange (key_ranges, cp)
      assert(cp <= key_ranges[range_idx][1])
      key = key_ranges[range_idx]
      if not key in split:
        split[key] = ((key[1] - key[0] + 1 if right_bits > 0 else 0), [])
      split[key][1].append (cp)
  return split

def PrintRanges(out_file, split, name, bytes_per_char):
  if bytes_per_char == 1:
    char_type = 'uint8_t'
  elif bytes_per_char == 2:
    char_type = 'Char16'
  else:
    char_type = 'Char32'
  print >> out_file, 'static const {0} ucd_{1}[{2} * 2] = {{'.format (char_type, name, len(split))
  for c in split:
    print >> out_file, '\t{0}, {1},'.format (hex(c[0]), hex(c[1]))
  print >> out_file, "};"

OUTPUT_DIR = None

def ProcessRanges(ranges, name):
  print >> sys.stderr, name + ":"
  out_file = open (os.path.join (OUTPUT_DIR, 'ucd_{0}.inc'.format (name)), "w")
  print >> out_file, '// Generated on {0} from Unicode {1} data'.format(datetime.datetime.now(), args.ucver)
  max_cp = 0
  for r in ranges:
    max_cp = max(max_cp, r[0], r[1])
  bytes_per_char = 1
  if max_cp >= 0x10000:
    bytes_per_char = 4
  elif max_cp >= 0x100:
    bytes_per_char = 2
  queries = int(math.ceil(math.log(len(ranges), 2)))
  print >> sys.stderr, "queries:", queries
  PrintRanges (out_file, ranges, name, bytes_per_char)

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

ranges_White_Space = Range()
ranges_XID_Start = Range()
ranges_XID_Continue = Range()

def HandleBaseProp(prop_info):
  if prop_info[1] == 'White_Space':
     ranges_White_Space.append(prop_info[0])

def HandleDerivedProp(prop_info):
  if prop_info[1] == 'XID_Start':
     ranges_XID_Start.append(prop_info[0])
  elif prop_info[1] == 'XID_Continue':
     ranges_XID_Continue.append(prop_info[0])



ParseProperties (LocateUCDData (args.ucd_dir, [], "PropList.txt"), HandleBaseProp)
ParseProperties (LocateUCDData (args.ucd_dir, ['extracted'], "DerivedCoreProperties.txt"), HandleDerivedProp)

OUTPUT_DIR = args.out_dir
ProcessRanges (ranges_White_Space, "White_Space")
ProcessRanges (ranges_XID_Start, "XID_Start")
ProcessRanges (ranges_XID_Continue, "XID_Continue")
