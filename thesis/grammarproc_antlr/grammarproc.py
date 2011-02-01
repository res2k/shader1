# -*- coding: latin-1 -*-
import sys
import antlr3
import antlr3.tree
import ANTLRv3_pyLexer as ANTLRv3_pyLexer_module
from ANTLRv3_pyLexer import *
from ANTLRv3_pyParser import ANTLRv3_pyParser
#from Eval import Eval

from optparse import OptionParser

options = OptionParser()
(parsed_options, args) = options.parse_args()

inFile = args[0]
outFile = args[1]

char_stream = antlr3.ANTLRInputStream(file (inFile, "r"), 'utf-8')
lexer = ANTLRv3_pyLexer(char_stream)
tokens = antlr3.CommonTokenStream(lexer)
parser = ANTLRv3_pyParser(tokens)
r = parser.grammarDef()

def tex_escape(oldstr):
  str = ''
  for char in oldstr:
    if char == '{':
      str = str + "\{"
    elif char == '}':
      str = str + "\}"
    elif char == '%':
      str = str + "\%"
    elif char == '&':
      str = str + "\&"
    elif char == '_':
      str = str + "\_"
    #elif char == '\\':
    #  str = str + "\\\\"
    elif char == '\\':
      str = str + "$\\backslash$"
    elif char == '~':
      str = str + "$\sim$"
    else:
      str = str + char
  return str

def tex_escape_label(oldstr):
  str = ''
  for char in unicode(tex_escape(oldstr)):
    if char == u'ö':
      str = str + "oe"
    elif char == u'ü':
      str = str + "ue"
    elif char == u'ä':
      str = str + "ae"
    elif char == u'Ö':
      str = str + "Oe"
    elif char == u'Ü':
      str = str + "Ue"
    elif char == u'Ä':
      str = str + "Ae"
    elif char in frozenset (['\\',  '{', '}']):
      str = str + "_"
    else:
      str = str + char
  return str

class DocSection(object):
    def __init__(self):
	self.name = None
	self.type = None
	self.contents = []
	self.rules = []
	self.type = []
	self.breakBeforeRules = False
	self.customRules = {}

currentDocSection = DocSection()
allSections = []

GRAMMAR_SPACE = "\Gspace"
DO_EXPAND = False
MERGE_ALTERNATIVES = False
MERGE_THRESHOLD = 3

def flushSections():
  global currentDocSection
  allSections.append (currentDocSection)
  currentDocSection = None

def format_nt_ref(ref):
  return "{\\scriptsize %s}" % ref;
  
def parseLine(line):
  str = ''
  currentLink = None
  for char in line:
    if currentLink != None:
      if char == '+':
	str = str + ("\\Gnt{%s}" % tex_escape (currentLink))
	str = str + format_nt_ref("(\\ref{%s})" % currentLink)
	currentLink = None
      else:
	currentLink = currentLink + char
    else:
      if char == '_':
	str = str + "\_"
      elif char == '+':
	currentLink = ""
      else:
	str = str + char
  return str

def parseDocs(docText):
    global currentDocSection
    global currentDocSubSection
    global currentDocSubSubSection
    global currentContainerSection
    global allSections
    global MERGE_ALTERNATIVES
    # Strip leading /**
    docText = docText[3:]
    # Strip trailing */
    docText = docText[:-2]
    # Go through lines
    lines = docText.split("\n")
    for line in lines:
	orgline = line
	line = line.strip()
	# Create new section if it starts with \section
	if line.startswith("\\section") or line.startswith("\\subsection") or line.startswith("\\subsubsection"):
	    breakBeforeRules = False
	    if line.startswith("\\section"):
		sectName = line[8:].strip()
		sectType = "subsection"
	    elif line.startswith("\\subsection"):
		sectName = line[11:].strip()
		sectType = "subsubsection"
	    elif line.startswith("\\subsubsection"):
		sectName = line[14:].strip()
		sectType = "paragraph"
		breakBeforeRules = True
	    newDocSection = DocSection()
	    newDocSection.name = sectName
	    newDocSection.type = sectType
	    newDocSection.breakBeforeRules = breakBeforeRules
	    flushSections()
	    currentDocSection = newDocSection
	elif line.startswith("\\customrule"):
	    ruleArgs = line[11:].strip().split(' ', 1)
	    currentDocSection.customRules[ruleArgs[0]] = ruleArgs[1]
	elif line.startswith("\\alt-merge-on"):
	    MERGE_ALTERNATIVES = True
	elif line.startswith("\\alt-merge-off"):
	    MERGE_ALTERNATIVES = False
	else:
	    currentDocSection.contents.append(parseLine(orgline.rstrip()))

