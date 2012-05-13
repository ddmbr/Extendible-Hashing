import re
import sys
import pygame as pg
from pygame import Rect
from time import sleep
DEBUG = 0
PCONTENT = 0
class Record:
    FormatPatt = re.compile(r'(\d+)\|(\d+)\|')
    def __init__(self, rawstr):
        self.data = rawstr
        self.orderkey, self.partkey = map(int, Record.FormatPatt.match(rawstr).groups())

    def __cmp__(self, other):
        return cmp(self.partkey, other.partkey)

    def __repr__(self):
        if PCONTENT:
            return str((self.orderkey, self.partkey, '...', 'hv=%30s' % (bin(hash_func(self.orderkey, globalDepth))[2:])))
        else:
            return 'r'

class Bucket:
    MaxSize = 8 * 1024
    # MaxSize = 8 * 500
    newBucketID = [2]
    def __init__(self, id, depth):
        if DEBUG: print 'bucket ', id, 'created'
        self.depth = depth
        self.id = id
        self.page = []
        self.sumLen = 0

    def __repr__(self):
        if PCONTENT:
            return """%s\nBucket(id=%d, depth=%d, \n%s\n, len=%d) \n%s\n""" % ('-'*60,
                    self.id, self.depth, '\n'.join(map(str, self.get_records())), self.sumLen, '-'*60)
        else:
            return "Bucket(id=%d, depth=%d)"%(self.id, self.depth)

    @staticmethod
    def get_by_hvalue(hvalue):
        return indexs[hvalue/Index.DictPerPage].d[hvalue%Index.DictPerPage]

    def is_overflow(self, record):
        return self.sumLen + len(record.data) > self.MaxSize

    def write_record(self, record):
        self.page.append(record)
        self.sumLen += len(record.data)

    def get_records(self):
        return self.page

    def insert(self, record):
        bucket = self
        while bucket.is_overflow(record):
            bucket.split(hash_func(record.orderkey, globalDepth))
            hvalue = hash_func(record.orderkey, globalDepth)
            bucket = Bucket.get_by_hvalue(hvalue)
        if DEBUG: print 'write record %s into Bucket(id=%d)' %(record, bucket.id)
        bucket.write_record(record)

    def split(self, hvalue):
        if DEBUG: print 'split bucket', self.id
        if self.depth == globalDepth:
            double_index()
        depth = self.depth
        hvalue = hvalue & ((1 << (depth)) - 1)
        newHv = ((1 << depth)) + hvalue
        if DEBUG:  print 'depth=%d, old,new(hv) = \n  %40s\n  %40s'%(depth, bin(hvalue), bin(newHv))
        self.depth += 1
        depth = self.depth
        # create new bucket
        newBucket = Bucket(Bucket.newBucketID[0], depth)
        Bucket.newBucketID[0] += 1
        buckets.append(newBucket)
        # create temp bucket
        tempBucket = Bucket(-1, depth)
        # relink new bucket to the indexs
        for i in xrange(0, 2**globalDepth):
            if Bucket.get_by_hvalue(i) == self:
                if hash_func(i, depth) == hvalue:
                    set_dict(i, self)
                else:
                    set_dict(i, newBucket)
                
        # redistribute the buckets
        for record in self.get_records():
            if hash_func(record.orderkey, depth) == newHv:
                newBucket.write_record(record)
            else:
                tempBucket.write_record(record)
        self.page = tempBucket.page
        self.sumLen = tempBucket.sumLen
        GDB.update()
        if DEBUG: print 'after split bucket(id=%d), n1=%d, n2=%d' %(self.id, len(self.get_records()), len(newBucket.get_records()))
        if DEBUG: print 'split bucket %d end' % self.id

class Index:
    DictPerPage = 2 * 1024
    # DictPerPage = 100
    def __init__(self, id):
        self.id = id
        self.d = [None for i in xrange(Index.DictPerPage)]

    def __repr__(self):
        if not PCONTENT: 
            n = 0
            for i in self.d:
                if i is not None:
                    n += 1
                else:
                    break
            return 'Index(id=%d)'%(self.id)
        w = 1
        s = []
        s.append('-'*60)
        s.append('Index(id=%d)' % self.id)
        line = ''
        for i, b in enumerate(self.d):
            if b is None: break
            line += str((i, b.id))
            if (i + 1) % w == 0:
                s.append(line)
                line = ''
        if line:
            s.append(line)
        s.append('-'*60)
        return '\n'.join(s)

def init():
    global globalDepth
    globalDepth = 1
    indexs.append(Index(0))
    buckets.append(Bucket(0, 1))
    buckets.append(Bucket(1, 1))
    indexs[0].d[0] = buckets[0]
    indexs[0].d[1] = buckets[1]

