import sys

import sys, matplotlib, os.path
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

matplotlib.rcParams['text.usetex'] = True
matplotlib.rcParams['text.latex.preamble'] = [r'\usepackage{amsmath}']

#pagesize = 2**20
pagesize = 4096
limit = 30

if __name__ == "__main__":
    filename = sys.argv[1]
    splits = filename.split("_")
    values = list(map(int, open(filename, 'r').read().split('\n')[:-1]))
    access = [ values.count(i) for i in range(pagesize) ]
    othgood = sum(access[x] for x in range(pagesize) if access[x] <= limit and x < pagesize-8)
    othbad = sum(access[x] for x in range(pagesize) if access[x] <= limit and x >= pagesize-8)
    x, y = zip(*[ (x, access[x]) for x in range(pagesize) if access[x] > limit ])
    x, y = ("others good",) + x + ("others bad",), (othgood,) + y + (othbad,)
    plt.yticks(np.arange(len(x)), x)
    plt.barh(np.arange(len(x)), y)
    plt.ylabel("memory offset")
    plt.xlabel("nr. of accesses")
    plt.title(splits[1] + " within-a-page memory access: " + splits[2])
    #plt.savefig('{}.pdf'.format(filename.split('.')[0], 'pdf'), bbox_inches='tight')
    plt.show()


#if __name__ == "__main__":
#    values = open(sys.argv[1], 'r').read().split('\n')[:-1]
#    total = len(values)
#    values = list(map(int, values))
#    counter = [ (i, values.count(i)) for i in set(values) ]
#    counter.sort(key = lambda k: k[1])
#    for k,v in counter:
#        print("{} => {}".format(k, v))
#    print("total = {}".format(total))
