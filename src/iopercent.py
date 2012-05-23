#! /usr/bin/env python
import os, sys
import subprocess as sb
import re

def get_io_percentage(args):
    """ args is a list, for example ['./a', '-a', '123', '-b', 234]"""

    tmpfile = '/tmp/time-out-%d'%os.getpid()
    cmd = ['/usr/bin/time', '-f', '%P', '-o', tmpfile] + args
    print 'run:', cmd
    p = sb.Popen(cmd)
    p.wait()
    output = open(tmpfile, 'r').read()[:-2]
    return 100 - float(output)

r = get_io_percentage(sys.argv[1:])
print 'io percentage:', r
