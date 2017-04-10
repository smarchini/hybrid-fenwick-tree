import sys, csv
import matplotlib.pyplot as plt

def plot(infile, outfile):
    with open(infile, "r") as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        elements = [ r for r in reader ]
        xdata = [ int(elements[i][0]) for i in range(1, len(elements)) ]
        ydata1 = [ float(elements[i][1]) for i in range(1, len(elements)) ]
        ydata2 = [ float(elements[i][2]) for i in range(1, len(elements)) ]
        ydata3 = [ float(elements[i][3]) for i in range(1, len(elements)) ]
        ydata4 = [ float(elements[i][4]) for i in range(1, len(elements)) ]
        ydata5 = [ float(elements[i][5]) for i in range(1, len(elements)) ]
        fig = plt.figure()
        ax = fig.add_subplot(1,1,1)
        ax.semilogx(xdata, ydata1, label='simple')
        ax.semilogx(xdata, ydata2, label='typed')
        ax.semilogx(xdata, ydata3, label='byte')
        ax.semilogx(xdata, ydata4, label='compact')
        ax.semilogx(xdata, ydata5, label='shrank')
        plt.legend(loc="upper left", bbox_to_anchor=[0, 1], ncol=2, shadow=True, fancybox=True)
        plt.savefig(outfile)
        plt.show()

if __name__ == '__main__':
    plot(sys.argv[1], 'prova.png')
