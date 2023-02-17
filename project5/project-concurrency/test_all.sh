#! /bin/bash

NAME=wshell
WORKING_DIR=$(pwd)
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [[ "$WORKING_DIR" != "$SCRIPT_DIR" ]]; then
  echo "Please rerun this script from: $SCRIPT_DIR"
  exit 1
fi

if !(gcc --help 2>&1 | grep -i "Usage: gcc" 2>&1 >/dev/null); then
  echo "Please install GCC before running this script" 
  exit 1
fi 

if !(valgrind --help 2>&1 | grep -i "usage: valgrind" 2>&1 >/dev/null); then
  echo "Please install valgrind before running this script" 
  exit 1
fi 



echo "PART 1: Functionality"
cd test-part1
PARTDIR="test-part1/" ./test_part1.sh $*
cd ..

echo  
echo "PART 2: Design Document"

if ! [[ -f "readme.txt" ]]; then
  echo "Error: Could not find your design document (readme.txt)"
else
 echo "Your design document consists of `wc -l readme.txt` lines."
fi

echo
echo "PART 3: Performance"
cd test-part1
./test_speed.sh $*
cd ..

