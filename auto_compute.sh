#!/bin/bash

# get script directory
DIR=$(dirname "$0")
# string for printing stars
STARS="******"

# print title
echo "### LAMMPS AUTOCOMPUTE SCRIPT ###"; echo; echo "$STARS"

# set number of OpenMP threads
export OMP_NUM_THREADS=8
echo "set number of OpenMP threads to $OMP_NUM_THREADS"; echo; echo "$STARS"

# run lammps script
echo "running lammps script"; echo " ---"
#lmp -sf omp -in fall_s.in
echo "$STARS"

echo $(python c60_distrib.py)
