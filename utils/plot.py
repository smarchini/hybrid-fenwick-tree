import sys, csv
import matplotlib.pyplot as plt

def plot(infile, outfile):
    with open(infile, "r") as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        elements = [ r for r in reader ]
        xdata = [ int(elements[i][0]) for i in range(1, len(elements)) ]

        fig = plt.figure()
        ax = fig.add_subplot(1,1,1)

        for j in range(1, len(elements[0])):
            ydata = [ float(elements[i][j]) for i in range(1, len(elements)) ]
            ax.plot(xdata, ydata, label=elements[0][j])

        plt.legend(loc="upper left", bbox_to_anchor=[0, 1], ncol=2, shadow=True, fancybox=True)
        plt.savefig(outfile)
        plt.show()

if __name__ == '__main__':
    plot(sys.argv[1], 'linear.png')
