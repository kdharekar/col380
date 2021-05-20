#!/bin/bash

#$1 $2 $3


if [[ $4 -eq 0 ]]
then
   ./method0 $1 $2 $3

elif [[ $4 -eq 1 ]]
then
   ./method1 $1 $2 $3


elif [[ $4 -eq 2 ]]
then
   ./method2 $1 $2 $3
elif [[ $4 -eq 3 ]]
then
   ./method3 $1 $2 $3

elif [[ $4 -eq 4 ]]
then
   mpiexec -np $3 ./method4 $1 $2
else
  echo "invalid method"
fi
