#!/usr/bin/env python
tmpfile = '.mftemp'
makecmd='make -B -f %s && mv -v ehdb ../bin/' % tmpfile

import sys,os
import re
ehdbs = {
    ('L', '8'):'ehdb_least_8',
    ('H', '8'):'ehdb_most_8',
    ('L', '128'):'ehdb_least_128',
    ('H', '128'):'ehdb_most_128'
    }

def build(LH, PageNum):
    if LH in 'LH' and PageNum in ('8', '128'):
        s = open('Makefile', 'r').read()
        s = re.sub('HashFunc=[LH]_HASH', 'HashFunc=%s_HASH' % LH, s)
        open(tmpfile, 'w').write(s)
        os.system(makecmd + ehdbs[LH, PageNum])
    else:
        print 'Invalid parameters: ', LH, PageNum
    
if len(sys.argv) == 1:
    for LH, PageNum in ehdbs:
        build(LH, PageNum)
elif len(sys.argv) == 3:
    LH = sys.argv[1]
    PageNum = sys.argv[2]
    build(LH, PageNum)
else:
    print """usage: build.py [(H|L) (8|128)]"""


