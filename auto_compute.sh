#!/bin/bash

# get script directory
DIR=$(dirname "$0")
SCRIPTS="$DIR/calc_scripts"
# string for printing stars
STARS="******"
# dump file names
DUMP="all.dump"
DUMP_C_Z="c60_z.dump"
# zero level
ZERO_LVL="-0.0184635"

# print title
echo "### LAMMPS AUTOCOMPUTE SCRIPT ###"; echo; echo "$STARS"

# set number of OpenMP threads
export OMP_NUM_THREADS=8
echo "set number of OpenMP threads to $OMP_NUM_THREADS"; echo; echo "$STARS"

# run lammps script
echo "running lammps script"; echo " ---"
lmp -sf omp -in fall_s.in
echo; echo "$STARS"

mkdir result

echo "last 10 steps C distribution average calculation"
c60_dist=$("$SCRIPTS/c60_dist/c60_dist" "$DIR/$DUMP_C_Z")
echo $c60_dist > "$DIR/result/c60_dist.vals"
echo; echo "$STARS"
