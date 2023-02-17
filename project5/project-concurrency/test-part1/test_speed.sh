#!/bin/bash

NUM_THREADS=16
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
    #echo "Removing old $NAME binary."
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


if ! [[ -f "$PARENT_DIR/scan_serial.c" ]]; then
    echo "Error: Could not find scan_serial.c in directory: $PARENT_DIR"
    exit 1
fi

gcc ../generator.c -o generator
gcc ../scan_serial.c -o scan_serial 
gcc ../scan.c -o scan -lpthread -lm

SIZE=$(( 2**22 ))

start_serial="$(date +%s%N)"

./generator $SIZE 3 | ./scan_serial - $SIZE | tail -n100 > ./tests-out/speed_scan_serial.out 

#Get the time in milliseconds
duration_serial=$[ ($(date +%s%N) - ${start_serial}) / 1000000 ]

start="$(date +%s%N)"

./generator $SIZE 3 | ./scan - $SIZE $NUM_THREADS | tail -n100 > ./tests-out/speed_scan.out

#Get the time in milliseconds
duration=$[ ($(date +%s%N) - ${start}) / 1000000 ]


# Check that the scan binary worked properly.
if !  diff ./tests-out/speed_scan_serial.out ./tests-out/speed_scan.out > /dev/null;
then
  echo "scan output does not match expected results. Please ensure your code passes all part 1 test cases."
  exit 1
fi 

echo "scan_serial runtime: ${duration_serial} milliseconds."
echo "scan runtime: ${duration} milliseconds."

overhead=`echo - | awk "{print ${duration} / ${duration_serial}}"`


if [ ${duration} -gt ${duration_serial} ]; 
then
  echo "Scan was slower than scan_serial!"
else 
  echo "Scan was faster than scan_serial!"
fi

echo "Difference of ${overhead}X."

