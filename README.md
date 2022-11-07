# Overview

Originally an LP solver written in python utilizing the simplex method for a university project. At the time I attempted to parallelize the code, only to find that python
and it's GIL really don't like that. I am going to now rewrite the program in C++. Then parallelize on the CPU via OpenMP, followed by parallelizing on an Nvidia GPU via CUDA. 
