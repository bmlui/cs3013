#! /bin/bash

NAME=scan
WORKING_DIR=$(pwd)
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [[ "$WORKING_DIR" != "$SCRIPT_DIR" ]]; then
  echo "Please rerun this script from: $SCRIPT_DIR"
  exit 1
fi

PARENT_DIR="$(dirname "$SCRIPT_DIR")"

if ! [[ -f "$PARENT_DIR/$NAME.c" ]]; then
    echo "Error: Could not find $NAME.c in directory: $PARENT_DIR"
    exit 1
fi

if [[ -x "$NAME" ]]; then
    echo "Removing old $NAME binary."
    rm "$NAME"
fi

if [[ -x "$NAME" ]]; then
    echo "Error: Could not remove old "$NAME" binary."
    exit 1
fi

if ! [[ -f "$PARENT_DIR/generator.c" ]]; then
    echo "Error: Could not find generator.c in directory: $PARENT_DIR"
    exit 1
fi

gcc -g ../generator.c -o generator

../test-lib/run_tests.sh $*


