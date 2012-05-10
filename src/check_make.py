import re
import sys, os
s = open('Makefile', 'r').read().replace('\\\n\t', ' ').replace('\\\n','')

IncludePatt = re.compile('#include ?"(.+)"')

equals = re.findall('([a-zA-z_0-9.]+)=(.+)', s)
print 'vars:', equals
for a, b in equals:
    s = s.replace('$(%s)' % a, b)

print 'final:'
print s

def search_file_dependents(fname):
    r = set()
    if not os.path.exists(fname):
        return r
    r.add(fname)
    # print 'search on "%s"' % fname
    if not (fname.endswith('.h') or fname.endswith('.c')):
        return r
    with open(fname, 'r') as f:
        s = f.read()
        includes = IncludePatt.findall(s)
        # print '  includes', includes
        for x in includes:
            r = r.union(search_file_dependents(x))
        # print '  return r', r
        return r

ItemPatt = re.compile('([a-zA-z_0-9.]+):\s*(.*?)\n\t(.+?)\n\n', re.M)

def check():
    items = ItemPatt.findall(s)
    for obj, originDeps, opr in items:
        print '-'*20
        originDeps = set(originDeps.split(' '))
        deps = reduce(set.union, (search_file_dependents(x) for x in originDeps), set())
        newDeps = deps - originDeps
        if newDeps:
            print 'object:', obj
            print 'origin dependents:', originDeps
            print 'new detected dependents:', newDeps

check()
