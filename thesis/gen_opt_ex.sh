#!/bin/sh

generate_common()
{
  sample=$1
  opt=$2
  s1cflags=$3
  
  S1C_LATEX="../../s1-build/apps/s1c/s1c --backend latex"
  $S1C_LATEX $3 -O0 s1source/opt_ex/${sample}.s1 > s1latex/opt_ex/${sample}_unopt.tex
  $S1C_LATEX $3 -O0 $opt s1source/opt_ex/${sample}.s1 > s1latex/opt_ex/${sample}_opt.tex
}

generate()
{
  generate_common "$1" "$2" --nosplit
}

generate bi -Oinline-blocks
generate cf -Oconstant-folding

generate dce -Odce
