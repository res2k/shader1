import os

def handleFile(fn):
  inFile = file(fn, "r")
  currentOutput = None
  for line in inFile:
    if line.startswith("//@@"):
      args = line.strip()[4:].split(' ', 1)
      if args[0] == "SNIP":
	currentOutput = file ("snips/" + args[1], "a")
      elif args[0] == "ENDSNIP":
	currentOutput = None
    elif currentOutput:
      currentOutput.write(line)

def handleEntry(entry):
  if entry.endswith("~"): return
  if os.path.isdir(entry):
    subEntries = os.listdir(entry)
    for subEntry in subEntries:
      handleEntry(entry + "/" + subEntry)
  elif os.path.isfile(entry):
    handleFile(entry)

from optparse import OptionParser

options = OptionParser()
(parsed_options, args) = options.parse_args()

for arg in args:
  handleEntry(arg)
  
