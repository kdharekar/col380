#!/bin/bash
echo hello__world
#my first bash script

gcc -fopenmp -o0 method0.c -o method0
gcc -fopenmp -o0 method1.c -o method1
gcc -fopenmp -o0 method2.c -o method2
gcc -fopenmp -o0 method3.c -o method3
mpicc -o method4 method4.c

echo hello__world
