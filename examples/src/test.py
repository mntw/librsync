import subprocess, time, os, numpy as np
#dd conv=notrunc if=/dev/urandom of=txt bs=1 seek=1 count=1

def exe(cmd):
    subprocess.check_output(cmd, shell=True)

class File(object):
    def __init__(self, filename, blocksize = 2048000, trials = 2048000/1000):
        self.filename = filename
        self.filesize = os.stat(self.filename).st_size
        self.blocksize = blocksize 
        self.trials = trials 
        self.prob = 0
        self.nblocks = self.filesize / self.blocksize
    def changeBit(self, n):
        exe('dd conv=notrunc if=/dev/urandom of=%s bs=1 seek=%d count=1' % (self.filename, n))
    def changeBlock(self, n):
        if (n + 1) * self.blocksize < self.filesize:
            exe('dd conv=notrunc if=/dev/urandom of=%s bs=%d seek=%d count=1; sync' % (self.filename, self.blocksize,  n ))
    def changeRandBits(self, block=0 ):
        x_1 = block * self.blocksize
        x_2 = x_1 + self.blocksize
        x = list(set(np.random.randint(x_1, x_2, self.trials)))
        for i in x:
            self.changeBit(i)
    def changeFile(self, p=0.01):
        self.prob = p
        res = []
        for i in range(self.nblocks):
            if np.random.rand() < p:
                #self.changeRandBits(i)
                self.changeBlock(i)
                res.append(i)
        return res



f = open('delData2', 'w')
f.write('fileSize;deltaSize;timeOndelta;timeOnPatch;chngedBlocks\n')
size = 1
for i in range(17):
    size *= 1.8 
    exe('rm -f FILE*')
    exe('fallocate -l %dM FILE' % int(size))
    exe('cp FILE FILE.new')
    exe('../bin/sync FILE ')
    F = File("FILE.new")
    array = F.changeFile(0.5)
    timestamp1 = time.time()
    exe('../bin/delta FILE FILE.new')
    timestamp2 = time.time()

    timestamp3 = time.time()
    exe('../bin/patch FILE')
    timestamp4 = time.time()
    f.write('%f;%d;%.3f;%.3f;%s\n' % (size,  int(os.stat('FILE.delta').st_size), 
        timestamp2 - timestamp1, 
        timestamp4 - timestamp3, 
        str(array)  )  )
f.close()

#f = open('delData', 'w')
#f.write('Prob;delSize;time;chngedBlocks\n')
#x = np.arange(0.0, 1.0, 0.05)
#for i in x:
#    exe('rm FILE.new; cp FILE FILE.new')
#    F = File("FILE.new")
#    array = F.changeFile(i)
#    timestamp1 = time.time()
#    exe('../bin/delta FILE FILE.new')
#    timestamp2 = time.time()
#    f.write('%f;%d;%.3f;%s\n' % (i,  int(os.stat('FILE.delta').st_size), timestamp2 - timestamp1, str(array)))
#    exe('rm FILE.delta')
#f.close()


#f = open('sigData', 'w')
#f.write('fileSize;sigSize;time\n')
#size = 1
#subprocess.check_output('make clear', shell=True)
#for i in range(17):
#    size *= 2
#    subprocess.check_output('fallocate -l %dM FILE' % size, shell=True)
#    timestamp1 = time.time()
#    subprocess.check_output('make t', shell=True)
#    timestamp2 = time.time()
#    f.write('%d;%d;%.3f\n' % (size,  int(os.stat('FILE.sig').st_size), timestamp2 - timestamp1))
#    subprocess.check_output('rm FILE.sig', shell=True)
#f.close()

