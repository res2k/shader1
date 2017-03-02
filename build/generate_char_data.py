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


# Character sets
def PrintSetRanges(out_file, split, name, bytes_per_char):
  if bytes_per_char == 1:
    char_type = 'uint8_t'
  elif bytes_per_char == 2:
    char_type = 'Char16'
  else:
    char_type = 'Char32'
  print('static const {0} ucd_{1}[{2} * 2] = {{'.format (char_type, name, len(split)), file=out_file)
  for c in split:
    print('\t{0}, {1},'.format (hex(c[0]), hex(c[1])), file=out_file)
  print("};", file=out_file)

OUTPUT_DIR = None

def ProcessSetRanges(ranges, name):
  print(name + ":", file=sys.stderr)
  out_file = open (os.path.join (OUTPUT_DIR, '{0}.inc'.format (name)), "w")
  print('// Generated on {0} from Unicode {1} data'.format(datetime.datetime.now(), args.ucver), file=out_file)
  max_cp = 0
  for r in ranges:
    max_cp = max(max_cp, r[0], r[1])
  bytes_per_char = 1
  if max_cp >= 0x10000:
    bytes_per_char = 4
  elif max_cp >= 0x100:
    bytes_per_char = 2
  queries = int(math.ceil(math.log(len(ranges), 2)))
  print("queries:", queries, file=sys.stderr)
  PrintSetRanges (out_file, ranges, name, bytes_per_char)

class Data_UI8_per_CP(object):
  type_str = 'uint8_t'

  def __init__(self, prop_map):
    self.prop_map = prop_map

  def WriteExtraMapDataDecl(self, out_file):
    return False

  def WriteExtraMapData(self, out_file):
    pass

  def StrForCP(self, cp):
    if cp in self.prop_map:
      val = self.prop_map[cp]
    else:
      val = 0
    return '{0:>3}'.format (val)

  @staticmethod
  def RangeDataSize(cp_range, prop_map):
    return (cp_range[1] - cp_range[0] + 1)

class Data_CP_Seq(object):
  type_str = 'uint32_t'

  def __init__(self, prop_map):
    self.prop_map = prop_map

  def _UTF16count(self, seq):
    n = 0
    for cp in seq:
      n += 1 if cp < 0x10000 else 2
    return n
  
  def WriteExtraMapDataDecl(self, out_file):
    self.ofs_for_cp = {}
    n = 0
    for cp in sorted(self.prop_map.keys()):
      seq = self.prop_map[cp]
      if len(seq) == 1: continue
      self.ofs_for_cp[cp] = n
      n += self._UTF16count(seq)
    print('\tChar16 seqdata[{0}];'.format (n), file=out_file)
    return True

  def _UTF16convert(self, seq):
    seq_u16 = []
    for cp in seq:
      if cp < 0x10000:
        seq_u16.append(cp)
      else:
        cp -= 0x10000
        seq_u16.append(0xd800 | (cp >> 10))
        seq_u16.append(0xdc00 | (cp & 0x3ff))
    return seq_u16

  def WriteExtraMapData(self, out_file):
    print('\t{', file=out_file)
    for cp in sorted(self.prop_map.keys()):
      seq = self.prop_map[cp]
      if len(seq) == 1: continue
      s = ""
      for seq_cp in self._UTF16convert(seq):
        s = s + '{0}, '.format (hex (seq_cp))
      print('\t\t{0}'.format (s.rstrip()), file=out_file)
    print('\t},', file=out_file)

  def StrForCP(self, cp):
    if cp in self.prop_map:
      seq = self.prop_map[cp]
      if cp in self.ofs_for_cp:
        val = self.ofs_for_cp[cp] | ((self._UTF16count(seq)-1) << 24)
      else:
        val = seq[0]
    else:
      val = 0
    return hex(val)

  @staticmethod
  def RangeDataSize(cp_range, prop_map):
    num_ui32 = 0
    for cp in range(cp_range[0], cp_range[1]):
      if not cp in prop_map:
        num_ui32 += 1
        continue
      mapped_seq = prop_map[cp]
      if len(mapped_seq) == 1:
        num_ui32 += 1
      else:
        num_ui32 += 1 + len(mapped_seq)
    return num_ui32 * 4

# Character maps
def PrintMapRanges(out_file, prop_map, ranges, name, bytes_per_char, datatype):
  if bytes_per_char == 1:
    char_type = 'uint8_t'
  elif bytes_per_char == 2:
    char_type = 'Char16'
  else:
    char_type = 'Char32'

  data_size = 0
  for c in ranges:
    data_size = data_size + c[1] - c[0] + 1

  save_data = datatype (prop_map)

  # Map data struct
  print('static const struct _ucd_{0}'.format (name), file=out_file)
  print('{', file=out_file)
  print('\t{0} key[{1}*2];'.format (char_type, len(ranges)), file=out_file)
  print('\tunsigned int idx[{0}];'.format (len(ranges)), file=out_file)
  print('\t{0} data[{1}];'.format (datatype.type_str, data_size), file=out_file)
  have_extra_data = save_data.WriteExtraMapDataDecl(out_file)
  print('}} ucd_{0} = {{'.format (name), file=out_file)

  print('\t{', file=out_file)
  for c in ranges:
    print('\t\t{0}, {1},'.format (hex(c[0]), hex(c[1])), file=out_file)
  print('\t},', file=out_file)
  print('\t{', file=out_file)
  i = 0
  for c in ranges:
    print('\t\t{0},'.format (i), file=out_file)
    i = i + c[1] - c[0] + 1
  print('\t},', file=out_file)
  print('\t{', file=out_file)
  for c in ranges:
    print('\t\t// {0} - {1}'.format (hex(c[0]), hex(c[1])), file=out_file)
    n = 0
    s = ""
    for cp in range(c[0], c[1]+1):
      if s and (n % 8 == 0):
        print('\t\t{0}'.format (s.rstrip()), file=out_file)
        s = ''
      s = s + save_data.StrForCP (cp) + ', '
      n = n + 1
    if s:
      print('\t\t{0}'.format (s.rstrip()), file=out_file)
  if have_extra_data:
    print('\t}, ', file=out_file)
  else:
    print('\t}', file=out_file)
  save_data.WriteExtraMapData(out_file)
  print('};', file=out_file)

