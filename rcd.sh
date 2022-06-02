#!/bin/bash

make

for var in "$@" 
do
 time ./huff C "$var"
 echo ""
 time ./huff D "$var.hf"
 echo ""
 diff "$var" "$var.dec" -s
 rm "$var".{hf,tree,dec}
done

make clean
