#!/bin/sh

collect_included()
{
  header_file="$1"
  shift
  gcc -x c-header -ffreestanding -E "$header_file" "$@" \
    | grep -v "$header_file" \
    | grep "^#" \
    | tail -n +2 \
    | sed -e 's/.*"\(.*\)".*/\1/' \
    | tac \
    | awk ' !x[$0]++' \
    | tac
}

for f in $(collect_included "$@"); do
  cat $f | grep -v "^#.*include"
done