def ProcessMap(prop_map, name, datatype):
  print(name + ":", file=sys.stderr)
  out_file = open (os.path.join (OUTPUT_DIR, '{0}.inc'.format (name)), "w")
  print('// Generated on {0} from Unicode {1} data'.format(datetime.datetime.now(), args.ucver), file=out_file)
  max_cp = 0
  char_ranges = Range()
  for cp in sorted(prop_map):
    max_cp = max(max_cp, cp)
    char_ranges.append ((cp, cp))
  bytes_per_char = 1
  if max_cp >= 0x10000:
    bytes_per_char = 4
  elif max_cp >= 0x100:
    bytes_per_char = 2
  max_bits = int(math.ceil(math.log(max_cp, 2)))
  min_size = 0x7fffffff
  min_b = 0
  min_ranges = []
  for b in range(0, max_bits+1):
    compressed_ranges = CompressRanges (char_ranges, b)
    queries = int(math.ceil(math.log(len(compressed_ranges), 2)))
    # For each range need at least two CPs and a pointer
    s = len(compressed_ranges) * (bytes_per_char * 2 + 4)
    for r in compressed_ranges:
      s = s + datatype.RangeDataSize (r, prop_map)
    print(b, queries, s, file=sys.stderr)
    if s < min_size:
      min_size = s
      min_b = b
      min_ranges = compressed_ranges
  print("min:", min_b, min_size, min_ranges, file=sys.stderr)
  PrintMapRanges (out_file, prop_map, min_ranges, name, bytes_per_char, datatype)


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


parser = argparse.ArgumentParser(description='Process UCD data')
parser.add_argument('-d', '--ucd', dest='ucd_dir', required=True, help='UCD directory')
parser.add_argument('-u', '--ucver', dest='ucver', required=True, help='Unicode version')
parser.add_argument('-o', '--out', dest='out_dir', required=True, help='Output directory')

args = parser.parse_args()

ranges_White_Space = Range()
ranges_XID_Start = Range()
ranges_XID_Continue = Range()
combining_class = {}
canonical_decomp = {}
ranges_NFD_QC_No = Range()

def HandleBaseProp(prop_info):
  if prop_info[1] == 'White_Space':
     ranges_White_Space.append(prop_info[0])

def HandleDerivedProp(prop_info):
  if prop_info[1] == 'XID_Start':
     ranges_XID_Start.append(prop_info[0])
  elif prop_info[1] == 'XID_Continue':
     ranges_XID_Continue.append(prop_info[0])

def HandleCCProp(prop_info):
  ch_range, prop_str = prop_info
  cc = int(prop_str)
  if not cc:
    return
  for ch in range(ch_range[0], ch_range[1]+1):
    combining_class[ch] = cc

def HandleUnicodeData(prop_info):
  ch = prop_info[0]
  decomp = prop_info[5]
  if not decomp: return
  # Ignore compatibility mappings
  if decomp[0] == '<': return
  seq = []
  for cp_str in decomp.split(' '):
    if not cp_str: continue
    cp = int(cp_str, 16)
    seq.append (cp)
  canonical_decomp[ch[0]] = seq

def RecursivelyResolveDecompositions():
  global canonical_decomp
  do_resolve = True
  while do_resolve:
    do_resolve = False
    new_canonical_decomp = {}
    for cp, decomp in canonical_decomp.items():
      new_decomp = []
      for dcp in decomp:
        if dcp in canonical_decomp:
          new_decomp += canonical_decomp[dcp]
          do_resolve = True
        else:
          new_decomp.append (dcp)
      new_canonical_decomp[cp] = new_decomp
    canonical_decomp = new_canonical_decomp

def HandleDerivedNormProp(prop_info):
  if prop_info[1] != "NFD_QC": return
  ranges_NFD_QC_No.append(prop_info[0])

ParseProperties (LocateUCDData (args.ucd_dir, [], "PropList.txt"), HandleBaseProp)
ParseProperties (LocateUCDData (args.ucd_dir, ['extracted'], "DerivedCoreProperties.txt"), HandleDerivedProp)
ParseProperties (LocateUCDData (args.ucd_dir, ['extracted'], "DerivedCombiningClass.txt"), HandleCCProp)
ParseProperties (LocateUCDData (args.ucd_dir, [], "UnicodeData.txt"), HandleUnicodeData)
ParseProperties (LocateUCDData (args.ucd_dir, [], "DerivedNormalizationProps.txt"), HandleDerivedNormProp)

OUTPUT_DIR = args.out_dir
ProcessSetRanges (ranges_White_Space, "White_Space")
ProcessSetRanges (ranges_XID_Start, "XID_Start")
ProcessSetRanges (ranges_XID_Continue, "XID_Continue")
ProcessMap (combining_class, "CombiningClass", Data_UI8_per_CP)
ProcessSetRanges (ranges_NFD_QC_No, "NFD_QC_No")

RecursivelyResolveDecompositions()
ProcessMap (canonical_decomp, "CanonicalDecomp", Data_CP_Seq)
