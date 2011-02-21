#!/bin/sh
for f in s1source/*.s1; do
  echo $f: 
  ../../s1-build/apps/s1c/s1c --backend latex --nosplit -O0 $f > s1latex/`basename $f .s1`.tex
done
