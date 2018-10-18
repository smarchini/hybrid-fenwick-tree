import sys, numpy, matplotlib
import matplotlib.pyplot as pyplot
from matplotlib.ticker import FuncFormatter
from os.path import basename

def ns_to_sec(x, pos):
    return '%1.0f' % (x*1.1e-9)

if __name__ == '__main__':
    for fname in sys.argv[1:]:
        title = basename(fname).split('.')[0]
        data = numpy.genfromtxt(fname, delimiter=',', missing_values=set([0, '0']), names=True, autostrip=True, case_sensitive=True, deletechars='', dtype=None)
        plot = pyplot.figure().add_subplot(1,1,1)
        matplotlib.rcParams.update({'font.size': 10})
        ########################################################################################################################################
        plot.semilogx(data['Elements'], data['fixed[$1$]'],  color='#999999', linestyle='-',  label='fixed[$1$]')
        plot.semilogx(data['Elements'], data['byte[$1$]'],   color='#999999', linestyle='--', label='byte[$1$]')
        plot.semilogx(data['Elements'], data['bit[$1$]'],    color='#999999', linestyle=':',  label='bit[$1$]')
        ####################################
        plot.semilogx(data['Elements'], data['fixed[$2$]'],  color='#ff0000', linestyle='-',  label='fixed[$2$]')
        plot.semilogx(data['Elements'], data['byte[$2$]'],   color='#ff0000', linestyle='--', label='byte[$2$]')
        plot.semilogx(data['Elements'], data['bit[$2$]'],    color='#ff0000', linestyle=':',  label='bit[$2$]')
        ####################################
        plot.semilogx(data['Elements'], data['fixed[$4$]'], color='#990000', linestyle='-',  label='fixed[$4$]')
        plot.semilogx(data['Elements'], data['byte[$4$]'],  color='#990000', linestyle='--', label='byte[$4$]')
        plot.semilogx(data['Elements'], data['bit[$4$]'],   color='#990000', linestyle=':',  label='bit[$4$]')
        ####################################
        plot.semilogx(data['Elements'], data['fixed[$8$]'], color='#00ff00', linestyle='-',  label='fixed[$8$]')
        plot.semilogx(data['Elements'], data['byte[$8$]'],  color='#00ff00', linestyle='--', label='byte[$8$]')
        plot.semilogx(data['Elements'], data['bit[$8$]'],   color='#00ff00', linestyle=':',  label='bit[$8$]')
        ####################################
        plot.semilogx(data['Elements'], data['fixed[$12$]'], color='#0000ff', linestyle='-',  label='fixed[$12$]')
        plot.semilogx(data['Elements'], data['byte[$12$]'],  color='#0000ff', linestyle='--', label='byte[$12$]')
        plot.semilogx(data['Elements'], data['bit[$12$]'],   color='#0000ff', linestyle=':',  label='bit[$12$]')
        ####################################
        plot.semilogx(data['Elements'], data['inverse'],     color='#000000', linestyle='-.', label='inverse')
        plot.semilogx(data['Elements'], data['linear'],      color='#000000', linestyle='--', label='linear')
        plot.semilogx(data['Elements'], data['bubble'],      color='#000000', linestyle=':',  label='bubble')
        ########################################################################################################################################
        pyplot.title('Kemeny distance', fontsize='large')
        pyplot.xlabel('# elements', fontsize='medium')
        pyplot.ylabel('time (nS)', fontsize='medium')
        pyplot.legend(loc="upper left", bbox_to_anchor=[0, 1], ncol=3, shadow=True, fancybox=True, prop={'size': 10})
        pyplot.gca().yaxis.set_major_formatter(FuncFormatter(ns_to_sec))
        ########################################################################################################################################
        pyplot.savefig('{}/{}.{}'.format('./kenemypdf', title, 'pdf'))
        #pyplot.show()