def parse(faddr):
    global DEBUG
    with open(faddr, 'r') as f:
        for line in f:
            record = Record(line.rstrip())
            if DEBUG: print 'prepare to insert ', record
            GDB.update()
            hvalue = hash_func(record.orderkey, globalDepth)
            bucket = Bucket.get_by_hvalue(hvalue)
            bucket.insert(record)
            if DEBUG: print 'after insert'
            if DEBUG: print len(indexs), indexs 
            if DEBUG: print len(buckets), buckets
            # print '%30s %30s usage: %.2f' %(len(indexs), len(buckets), 
            #         sum(x.sumLen for x in buckets)*100./(len(buckets)*Bucket.MaxSize))

def query(faddr):
    with open(faddr, 'r') as f:
        n = int(f.readline())
        for i in xrange(n):
            key = int(f.readline())
            bucket = Bucket.get_by_hvalue(hash_func(key, globalDepth))
            ans = []
            for record in bucket.get_records():
                if record.orderkey == key:
                    ans.append(record)
            ans.sort()
            for record in ans:
                print record.data
            print -1

def hash_func(key, depth):
    # return key & ((1 << depth) - 1)
    j = 1 << (depth - 1)
    # for i in xrange(29):
    #     if key & (1 << i):
    #         j = (1 << i)
    ans = 0
    for i in xrange(depth):
        if (key & j) : ans += 1 << i
        j >>= 1
    # print bin(key), bin(ans), depth
    return ans

def double_index():
    global globalDepth
    n = 1 << globalDepth
    print 'double index from %d' % n
    for i in xrange(n):
        j = i + n
        indexID1 = i / Index.DictPerPage
        indexID2 = j / Index.DictPerPage
        if indexID2 >= len(indexs):
            indexs.append(Index(indexID2))
        # indexs[indexID2].d[j % Index.DictPerPage] = indexs[indexID1].d[i % Index.DictPerPage]
        set_dict(j, Bucket.get_by_hvalue(i))
    GDB.update()
    globalDepth += 1
    if DEBUG: print 'after double'
    if DEBUG: print 'globalDepth', globalDepth
    if DEBUG: print 'indexs=', indexs

class GDB(): # graphical debugger
    W, H = 1000, 500
    FPS = 10

    @staticmethod
    def init():
        GDB.screen = pg.display.set_mode((GDB.W, GDB.H), 0, 32)
        GDB.timer = pg.time.Clock()
        GDB.t = 0.
        GDB.update()

    @staticmethod
    def update():
        # GDB.t += GDB.timer.tick()
        # if GDB.t >= 1000/GDB.FPS:
        #     GDB.t = 0
        # else:
        #     return
        sleep(0.02)
        if DEBUG: print 'update screen'
        bg = (0xff, 0xff, 0xff, 0xff)
        idxColor = (0x00, 0x2f, 0x00, 0xff)
        bucketColor = (0x1f, 0x1f, 0x00, 0xff)
        screen = GDB.screen
        screen.fill(bg)

        x0, y0 = 10, 5
        y1 = GDB.H - 5
        Sep = 2
        bucketW = min(20, max(1, GDB.W / len(buckets) - Sep))
        idxW = min(20, max(1, GDB.W / len(indexs) - Sep))

        # b.x = x0 + b.id * (bucketW + Sep)
        # b.y = y0 - q * b.sumLen
        # q = 0.02
        dw = min((GDB.W + 0.) / len(buckets), (bucketW + Sep))
        for b in buckets:
            x = x0 + b.id * dw
            h = 10 + 200./Bucket.MaxSize * b.sumLen
            y = y1 - h
            pg.draw.rect(screen, bucketColor, Rect((x, y), (bucketW, h)))
        # idx.x = x0 + idx.id * (idxW + Sep)
        # idx.y = y0 + 5
        dw = min((GDB.W + 0.) / len(indexs), (idxW + Sep))
        for index in indexs:
            x = x0 + index.id * dw
            y = y0
            h = 10 + 100./Index.DictPerPage * len(index.d)
            pg.draw.rect(screen, idxColor, Rect((x, y), (idxW, h)))

        pg.display.flip()

def set_dict(index, bucket):
    indexs[index / Index.DictPerPage].d[index % Index.DictPerPage] = bucket

globalDepth = 0
indexs = []
buckets = []

def main():
    init()
    GDB.init()
    print indexs 
    print buckets
    parse('lineitemcut.tbl')
    query('testinput.in')
    print 'finish'
    print 'buckets', buckets
    sys.stdout.flush()
    while 1:
        for e in pg.event.get():
            if e.type == pg.QUIT:
                exit(0)
            elif e.type == pg.KEYDOWN:
                GDB.update()
        sleep(0.1)

if __name__ == '__main__':
    main()
