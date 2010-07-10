#!/bin/sh
for f in snips/*; do
  truncate -s 0 $f
done
python ./snipextract.py ../lib/ ../include/
