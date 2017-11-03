import sys, csv
import matplotlib.pyplot as plt
from os.path import basename

def plot(infile, graphtype='semilog'):
    with open(infile, "r") as csvfile:
        title = basename(infile).split('.')[0]
        reader = csv.reader(csvfile, delimiter=',')
        elements = [ r for r in reader ]
        xdata = [ int(elements[i][0]) for i in range(1, len(elements)) ]

        fig = plt.figure(figsize=(9.5,7))
        ax = fig.add_subplot(1,1,1)

        color = ['black', 'b', 'g', 'r', 'c', 'm', 'y', 'gray', 'orange']
        c = 9
        for j in range(1, 1+c):
            ydata = [ float(elements[i][j]) for i in range(1, len(elements)) ]
            ax.semilogx(xdata, ydata, label=elements[0][j], color=color[j-1], linestyle='solid')

        for j in range(1+c, 1+c+c):
            ydata = [ float(elements[i][j]) for i in range(1, len(elements)) ]
            ax.semilogx(xdata, ydata, label=elements[0][j], color=color[j-(1+c)], linestyle='dashed')

        for j in range(1+c+c, 1+c+c+c):
            ydata = [ float(elements[i][j]) for i in range(1, len(elements)) ]
            ax.semilogx(xdata, ydata, label=elements[0][j], color=color[j-(1+c+c)], linestyle='dotted')

        for j in range(1+c+c+c, 1+c+c+c+c):
            ydata = [ float(elements[i][j]) for i in range(1, len(elements)) ]
            ax.semilogx(xdata, ydata, label=elements[0][j], color=color[j-(1+c+c+c)], linestyle='dashdot')

        plt.legend(loc="upper left", bbox_to_anchor=[0, 1], ncol=2, shadow=True, fancybox=True)
        plt.title(title, fontsize='xx-large')
        plt.xlabel("Dimensione del vettore (bit)", fontsize='large')
        plt.ylabel("Tempo d'esecuzione (nsec)", fontsize='large')

        plt.savefig('{}/{}.{}'.format("./images", title, "pdf"))

if __name__ == '__main__':
    plot(*sys.argv[1:])
