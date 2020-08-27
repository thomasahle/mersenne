K, L = 2, 2
K, L = 3, 4
K, L = 4, 7
cs = [sum((i+1)*(j+1) << 64*j for j in range(L)) for i in range(K)]
x = 100

res = cs[0]
for c in cs[1:]:
    res = x*res + c
print(cs)
print(res)
print((res >> 64*(L-1)) & ((1<<64)-1))
