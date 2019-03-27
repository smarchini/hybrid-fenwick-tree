#!/usr/bin/env python3

import sys, numpy, matplotlib, os.path
import matplotlib.pyplot as pyplot
from matplotlib.ticker import FuncFormatter
from collections import OrderedDict

style = OrderedDict(
    [('solid',               (0, ())),
     ('loosely dotted',      (0, (1, 10))),
     ('dotted',              (0, (1, 5))),
     ('densely dotted',      (0, (1, 1))),

     ('loosely dashed',      (0, (5, 10))),
     ('dashed',              (0, (5, 5))),
     ('densely dashed',      (0, (5, 1))),

     ('loosely dashdotted',  (0, (3, 10, 1, 10))),
     ('dashdotted',          (0, (3, 5, 1, 5))),
     ('densely dashdotted',  (0, (3, 1, 1, 1))),

     ('loosely dashdotdotted', (0, (3, 10, 1, 10, 1, 10))),
     ('dashdotdotted',         (0, (3, 5, 1, 5, 1, 5))),
     ('densely dashdotdotted', (0, (3, 1, 1, 1, 1, 1)))])

matplotlib.rcParams['text.usetex'] = True
matplotlib.rcParams['text.latex.preamble'] = [r'\usepackage{amsmath}']

titles = {'prefix.csv': r'$\operatorname{\textsf{prefix}}$',
          'add.csv': r'$\operatorname{\textsf{add}}$',
          'find.csv': r'$\operatorname{\textsf{find}}$'}

def generate(prefix, setxlim, setylim):
    for filename in titles.keys():
        fname = os.path.join('./data', filename)
        title = titles[filename]
        data = numpy.genfromtxt(fname, delimiter=',', missing_values=set([0, '0']), names=True, autostrip=True, case_sensitive=True, deletechars='', dtype=None)
        plot = pyplot.figure(figsize=[7.0, 2.5]).add_subplot(1,1,1, xlim=setxlim(filename), ylim=setylim(filename))
        matplotlib.rcParams.update({'font.size': 8})
        ########################################################################################################################################
        # NOTE: Comment what you want to hide
        ########################################################################################################################################
        plot.semilogx(data['Elements'], data['fixed[F]holes'],         linestyle=style['solid'],  color='#000000', label='fixed[F]holes')
        plot.semilogx(data['Elements'], data['byte[F]holes'],          linestyle=style['solid'],  color='#ff0000', label='byte[F]holes')
        plot.semilogx(data['Elements'], data['bit[F]holes'],           linestyle=style['solid'],  color='#00ff00', label='bit[F]holes')
        #################################### 
        plot.semilogx(data['Elements'], data['fixed[F]noholes'],         linestyle=style['densely dotted'],  color='#000000', label='fixed[F]noholes')
        plot.semilogx(data['Elements'], data['byte[F]noholes'],          linestyle=style['densely dotted'],  color='#ff0000', label='byte[F]noholes')
        plot.semilogx(data['Elements'], data['bit[F]noholes'],           linestyle=style['densely dotted'],  color='#00ff00', label='bit[F]noholes')
        ########################################################################################################################################
        pyplot.title('Fenwick tree: {}'.format(title), fontsize='large')
        pyplot.xlabel('$\#$ elements', fontsize='medium')
        pyplot.ylabel('time (nS)', fontsize='medium')
        pyplot.legend(loc="upper center", ncol=2, shadow=True, fancybox=True, prop={'size': 6}, framealpha=0.5, frameon=True)
        ########################################################################################################################################
        pyplot.savefig('{}/{}.{}'.format(prefix, filename.split('.')[0], 'pdf'), bbox_inches='tight')
        #pyplot.show()

def large_x(fig):
    if fig == 'prefix.csv': return (5*10**3, 2*10**9)
    if fig == 'add.csv': return (5*10**3, 2*10**9)
    if fig == 'find.csv': return (5*10**3, 2*10**9)
    return None

def large_y(fig):
    if fig == 'prefix.csv': return (0, 200)
    if fig == 'add.csv': return (0, 350)
    if fig == 'find.csv': return (0, 700)
    return None

def zoom_x(fig):
    if fig == 'prefix.csv': return (10**8, 3*10**9)
    if fig == 'add.csv': return (10**8, 3*10**9)
    if fig == 'find.csv': return (10**8, 3*10**9)
    return None

def zoom_y(fig):
    if fig == 'prefix.csv': return (50, 400)
    if fig == 'add.csv': return (150, 700)
    if fig == 'find.csv': return (100, 700)
    return None

if __name__ == '__main__':
    if os.path.exists("./large/") and os.path.exists("./zoom/"):
        generate('large', large_x, large_y)
        generate('zoom', zoom_x, zoom_y)
    else:
        print("You need to:")
        print("    mkdir large zoom")
