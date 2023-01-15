#! /bin/bash

NAME=wshell
WORKING_DIR=$(pwd)
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [[ "$WORKING_DIR" != "$SCRIPT_DIR" ]]; then
  echo "Please rerun this script from: $SCRIPT_DIR"
  exit 1
fi

echo "PART 3:"
cd test-part3
./test_part3.sh $*
cd ..

echo "PART 4:"
cd test-part4
./test_part4.sh $*
cd ..

