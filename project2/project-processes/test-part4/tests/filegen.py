#! /usr/bin/env python

import random
import string

for i in range(50000):
    x = ''
    for i in range(80):
        x += random.choice(string.letters + ' ')
    print(x)
