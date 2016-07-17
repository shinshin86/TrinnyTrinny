#!/bin/bash

files="./image/*.png"
for filepath in $files; do
  ./HoughTrim $filepath
done
