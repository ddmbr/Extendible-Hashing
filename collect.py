#! /usr/bin/env python
import os, sys
dests = ['bin/least_128', 'bin/least_8', 'bin/most_8', 'bin/most_128']
for dest in dests:
    os.system('src/iopercent.py %s %s| tee result-%s' % (dest, sys.argv[1], os.path.basename(dest)))
