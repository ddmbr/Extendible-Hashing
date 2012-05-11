#! /usr/bin/env python
import sys
num = int(sys.argv[1])
cnt = 0
with open('lineitemcut.tbl', 'w') as fout:
    for line in open('lineitem.tbl', 'r'):
        cnt += 1
        if cnt <= num:
            print >> fout, line.rstrip()
        else:
            break
