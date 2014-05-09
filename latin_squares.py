
def factorial(n):
    if n == 0:
        return 1
    else:
        return factorial(n - 1) * n

def permutationIndex(permutation):
    n = len(permutation)
    elements_remaining = range(n)
    index = 0
    for el in permutation:
        size = factorial(n - 1)
        for i in xrange(len(elements_remaining)):
            if elements_remaining[i] == el:
                del elements_remaining[i]
                break
        index += size * i
        n -= 1
    return index

mappings = [((0, 1),   [0, 1, 2, 3, 4, 5, ]),
            ((0, 2),   [0, 1, 2, 3, 4, 5, ]),
            ((0, 3),   [0, 1, 2, 3, 4, 5, ]),
            ((0, 4),   [0, 1, 2, 3, 4, 5, ]),
            ((0, 5),   [0, 1, 2, 3, 4, 5, ]),
            ((0, 6),   [0, 1, 2, 3, 4, 5, ]),

            ((1, 0),   [0, 1, 2, 3, 4, 5, ]),
            ((1, 2),   [1, 0, 4, 5, 2, 3, ]),
            ((1, 3),   [2, 3, 0, 1, 5, 4, ]),
            ((1, 4),   [3, 4, 5, 0, 1, 2, ]),
            ((1, 5),   [4, 5, 3, 2, 0, 1, ]),
            ((1, 6),   [5, 2, 1, 4, 3, 0, ]),

            ((2, 0),   [0, 1, 2, 3, 4, 5, ]),
            ((2, 1),   [1, 0, 4, 5, 2, 3, ]),
            ((2, 3),   [5, 4, 3, 2, 1, 0, ]),
            ((2, 4),   [2, 5, 0, 4, 3, 1, ]),
            ((2, 5),   [3, 2, 1, 0, 5, 4, ]),
            ((2, 6),   [4, 3, 5, 1, 0, 2, ]),

            ((3, 0),   [0, 1, 2, 3, 4, 5, ]),
            ((3, 1),   [2, 3, 0, 1, 5, 4, ]),
            ((3, 2),   [5, 4, 3, 2, 1, 0, ]),
            ((3, 4),   [4, 2, 1, 5, 0, 3, ]),
            ((3, 5),   [1, 0, 5, 4, 3, 2, ]),
            ((3, 6),   [3, 5, 4, 0, 2, 1, ]),

            ((4, 0),   [0, 1, 2, 3, 4, 5, ]),
            ((4, 1),   [3, 4, 5, 0, 1, 2, ]),
            ((4, 2),   [2, 5, 0, 4, 3, 1, ]),
            ((4, 3),   [4, 2, 1, 5, 0, 3, ]),
            ((4, 5),   [5, 3, 4, 1, 2, 0, ]),
            ((4, 6),   [1, 0, 3, 2, 5, 4, ]),

            ((5, 0),   [0, 1, 2, 3, 4, 5, ]),
            ((5, 1),   [4, 5, 3, 2, 0, 1, ]),
            ((5, 2),   [3, 2, 1, 0, 5, 4, ]),
            ((5, 3),   [1, 0, 5, 4, 3, 2, ]),
            ((5, 4),   [5, 3, 4, 1, 2, 0, ]),
            ((5, 6),   [2, 4, 0, 5, 1, 3, ]),
            
            ((6, 0),   [0, 1, 2, 3, 4, 5, ]),
            ((6, 1),   [5, 2, 1, 4, 3, 0, ]),
            ((6, 2),   [4, 3, 5, 1, 0, 2, ]),
            ((6, 3),   [3, 5, 4, 0, 2, 1, ]),
            ((6, 4),   [1, 0, 3, 2, 5, 4, ]),
            ((6, 5),   [2, 4, 0, 5, 1, 3, ])]

mapDict = {x:y for (x,y) in mappings}

def printLatin(rows):
    for row in rows:
        print row
        
def printReducedLatinSquares(el_ith):
    rows = [((el_ith, i), mapDict[(el_ith, i)]) for i in xrange(7) if i != el_ith]
    squares = sorted(rows, key=lambda x: x[1][0])
    printLatin(squares)

def printLatinSquareIndex(el_ith):
    rows = [mapDict[(el_ith, i)] for i in xrange(7) if i != el_ith]
    squares = sorted(rows, key=lambda x: x[0])
    for el in map(permutationIndex, squares):
        print el


for i in xrange(7):
    print ""
    printReducedLatinSquares(i)
    #printLatinSquareIndex(i)


