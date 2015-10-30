#!env python
# Generate some file from a error/result/codes XML file.

import argparse
import tempita
import xml.etree.ElementTree as ElementTree

parser = argparse.ArgumentParser(description='Generate some file from a error/result/codes XML file.')
parser.add_argument('input_file', metavar='XML-FILE', help='input XML file')
parser.add_argument('template_file', metavar='TEMPLATE-FILE', help='results template file')

args = parser.parse_args()
xmlparser = ElementTree.XMLParser()
xml_tree = ElementTree.parse(args.input_file, xmlparser)

template = tempita.Template.from_filename(args.template_file,
                                          namespace={},
                                          encoding='utf8')
components = xml_tree.getroot().getchildren()

last_empty = False
for line in template.substitute(components=components).splitlines():
    if not line:
        if not last_empty:
            print()
        last_empty = True
    else:
        print(line)
        last_empty = False
