#!/bin/bash 

for file in ./input1/*
do

    echo "---------------------------------------------------------------"
    echo "Input LP is ${file##*/}"
    echo "Program Output is: "
    python3 lpsolver.py < ${file}
    echo "Correct Output is: "
    cat ./output1/${file##*/}
    echo "---------------------------------------------------------------"
    echo 
    wait
done