#!env python
# Generate some file from a error/result/codes XML file.

import argparse
import sys
import tempita
import xml.etree.ElementTree as ElementTree

parser = argparse.ArgumentParser(description='Generate some file from a error/result/codes XML file.')
parser.add_argument('input_file', metavar='XML-FILE', nargs='*', help='input XML file')
parser.add_argument('--template', '-t', dest='template_file', metavar='TEMPLATE-FILE', help='results template file', required=True)
parser.add_argument('--root', '-r', metavar='ROOT-ID', help='identifier of root data container used in template')
parser.add_argument('-D', dest='defines', metavar='KEY=VALUE', nargs='*', help='additional values passed to template')

root_children = []
args = parser.parse_args()
if args.root:
    for input_file in args.input_file:
        xmlparser = ElementTree.XMLParser()
        xml_tree = ElementTree.parse(input_file, xmlparser)
        root_children += xml_tree.getroot().getchildren()
else:
    # No root, no input files: Just substitutes defines passed via -D
    if len(args.input_file) > 0:
        print("Input XML files but no root identifier given. That doesn't make sense", file=sys.stderr)
        exit(1)

template = tempita.Template.from_filename(args.template_file,
                                          namespace={},
                                          encoding='utf8')

last_empty = False
subst_args = {}
if args.root: subst_args[args.root] = root_children
if args.defines:
    for d in args.defines:
        key, val = d.split('=', 1)
        subst_args[key] = val
for line in template.substitute(subst_args).splitlines():
    if not line.strip():
        if not last_empty:
            print()
        last_empty = True
    else:
        print(line)
        last_empty = False
