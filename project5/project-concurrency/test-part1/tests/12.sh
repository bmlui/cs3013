#!/bin/bash

SIZE=$(( 2**22 ))

./generator $SIZE 3 | ./scan - $SIZE 16 | tail -n100
