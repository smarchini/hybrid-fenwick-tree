#example: python pretty_print.py <(../bin/benchmark/get)
#example: ./bin/benchmark/get > get.csv & python ./utils/pretty_print.py get.csv
import sys, csv

def prettyprint(filename):
    with open(filename, "r") as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        elements = [ r for r in reader ]
        lenghts = [ max(len(elements[i][k]) for i in range(len(elements))) for k in range(len(elements[0])) ]
        print(' | '.join('{:{width}}'.format(elements[0][col], width=lenghts[col]) for col in range(len(elements[0]))))
        print('-' * (sum(lenghts) + len(lenghts)*3))
        for row in range(1, len(elements)):
            print(' | '.join('{:{width}}'.format(elements[row][col], width=lenghts[col]) for col in range(len(elements[row]))))

if __name__ == '__main__':
    for f in sys.argv[1:]:
        prettyprint(f)
        print()
