#!/usr/bin/env bash

for compiler in "clang" "gcc"
do
    #for MA in "" "-DMA"
    for MA in "" "-march=native" "-march=native -DMA"
    do
        echo "With $compiler $MA"
        echo "32 Bit"
        $compiler mult_32.c -DK=2 -O3 $MA -DNOP -lm
        ./a.out 2> /dev/null
        for K in 3 4
        do
           $compiler mult_32.c -DK=$K -O3 $MA -lm
           ./a.out 2> /dev/null
        done
        echo "64 Bit"
        for K in 2 3 4
        do
           $compiler mult.c -DK=$K -O3 $MA -lm
           ./a.out 2> /dev/null
        done
    done
done
