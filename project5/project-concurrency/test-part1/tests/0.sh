#!/bin/bash

gcc -Wall -Werror -o scan ../scan.c 2> /dev/null

if [ "$?" -eq 0 ]; then
  echo "Reason for failure: Program does not appear to use the pthreads library!"
  exit 1
fi

exit 0
