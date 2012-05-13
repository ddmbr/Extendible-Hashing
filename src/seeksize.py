#! /usr/bin/env python
import os
from time import sleep
while 1:
    os.system('clear')
    os.system('du --max-depth=0 -h index')
    os.system('du --max-depth=0 -h bucket')
    print '-'*80
    sleep(0.2)