# Documentation:
# - multiple sections
# - each sections has a list of rules
# - when parsing a rule, update docs state by parsing docs comment
# - _after_ that add rule to currect docs section

def getNodeStr(node):
    if node.token.input:
      return node.token.input.strdata[node.token.start:node.token.stop+1]
    return None

def reverseLookupType(type):
  for val in ANTLRv3_pyLexer_module.__dict__:
    if ANTLRv3_pyLexer_module.__dict__[val] == type:
      return val
  return None

def printChildren(node):
  for child in node.children:
    printNode(child)

def printChildrenWithIter(currentChild, childIter):
  child = currentChild
  while child:
    printNode(child)
    try:
      child = childIter.next()
    except StopIteration:
      child = None

def printGrammar(node):
  childIter = iter(node.children)
  child = childIter.next()
  while child.token.type != RULE:
    child = childIter.next()
  printChildrenWithIter(child, childIter)

def printRule(node):
  global currentDocSection
  global allSections
  prefix = []
  
  def concat_with_space(str1, str2):
    str = str1
    if str != "": str = str + GRAMMAR_SPACE
    str = str + str2
    return str
  
  def handleBLOCK(node, prefix):
    altPrefix = []
    for child in node.children:
      altPrefix = altPrefix + handleRuleNode(child, [])
    # The len(node.children) test is kind of a hack to avoid merging for 'unsigned'? int matrix types
    if (len(node.children) >= MERGE_THRESHOLD) and (len(altPrefix)) > 1 and (not DO_EXPAND and MERGE_ALTERNATIVES):
      newAltPrefix = []
      while len(altPrefix) > 0:
	part = altPrefix[:4]
	newAltPrefix.append ("\\Galt{"+ "\\Gor{}".join(part) + "}")
	altPrefix = altPrefix[4:]
      altPrefix = newAltPrefix
    newPrefix = []
    if prefix:
      for p in prefix:
	for a in altPrefix:
	  newPrefix.append(concat_with_space (p, a))
    else:
      newPrefix = altPrefix
    #print "prefix = ", prefix, "altPrefix = ", altPrefix, "newPrefix = ", newPrefix
    return newPrefix
    
  def handleALT(node, prefix):
    for child in node.children:
      prefix = handleRuleNode(child, prefix)
    return prefix
    
  def handleRULE_REF(node, prefix):
    NT = getNodeStr(node)
    nodeStr = "\Gnt{%s}%s " % (tex_escape(NT), format_nt_ref("(\\ref{%s})" % NT))
    if nodeStr:
      newPrefix = []
      if prefix:
	for p in prefix:
	    newPrefix.append(concat_with_space (p, nodeStr))
      else:
	newPrefix = [nodeStr]
      return newPrefix
    else:
      return prefix
      
  def handleTOKEN_REF(node, prefix):
    NT = getNodeStr(node)
    nodeStr = "\Gnt{%s}%s" % (tex_escape(NT), format_nt_ref("(\\ref{%s})" % NT))
    if nodeStr:
      newPrefix = []
      if prefix:
	for p in prefix:
	    newPrefix.append(concat_with_space (p, nodeStr))
      else:
	newPrefix = [nodeStr]
      return newPrefix
    else:
      return prefix
      
  def handleLITERAL(node, prefix):
    nodeStr = getNodeStr(node)
    nodeStr = nodeStr[1:][:-1]
    nodeStr = "\Gt{" + tex_escape(nodeStr) + "}"
    if nodeStr:
      newPrefix = []
      if prefix:
	for p in prefix:
	    newPrefix.append(concat_with_space (p, nodeStr))
      else:
	newPrefix = [nodeStr]
      return newPrefix
    else:
      return prefix
      
  def handleOPTIONAL(node, prefix):
    optPrefix = []
    for child in node.children:
      optPrefix = handleRuleNode(child, optPrefix)
    if DO_EXPAND:
      newPrefix = []
      if prefix:
	for p in prefix:
	  newPrefix.append(p)
	  for p2 in optPrefix:
	    np = concat_with_space (p, p2)
	    newPrefix.append(np)
      else:
	newPrefix = [""] + optPrefix
    else:
      newPrefix = []
      if prefix:
	for p in prefix:
	  for p2 in optPrefix:
	    np = concat_with_space (p, "\Gopt{" + p2 + "}")
	    newPrefix.append(np)
      else:
	for p2 in optPrefix:
	  newPrefix.append("\Gopt{" + p2 + "}")
    return newPrefix
    
  def handleCLOSURE(node, prefix):
    #raise "Closures are not supported"
    if DO_EXPAND:
      print >> sys.stderr, "Closures are not supported"
      newPrefix = prefix
    else:
      optPrefix = []
      for child in node.children:
	optPrefix = handleRuleNode(child, optPrefix)
      newPrefix = []
      if prefix:
	for p in prefix:
	  for p2 in optPrefix:
	    np = concat_with_space (p, "\Gclosure{" + p2 + "}")
	    newPrefix.append(np)
      else:
	for p2 in optPrefix:
	  newPrefix.append("\Gclosure{" + p2 + "}")
    return newPrefix
      
  def handleCHAR_RANGE(node, prefix):
    nodeStr = "\Grange{" + getNodeStr(node.children[0])[1:2] + "}"
    nodeStr = nodeStr + "{" + getNodeStr(node.children[1])[1:2] + "}"
    if nodeStr:
      newPrefix = []
      if prefix:
	for p in prefix:
	    newPrefix.append(concat_with_space (p, nodeStr))
      else:
	newPrefix = [nodeStr]
      return newPrefix
    else:
      return prefix
      
  def noop(node, prefix):
    return prefix
    
  def handleDOC_COMMENT(node, prefix):
    docStr = getNodeStr(node)
    parseDocs(docStr)
    return prefix
    
  def handleRuleNode(node, prefix):
    type = node.token.type
    handleType = {
      BLOCK: handleBLOCK,
      ALT: handleALT,
      RULE_REF: handleRULE_REF,
      TOKEN_REF: handleTOKEN_REF,
      EOA: noop,
      EOB: noop,
      EOR: noop,
      CHAR_LITERAL: handleLITERAL,
      STRING_LITERAL: handleLITERAL,
      OPTIONAL: handleOPTIONAL,
      CLOSURE: handleCLOSURE,
      CHAR_RANGE: handleCHAR_RANGE,
      ACTION: noop,
      DOC_COMMENT: handleDOC_COMMENT,
    }
    if type in handleType:
      prefix = handleType[type](node, prefix)
    else:
      print "<unknown rule node type %s>" % reverseLookupType(type)
      nodeStr = getNodeStr(child)
      if nodeStr:
	print nodeStr
      printChildren(node)
    return prefix
  
  ruleID = ''
  for child in node.children:
    type = child.token.type
    if type == ID:
      ruleID = getNodeStr(child)
      break

  for child in node.children:
    type = child.token.type
    if type == ID:
      continue
    prefix = handleRuleNode(child, prefix)
  #print ruleID, "->", prefix
  currentDocSection.rules += [(ruleID, prefix)]
  
