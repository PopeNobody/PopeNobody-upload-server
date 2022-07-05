#!/bin/bash


set -vx
file="$1"
shift
echo "$@" > ${file}.new
if test -e "${file}" ; then
  report cmp ${file}.new ${file} 
  rm -f "${file}"
fi
if test -e "${file}"; then
  rm -f "${file}.new"
else
  mv ${file}.new ${file}
fi
