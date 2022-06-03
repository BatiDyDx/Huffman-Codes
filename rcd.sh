#!/bin/bash

# Run Compressions and Decompressions script

make

for var in "$@"
do
  echo ""
  time ./huff C "$var"
  echo ""
  time ./huff D "$var.hf"
  echo ""
  echo "Informacion de la compresion: " &&  ls -l -sh "$var.hf"
  echo ""
  diff "$var" "$var.dec" -s
  rm "$var".{hf,tree,dec}
done

make clean