def printID(node):
  nodeStr = getNodeStr(node)
  print nodeStr

def printNode(node):
  type = node.token.type
  printType = {
    COMBINED_GRAMMAR: printGrammar,
    RULE: printRule,
    ID: printID,
  }
  if type in printType:
    printType[type](node)
  else:
    print "<unknown node type %s>" % reverseLookupType(type)
    nodeStr = getNodeStr(node)
    if nodeStr:
      print nodeStr
    printChildren(node)
  #if node.token.input:
  #  print node.token.type, node.token.input.strdata[node.token.start:node.token.stop+1]
  #else:
  #  print node.token.type

printNode(r.tree)

def writeSections(sections):
  for docSect in sections:
      if docSect.name:
	  print "\\%s{%s}\\label{%s}" % (docSect.type, docSect.name, tex_escape_label (docSect.name))
      
      if (len(docSect.rules) > 0) and (docSect.breakBeforeRules):
	print "~\\\\"
      for (ruleID, prefix) in docSect.rules:
	print ("\\Gnt{%s}:\\label{%s}\\\\" % (tex_escape (ruleID), ruleID))
	# @@@ Insert some indentation here
	if docSect.customRules.has_key(ruleID):
	    print ("\hspace*{1cm}%s\\\\" % docSect.customRules[ruleID])
	else:
	    for p in prefix:
	      print ("\hspace*{1cm}%s\\\\" % p)
	  
      for line in docSect.contents:
	  print line

import codecs
sys.stdout = codecs.open (outFile, "w", "utf-8")

flushSections()
writeSections(allSections)
	 
#print r.__dict__
#print r.tree
#print r.tree.__dict__
#print r.tree.token
#print r.tree.token.__dict__
##print r.tree.__dict__
#print r.tree.children[1]
#print r.tree.children[1].__dict__
#for c in r.tree.children[1].children:
#  print c
#  print c.__dict__
#  #print c.token
#  #print c.token.__dict__
##print r.tree.children[1].token
##print r.tree.children[1].token.__dict__

# this is the root of the AST
#root = r.tree

#nodes = antlr3.tree.CommonTreeNodeStream(root)
#nodes.setTokenStream(tokens)
#eval = Eval(nodes)
#eval.prog()

