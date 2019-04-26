#!/usr/bin/python3
import subprocess, signal, sys, os
from datetime import datetime

def ctrlc(sig, frame):
sys.exit(0)

signal.signal(signal.SIGINT, ctrlc)

timestamp = datetime.now().strftime('%Y%m%d-%H%M%S')
exe = './bin/benchmark/rankselect/tofile'
dst = './benchout/rankselect/{}/'.format(timestamp)
queries = '100000'

#points = [ 2**k - 1 + 2**(k-m) - 1 for k in range(33) for m in range(k) ]
points = list(sum([ (2**k-1, 3*2**k-1) for k in range(33) for m in range(k) ], ()))

points = filter(lambda n: n > 10**5 and n < 4*10**9, points)
points = [ str(i) for i in sorted(list(set(points))) ]

os.makedirs(dst, exist_ok=True)
for size in points:
            cmd = subprocess.run([ exe, dst, size, queries], stdout=sys.stdout, stderr=sys.stderr)
