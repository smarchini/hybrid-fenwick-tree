import gmpy2

w = 64
LL1 = 32768 * 8
LL2 = 262144 * 8
LL3 = 8388608 * 8
page4k = 4096 * 8
page2M = 2048 * 1024 * 8

S = 7

def nu(j):
    assert j >= 0, "nu is undefined"
    return bin(j)[2:].count('1')

def rho(j):
    assert j != 0, "rho is inf"
    return gmpy2.bit_scan1(j)

def lbda(j):
    assert j > 0, "lambda is undefined"
    return len(bin(j)[2:])

def prop2(j):
    return (j) * (S + 1) - nu(j)

def bitlength(j):
    return S + rho(j)

# Bit
##################################
def alignedEnding(j):
    return j * (S + 1) + w - 1

def startingBit(j):
    return prop2(j - 1) + w + 1
##################################

# Byte
##################################
def bytepos(j):
    nextbyte = ((S - 1) | (8 - 1)) + 1
    small = ((S - 1) >> 3) + 1
    medium = nextbyte - S + 1
    large = medium + 8
    multiplier = 8 - small - 1
    value =  j * small + (j >> medium) + (j >> large) * multiplier
    return value * 8
##################################

# Fixed
##################################
def fixedpos(j):
    return 64 * j
##################################


def max_j_bit(size):
    j = 0
    while alignedEnding(j) < size: j += 1
    return j - 1

def max_j_byte(size):
    j = 0
    while bytepos(j) < size: j += 1
    return j - 1

def max_j_fixed(size):
    j = 0
    while fixedpos(j) < size: j += 1
    return j - 1


if __name__ == '__main__':
    max_j_bit_LL1 = max_j_bit(LL1)
    max_j_bit_LL2 = max_j_bit(LL2)
    max_j_bit_LL3 = max_j_bit(LL3)
    max_j_bit_4k = max_j_bit(page4k)
    max_j_bit_2M = max_j_bit(page2M)
    
    max_j_byte_LL1 = max_j_byte(LL1)
    max_j_byte_LL2 = max_j_byte(LL2)
    max_j_byte_LL3 = max_j_byte(LL3)
    max_j_byte_4k = max_j_byte(page4k)
    max_j_byte_2M = max_j_byte(page2M)
    
    max_j_fixed_LL1 = max_j_fixed(LL1)
    max_j_fixed_LL2 = max_j_fixed(LL2)
    max_j_fixed_LL3 = max_j_fixed(LL3)
    max_j_fixed_4k = max_j_fixed(page4k)
    max_j_fixed_2M = max_j_fixed(page2M)

    print('LL1 = {} Byte -> {} bit'.format(LL1 // 8, LL1))
    print('LL2 = {} Byte -> {} bit'.format(LL2 // 8, LL2))
    print('LL3 = {} Byte -> {} bit'.format(LL3 // 8, LL3))
    print('-----------------------------------------------')
    print('Bit: maximum j inside LL1: {} -- C = {}'.format(max_j_bit_LL1, lbda(max_j_bit_LL1)))
    print('Bit: maximum j inside LL2: {} -- C = {}'.format(max_j_bit_LL2, lbda(max_j_bit_LL2))) 
    print('Bit: maximum j inside LL3: {} -- C = {}'.format(max_j_bit_LL3, lbda(max_j_bit_LL3)))
    print('Bit: maximum j inside 4k: {} -- C = {}'.format(max_j_bit_4k, lbda(max_j_bit_4k)))
    print('Bit: maximum j inside 2M: {} -- C = {}'.format(max_j_bit_2M, lbda(max_j_bit_2M)))
    print('-----------------------------------------------')
    print('Byte: maximum j inside LL1: {} -- C = {}'.format(max_j_byte_LL1, lbda(max_j_byte_LL1))) 
    print('Byte: maximum j inside LL2: {} -- C = {}'.format(max_j_byte_LL2, lbda(max_j_byte_LL2))) 
    print('Byte: maximum j inside LL3: {} -- C = {}'.format(max_j_byte_LL3, lbda(max_j_byte_LL3))) 
    print('Byte: maximum j inside 4k: {} -- C = {}'.format(max_j_byte_4k, lbda(max_j_byte_4k)))
    print('Byte: maximum j inside 2M: {} -- C = {}'.format(max_j_byte_2M, lbda(max_j_byte_2M)))
    print('-----------------------------------------------')
    print('Fixed: maximum j inside LL1: {} -- C = {}'.format(max_j_fixed_LL1, lbda(max_j_fixed_LL1))) 
    print('Fixed: maximum j inside LL2: {} -- C = {}'.format(max_j_fixed_LL2, lbda(max_j_fixed_LL2))) 
    print('Fixed: maximum j inside LL3: {} -- C = {}'.format(max_j_fixed_LL3, lbda(max_j_fixed_LL3))) 
    print('Fixed: maximum j inside 4k: {} -- C = {}'.format(max_j_fixed_4k, lbda(max_j_fixed_4k)))
    print('Fixed: maximum j inside 2M: {} -- C = {}'.format(max_j_fixed_2M, lbda(max_j_fixed_2M)))

