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
        plot = pyplot.figure(figsize=[3.5, 7.0]).add_subplot(1,1,1, xlim=setxlim(filename), ylim=setylim(filename))
        matplotlib.rcParams.update({'font.size': 8})
        ########################################################################################################################################
        # NOTE: Comment what you want to hide
        ########################################################################################################################################
        plot.semilogx(data['Elements'], data['fixed[F]'],         linestyle=style['solid'],  color='#000000', label='fixed[F]')                   #
        plot.semilogx(data['Elements'], data['fixed[$\ell$]'],    linestyle=style['solid'],  color='#999999', label='fixed[$\ell$]')              #
        plot.semilogx(data['Elements'], data['byte[F]'],          linestyle=style['solid'],  color='#ff0000', label='byte[F]')                    #
        plot.semilogx(data['Elements'], data['byte[$\ell$]'],     linestyle=style['solid'],  color='#ff34ff', label='byte[$\ell$]')               #
        plot.semilogx(data['Elements'], data['bit[F]'],           linestyle=style['solid'],  color='#00ff00', label='bit[F]')                     #
        plot.semilogx(data['Elements'], data['bit[$\ell$]'],      linestyle=style['solid'],  color='#0000ff', label='bit[$\ell$]')                #
        ####################################
      # plot.semilogx(data['Elements'], data['fixed[18]byte'],  linestyle=style['densely dotted'],  label='fixed[18]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[18]bit'],   linestyle=style['densely dotted'],  label='fixed[18]bit')   #color='#00ff00',
      # plot.semilogx(data['Elements'], data['fixed[19]byte'],  linestyle=style['densely dotted'],  label='fixed[19]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[19]byte'],  linestyle=style['densely dotted'],  label='fixed[19]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[20]byte'],  linestyle=style['densely dotted'],  label='fixed[20]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[20]byte'],  linestyle=style['densely dotted'],  label='fixed[20]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[21]byte'],  linestyle=style['densely dotted'],  label='fixed[21]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[21]byte'],  linestyle=style['densely dotted'],  label='fixed[21]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[22]byte'],  linestyle=style['densely dotted'],  label='fixed[22]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[22]byte'],  linestyle=style['densely dotted'],  label='fixed[22]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[23]byte'],  linestyle=style['densely dotted'],  label='fixed[23]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[23]byte'],  linestyle=style['densely dotted'],  label='fixed[23]byte')  #color='#ff34ff',
      # plot.semilogx(data['Elements'], data['fixed[24]bit'],   linestyle=style['densely dotted'],  label='fixed[24]bit')   #color='#00ff00',
      # plot.semilogx(data['Elements'], data['fixed[24]bit'],   linestyle=style['densely dotted'],  label='fixed[24]bit')   #color='#00ff00',
      # plot.semilogx(data['Elements'], data['fixed[25]bit'],   linestyle=style['densely dotted'],  label='fixed[25]bit')   #color='#00ff00',
      # plot.semilogx(data['Elements'], data['fixed[25]bit'],   linestyle=style['densely dotted'],  label='fixed[25]bit')   #color='#00ff00',
        plot.semilogx(data['Elements'], data['fixed[26]bit'],   linestyle=style['densely dotted'],  label='fixed[26]bit')   #color='#00ff00',
        plot.semilogx(data['Elements'], data['fixed[26]byte'],   linestyle=style['densely dotted'],  label='fixed[26]byte')   #color='#00ff00',
      # plot.semilogx(data['Elements'], data['fixed[32]bit'],   linestyle=style['densely dotted'],  label='fixed[32]bit')   #color='#00ff00',
      # plot.semilogx(data['Elements'], data['fixed[32]byte'],   linestyle=style['densely dotted'],  label='fixed[32]byte')   #color='#00ff00',
        ########################################################################################################################################
        pyplot.title('Fenwick tree: {}'.format(title), fontsize='large')
        pyplot.xlabel('$\#$ elements', fontsize='medium')
        pyplot.ylabel('time (nS)', fontsize='medium')
        pyplot.legend(loc="upper left", ncol=2, shadow=True, fancybox=True, prop={'size': 6}, framealpha=1, frameon=True)
        ########################################################################################################################################
        pyplot.savefig('{}/{}.{}'.format(prefix, filename.split('.')[0], 'pdf'), bbox_inches='tight')
        #pyplot.show()

def large_x(fig):
    if fig == 'prefix.csv': return (1*10**5, 3*10**9)
    if fig == 'add.csv': return (1*10**5, 3*10**9)
    if fig == 'find.csv': return (1*10**5, 3*10**9)
    return None

def large_y(fig):
    if fig == 'prefix.csv': return (20, 180)
    if fig == 'add.csv': return (25, 300)
    if fig == 'find.csv': return (50, 450)
    return None

if __name__ == '__main__':
    if os.path.exists("./large/"):
        generate('large', large_x, large_y)
    else:
        print("You need to:")
        print("    mkdir large")
