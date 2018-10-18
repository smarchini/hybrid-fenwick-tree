import sys, numpy, matplotlib
import matplotlib.pyplot as pyplot
from os.path import basename

if __name__ == '__main__':
    for fname in sys.argv[1:]:
        title = basename(fname).split('.')[0]
        data = numpy.genfromtxt(fname, delimiter=',', missing_values=set([0, '0']), names=True, autostrip=True, case_sensitive=True)
        plot = pyplot.figure().add_subplot(1,1,1)
        matplotlib.rcParams.update({'font.size': 6})
        ########################################################################################################################################
        plot.semilogx(data['Elements'], data['Naive'],         color='#ff0000', linestyle='-',  label='classic uncompress')
        plot.semilogx(data['Elements'], data['LNaive'],        color='#ff0000', linestyle='--', label='levelord uncompress')
        plot.semilogx(data['Elements'], data['Byte'],          color='#00ff00', linestyle='-',  label='classic byte-compress')
        plot.semilogx(data['Elements'], data['LByte'],         color='#00ff00', linestyle='--', label='levelord byte-compress')
        plot.semilogx(data['Elements'], data['Bit'],           color='#0000ff', linestyle='-',  label='classic bit-compress')
        plot.semilogx(data['Elements'], data['LBit'],          color='#0000ff', linestyle='--', label='levelord bit-compress')
        plot.semilogx(data['Elements'], data['LNaiveNaive16'], color='#ffff00', linestyle=':',  label='hybird uncompressed')
        plot.semilogx(data['Elements'], data['LByteByte16'],   color='#00ffff', linestyle=':',  label='hybird byte-compress')
        plot.semilogx(data['Elements'], data['LBitBit16'],     color='#ff00ff', linestyle=':',  label='hybird bit-compress')
        plot.semilogx(data['Elements'], data['LByteBit16'],    color='#0ffff0', linestyle=':',  label='hybird byte/bit-compress')
        ####################################
        plot.semilogx(data['Elements'], data['Naive_16'],         color='#bb0000', linestyle='-',  label='16w classic uncompress')
        plot.semilogx(data['Elements'], data['LNaive_16'],        color='#bb0000', linestyle='--', label='16w levelord uncompress')
        plot.semilogx(data['Elements'], data['Byte_16'],          color='#00bb00', linestyle='-',  label='16w classic byte-compress')
        plot.semilogx(data['Elements'], data['LByte_16'],         color='#00bb00', linestyle='--', label='16w levelord byte-compress')
        plot.semilogx(data['Elements'], data['Bit_16'],           color='#0000bb', linestyle='-',  label='16w classic bit-compress')
        plot.semilogx(data['Elements'], data['LBit_16'],          color='#0000bb', linestyle='--', label='16w levelord bit-compress')
        plot.semilogx(data['Elements'], data['LNaiveNaive16_16'], color='#bbbb00', linestyle=':',  label='16w hybird uncompressed')
        plot.semilogx(data['Elements'], data['LByteByte16_16'],   color='#00bbbb', linestyle=':',  label='16w hybird byte-compress')
        plot.semilogx(data['Elements'], data['LBitBit16_16'],     color='#bb00bb', linestyle=':',  label='16w hybird bit-compress')
        plot.semilogx(data['Elements'], data['LByteBit16_16'],    color='#0bbbb0', linestyle=':',  label='16w hybird byte/bit-compress')
        ####################################
        plot.semilogx(data['Elements'], data['Naive_32'],         color='#880000', linestyle='-',  label='32w classic uncompress')
        plot.semilogx(data['Elements'], data['LNaive_32'],        color='#880000', linestyle='--', label='32w levelord uncompress')
        plot.semilogx(data['Elements'], data['Byte_32'],          color='#008800', linestyle='-',  label='32w classic byte-compress')
        plot.semilogx(data['Elements'], data['LByte_32'],         color='#008800', linestyle='--', label='32w levelord byte-compress')
        plot.semilogx(data['Elements'], data['Bit_32'],           color='#000088', linestyle='-',  label='32w classic bit-compress')
        plot.semilogx(data['Elements'], data['LBit_32'],          color='#000088', linestyle='--', label='32w levelord bit-compress')
        plot.semilogx(data['Elements'], data['LNaiveNaive16_32'], color='#888800', linestyle=':',  label='32w hybird uncompressed')
        plot.semilogx(data['Elements'], data['LByteByte16_32'],   color='#008888', linestyle=':',  label='32w hybird byte-compress')
        plot.semilogx(data['Elements'], data['LBitBit16_32'],     color='#880088', linestyle=':',  label='32w hybird bit-compress')
        plot.semilogx(data['Elements'], data['LByteBit16_32'],    color='#088880', linestyle=':',  label='32w hybird byte/bit-compress')
        ####################################
        plot.semilogx(data['Elements'], data['Naive_64'],         color='#440000', linestyle='-',  label='64w classic uncompress')
        plot.semilogx(data['Elements'], data['LNaive_64'],        color='#440000', linestyle='--', label='64w levelord uncompress')
        plot.semilogx(data['Elements'], data['Byte_64'],          color='#004400', linestyle='-',  label='64w classic byte-compress')
        plot.semilogx(data['Elements'], data['LByte_64'],         color='#004400', linestyle='--', label='64w levelord byte-compress')
        plot.semilogx(data['Elements'], data['Bit_64'],           color='#000044', linestyle='-',  label='64w classic bit-compress')
        plot.semilogx(data['Elements'], data['LBit_64'],          color='#000044', linestyle='--', label='64w levelord bit-compress')
        plot.semilogx(data['Elements'], data['LNaiveNaive16_64'], color='#444400', linestyle=':',  label='64w hybird uncompressed')
        plot.semilogx(data['Elements'], data['LByteByte16_64'],   color='#004444', linestyle=':',  label='64w hybird byte-compress')
        plot.semilogx(data['Elements'], data['LBitBit16_64'],     color='#440044', linestyle=':',  label='64w hybird bit-compress')
        plot.semilogx(data['Elements'], data['LByteBit16_64'],    color='#044440', linestyle=':',  label='64w hybird byte/bit-compress')
        ########################################################################################################################################
        pyplot.title('Rank & Select: {}'.format(title), fontsize='large')
        pyplot.xlabel('vector size (bit)', fontsize='medium')
        pyplot.ylabel('time (nSec)', fontsize='medium')
        pyplot.legend(loc="upper left", bbox_to_anchor=[0, 1], ncol=4, shadow=True, fancybox=True, prop={'size': 4})
        ########################################################################################################################################
        pyplot.savefig('{}/{}.{}'.format('./rankselectpdf', title, 'pdf'))
        #pyplot.show()
