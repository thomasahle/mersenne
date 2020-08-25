# Takes data in the format from div-test.c and turns it into latex
# Format:
#
# ms per 10000000 divisions
# NOP, Crand, Us, Gnu
# b = 32, 132 526 277 273 
# b = 64, 122 503 276 273 
# b = 128, 136 631 298 366 
# b = 256, 153 576 343 763 
# b = 512, 208 952 470 1380 
# b = 1024, 309 1211 686 2984 

import sys
import re
for line in sys.stdin:
    ns = re.findall(r'\d+', line)
    if len(ns) == 5:
        b, nop, *xs = map(int, ns)
        xs = [x - nop for x in xs]
        print(b, '&', end=' ')
        best = min(range(len(xs)), key=lambda i: xs[i])
        xs[best] = f'\\textbf{{{xs[best]}}}'
        print(' & '.join(map(str, xs)), end=r'\\'+'\n', flush=True)
    elif line.startswith('NOP, '):
        print('$b$ & Crandall & \\Cref{alg:division-generalized} & GMP\\\\', flush=True)
        print('\\hline\\\\')
    else:
        print(line, end='')

