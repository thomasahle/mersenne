#!/usr/bin/env bash
echo "32 Bit"

clang mult_32.c -DK=2 -O3 -DMA -DNOP
./a.out 2> /dev/null

for K in 4 8
do
   clang mult_32.c -DK=$K -O3 -DMA
   ./a.out 2> /dev/null
done
echo "64 Bit"
for K in 2 4 8
do
   clang mult.c -DK=$K -O3
   ./a.out 2> /dev/null
done
