#!env python
# Generate some file from a error/result/codes XML file.

import argparse
import tempita
import xml.etree.ElementTree as ElementTree

parser = argparse.ArgumentParser(description='Generate some file from a error/result/codes XML file.')
parser.add_argument('input_file', metavar='XML-FILE', nargs='+', help='input XML file')
parser.add_argument('--template', '-t', dest='template_file', metavar='TEMPLATE-FILE', help='results template file', required=True)
parser.add_argument('--root', '-r', metavar='ROOT-ID', help='identifier of root data container used in template', required=True)

root_children = []
args = parser.parse_args()
for input_file in args.input_file:
    xmlparser = ElementTree.XMLParser()
    xml_tree = ElementTree.parse(input_file, xmlparser)
    root_children += xml_tree.getroot().getchildren()

template = tempita.Template.from_filename(args.template_file,
                                          namespace={},
                                          encoding='utf8')

last_empty = False
subst_args = { args.root: root_children }
for line in template.substitute(subst_args).splitlines():
    if not line.strip():
        if not last_empty:
            print()
        last_empty = True
    else:
        print(line)
        last_empty = False
