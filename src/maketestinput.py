import re
keys = []
with open('lineitemcut.tbl', 'r') as f:
    n = 0
    for line in f:
        key = int(line[:line.find('|')])
        n += 1
        keys.append(key)
    print n
    for key in keys:
        print key
