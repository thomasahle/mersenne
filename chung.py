import itertools
def div(x, n, c):
    t = 2**n - c
    r, q = x, 0
    steps = 0
    while r > t:
    #while r >= t:
        A = r >> n
        B = r & (1<<n) - 1
        q = q + A
        r = B + c*A
        steps += 1
        if steps > 10:
            print((x,n,c), q, r)
            break
    return q, r, steps

def div2(x, n, c):
    qs = [x >> n]
    rs = [x & 2**n-1]
    q, r = qs[0], rs[0]
    i = 0
    while qs[i] > 0:
        t = qs[i] * c
        qs.append(t >> n)
        rs.append(t & 2**n-1)
        q += qs[i+1]
        r += rs[i+1]
        i += 1
    t = 2**n - c
    while r >= t:
        r -= t
        q += 1
    return q, r, i

def test(f, n, c):
    #for x in itertools.count():
    for x in range(2**n-c, 2**(2*n)):
        p = 2**n - c
        truth = x // p
        q, r, steps = f(x, n, c)
        if truth != q:
        #if q*p+r != x:
            print(f'fejl {x}/{2**n-c}={truth} != {q}. {q}*{2**n-c}+{r}={x}?')
            return x
        print(x, steps)

for n in range(2,4):
    for c in range(2**(n//2)):
        print('nc', n, c)
        best =  test(div2, n, c)
        #print('Good up to', best)
