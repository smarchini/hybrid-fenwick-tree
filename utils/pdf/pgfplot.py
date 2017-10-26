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

        plt.savefig('{}/{}.{}'.format("./images", title, "pgf"))
        #plt.show()

if __name__ == '__main__':
    plot(*sys.argv[1:])





# Other Parameters
# ----------------
# **kwargs :
#     Keyword arguments control the :class:`~matplotlib.lines.Line2D`
#     properties:
#
#       agg_filter: unknown
#   alpha: float (0.0 transparent through 1.0 opaque)
#   animated: [True | False]
#   antialiased or aa: [True | False]
#   clip_box: a :class:`matplotlib.transforms.Bbox` instance
#   clip_on: [True | False]
#   clip_path: [ (:class:`~matplotlib.path.Path`, :class:`~matplotlib.transforms.Transform`) | :class:`~matplotlib.patches.Patch` | None ]
#   color or c: any matplotlib color
#   contains: a callable function
#   dash_capstyle: ['butt' | 'round' | 'projecting']
#   dash_joinstyle: ['miter' | 'round' | 'bevel']
#   dashes: sequence of on/off ink in points
#   drawstyle: ['default' | 'steps' | 'steps-pre' | 'steps-mid' | 'steps-post']
#   figure: a :class:`matplotlib.figure.Figure` instance
#   fillstyle: ['full' | 'left' | 'right' | 'bottom' | 'top' | 'none']
#   gid: an id string
#   label: string or anything printable with '%s' conversion.
#   linestyle or ls: ['solid' | 'dashed', 'dashdot', 'dotted' | (offset, on-off-dash-seq) | ``'-'`` | ``'--'`` | ``'-.'`` | ``':'`` | ``'None'`` | ``' '`` | ``''``]
#   linewidth or lw: float value in points
#   marker: :mod:`A valid marker style <matplotlib.markers>`
#   markeredgecolor or mec: any matplotlib color
#   markeredgewidth or mew: float value in points
#   markerfacecolor or mfc: any matplotlib color
#   markerfacecoloralt or mfcalt: any matplotlib color
#   markersize or ms: float
#   markevery: [None | int | length-2 tuple of int | slice | list/array of int | float | length-2 tuple of float]
#   path_effects: unknown
#   picker: float distance in points or callable pick function ``fn(artist, event)``
#   pickradius: float distance in points
#   rasterized: [True | False | None]
#   sketch_params: unknown
#   snap: unknown
#   solid_capstyle: ['butt' | 'round' |  'projecting']
#   solid_joinstyle: ['miter' | 'round' | 'bevel']
#   transform: a :class:`matplotlib.transforms.Transform` instance
#   url: a url string
#   visible: [True | False]
#   xdata: 1D array
#   ydata: 1D array
#   zorder: any number
