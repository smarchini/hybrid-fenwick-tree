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

titles = {'rank1.csv': r'$\operatorname{\textsf{rank}}$',
          'select1.csv': r'$\operatorname{\textsf{select}}$',
          'update.csv': r'$\operatorname{\textsf{update}}$'}

def generate(prefix, setxlim, setylim):
    for filename in titles.keys():
        fname = os.path.join('./data', filename)
        title = titles[filename]
        data = numpy.genfromtxt(fname, delimiter=',', missing_values=set([0, '0']), names=True, autostrip=True, case_sensitive=True, deletechars='', dtype=None)
        plot = pyplot.figure(figsize=[3.5, 7.0]).add_subplot(1,1,1, xlim=setxlim(filename), ylim=setylim(filename))
        matplotlib.rcParams.update({'font.size': 8})
        ##########################################################################################################################
        # NOTE: Comment what you want to hide
        ##########################################################################################################################
      ##  plot.semilogx(data['Elements'], data['fixed[F]1'],       linestyle=style['solid'],  color='#e6194B', label='fixed[F]$1$')
      ##  plot.semilogx(data['Elements'], data['fixed[$\ell$]1'],  linestyle=style['solid'],  color='#3cb44b', label='fixed[$\ell$]$1$')
      ##  plot.semilogx(data['Elements'], data['byte[F]1'],        linestyle=style['solid'],  color='#ffe119', label='byte[F]$1$')
      ##  plot.semilogx(data['Elements'], data['byte[$\ell$]1'],   linestyle=style['solid'],  color='#4363d8', label='byte[$\ell$]$1$')
      ##  plot.semilogx(data['Elements'], data['bit[F]1'],         linestyle=style['solid'],  color='#f58231', label='bit[F]$1$')
      ##  plot.semilogx(data['Elements'], data['bit[$\ell$]1'],    linestyle=style['solid'],  color='#911eb4', label='bit[$\ell$]$1$')
      # plot.semilogx(data['Elements'], data['fixed[26]bit1'],   linestyle=style['solid'],  color='#42d4f4', label='fixed[26]bit1')
      # plot.semilogx(data['Elements'], data['fixed[26]byte1'],   linestyle=style['solid'],  color='#f032e6', label='fixed[26]byte1')
      # plot.semilogx(data['Elements'], data['fixed[32]bit1'],   linestyle=style['solid'],  color='#bfef45', label='fixed[32]bit1')
      # plot.semilogx(data['Elements'], data['fixed[32]bit1'],   linestyle=style['solid'],  color='#fabebe', label='fixed[32]bit1')
        ###########################################
        plot.semilogx(data['Elements'], data['fixed[F]8'],       linestyle=style['solid'],  color='#e6194B', label='fixed[F]$8$')
        plot.semilogx(data['Elements'], data['fixed[$\ell$]8'],  linestyle=style['solid'],  color='#3cb44b', label='fixed[$\ell$]$8$')
        plot.semilogx(data['Elements'], data['byte[F]8'],        linestyle=style['solid'],  color='#ffe119', label='byte[F]$8$')
        plot.semilogx(data['Elements'], data['byte[$\ell$]8'],   linestyle=style['solid'],  color='#4363d8', label='byte[$\ell$]$8$')
        plot.semilogx(data['Elements'], data['bit[F]8'],         linestyle=style['solid'],  color='#f58231', label='bit[F]$8$')
        plot.semilogx(data['Elements'], data['bit[$\ell$]8'],    linestyle=style['solid'],  color='#911eb4', label='bit[$\ell$]$8$')
      # plot.semilogx(data['Elements'], data['fixed[26]bit8'],   linestyle=style['densely dotted'],  color='#42d4f4', label='fixed[26]bit$8$')
      # plot.semilogx(data['Elements'], data['fixed[26]bit8'],   linestyle=style['densely dotted'],  color='#f032e6', label='fixed[26]bit$8$')
      # plot.semilogx(data['Elements'], data['fixed[32]bit8'],   linestyle=style['densely dotted'],  color='#bfef45', label='fixed[32]bit$8$')
      # plot.semilogx(data['Elements'], data['fixed[32]bit8'],   linestyle=style['densely dotted'],  color='#fabebe', label='fixed[32]bit$8$')
        ###########################################
        plot.semilogx(data['Elements'], data['fixed[F]16'],      linestyle=style['densely dotted'],  color='#e6194B', label='fixed[F]$16$')
        plot.semilogx(data['Elements'], data['fixed[$\ell$]16'], linestyle=style['densely dotted'],  color='#3cb44b', label='fixed[$\ell$]$16$')
        plot.semilogx(data['Elements'], data['byte[F]16'],       linestyle=style['densely dotted'],  color='#ffe119', label='byte[F]$16$')
        plot.semilogx(data['Elements'], data['byte[$\ell$]16'],  linestyle=style['densely dotted'],  color='#4363d8', label='byte[$\ell$]$16$')
        plot.semilogx(data['Elements'], data['bit[F]16'],        linestyle=style['densely dotted'],  color='#f58231', label='bit[F]$16$')
        plot.semilogx(data['Elements'], data['bit[$\ell$]16'],   linestyle=style['densely dotted'],  color='#911eb4', label='bit[$\ell$]$16$')
      # plot.semilogx(data['Elements'], data['fixed[26]bit16'],  linestyle=style['densely dotted'],  color='#42d4f4', label='fixed[26]bit$16$')
      # plot.semilogx(data['Elements'], data['fixed[26]byte16'],  linestyle=style['densely dotted'],  color='#f032e6', label='fixed[26]byte$16$')
      # plot.semilogx(data['Elements'], data['fixed[32]bit16'],  linestyle=style['densely dotted'],  color='#bfef45', label='fixed[32]bit$16$')
      # plot.semilogx(data['Elements'], data['fixed[32]bit16'],  linestyle=style['densely dotted'],  color='#fabebe', label='fixed[32]bit$16$')
        ###########################################
      # plot.semilogx(data['Elements'], data['fixed[F]24'],      linestyle=style['densely dotted'],  color='#e6194B', label='fixed[F]$24$')
      # plot.semilogx(data['Elements'], data['fixed[$\ell$]24'], linestyle=style['densely dotted'],  color='#3cb44b', label='fixed[$\ell$]$24$')
      # plot.semilogx(data['Elements'], data['byte[F]24'],       linestyle=style['densely dotted'],  color='#ffe119', label='byte[F]$24$')
      # plot.semilogx(data['Elements'], data['byte[$\ell$]24'],  linestyle=style['densely dotted'],  color='#4363d8', label='byte[$\ell$]$24$')
      # plot.semilogx(data['Elements'], data['bit[F]24'],        linestyle=style['densely dotted'],  color='#f58231', label='bit[F]$24$')
      # plot.semilogx(data['Elements'], data['bit[$\ell$]24'],   linestyle=style['densely dotted'],  color='#911eb4', label='bit[$\ell$]$24$')
      # plot.semilogx(data['Elements'], data['fixed[26]bit24'],  linestyle=style['densely dotted'],  color='#42d4f4', label='fixed[26]bit$24$')
      # plot.semilogx(data['Elements'], data['fixed[26]bit24'],  linestyle=style['densely dotted'],  color='#f032e6', label='fixed[26]bit$24$')
      # plot.semilogx(data['Elements'], data['fixed[32]bit24'],  linestyle=style['densely dotted'],  color='#bfef45', label='fixed[32]bit$24$')
      # plot.semilogx(data['Elements'], data['fixed[32]bit24'],  linestyle=style['densely dotted'],  color='#fabebe', label='fixed[32]bit$24$')
      # ###########################################
      # plot.semilogx(data['Elements'], data['fixed[F]32'],      linestyle=style['solid'],  color='#e6194B', label='fixed[F]$32$')
      # plot.semilogx(data['Elements'], data['fixed[$\ell$]32'], linestyle=style['solid'],  color='#3cb44b', label='fixed[$\ell$]$32$')
      # plot.semilogx(data['Elements'], data['byte[F]32'],       linestyle=style['solid'],  color='#ffe119', label='byte[F]$32$')
      # plot.semilogx(data['Elements'], data['byte[$\ell$]32'],  linestyle=style['solid'],  color='#4363d8', label='byte[$\ell$]$32$')
      # plot.semilogx(data['Elements'], data['bit[F]32'],        linestyle=style['solid'],  color='#f58231', label='bit[F]$32$')
      # plot.semilogx(data['Elements'], data['bit[$\ell$]32'],   linestyle=style['solid'],  color='#911eb4', label='bit[$\ell$]$32$')
      # plot.semilogx(data['Elements'], data['fixed[26]bit32'],  linestyle=style['solid'],  color='#42d4f4', label='fixed[26]bit$32$')
      # plot.semilogx(data['Elements'], data['fixed[26]bit32'],  linestyle=style['solid'],  color='#f032e6', label='fixed[26]bit$32$')
      # plot.semilogx(data['Elements'], data['fixed[32]bit32'],  linestyle=style['solid'],  color='#bfef45', label='fixed[32]bit$32$')
      # plot.semilogx(data['Elements'], data['fixed[32]bit32'],  linestyle=style['solid'],  color='#fabebe', label='fixed[32]bit$32$')
        ###############################################################################
      # plot.semilogx(data['Elements], data['Prezza'],    linestyle='-', color='#000000', label='Prezza')
        ##########################################################################################################################
        pyplot.title('Rank Select: {}'.format(title), fontsize='large')
        pyplot.xlabel('$\#$ elements (bit)', fontsize='medium')
        pyplot.ylabel('time (nS)', fontsize='medium')
        pyplot.legend(loc="upper left", ncol=2, shadow=True, fancybox=True, prop={'size': 6}, framealpha=1, frameon=True)
        #pyplot.legend(loc="upper left", bbox_to_anchor=[0, 1], ncol=4, shadow=True, fancybox=True, prop={'size': 6}, framealpha=1, frameon=True)
        ##########################################################################################################################
        pyplot.savefig('{}/{}.{}'.format(prefix, filename.split('.')[0], 'pdf'), bbox_inches='tight')
        #pyplot.show()


def large_x(fig):
    if fig == 'rank1.csv': return (1*10**7, 3*10**10)
    if fig == 'select1.csv': return (1*10**7, 3*10**10)
    if fig == 'update.csv': return (1*10**7, 3*10**10)
    return None

def large_y(fig):
    if fig == 'rank1.csv': return (20, 110)
    if fig == 'select1.csv': return (0, 600)
    if fig == 'update.csv': return (0, 400)
    return None

if __name__ == '__main__':
    if os.path.exists("./large/"):
        generate('large', large_x, large_y)
    else:
        print("You need to:")
        print("    mkdir large")
