#!/bin/bash

# get script directory
DIR=$(dirname "$0")
SCRIPTS="$DIR/calc_scripts"
# string for printing stars
STARS="******"
# input file name
INPUT="fall_s.input.data"
# dump file names
DUMP="all.dump"
DUMP_C_Z="c60_z.dump"
DUMP_VOR="vor_time.dump"
# zero level
ZERO_LVL="-0.0184635"

# print title
echo "### LAMMPS AUTOCOMPUTE SCRIPT ###"; echo; echo "$STARS"

# set number of OpenMP threads
export OMP_NUM_THREADS=8
echo "set number of OpenMP threads to $OMP_NUM_THREADS"; echo; echo "$STARS"

# make a directory to store results
rm -rf result
mkdir result

# variants loop
for i in {1..5}
do
  echo "compute: moved$i"; echo; echo "$STARS"

  RESULTS_DIR="$DIR/result/moved$i"
  NEW_INPUT_DATA="$RESULTS_DIR/input.data"

  mkdir $RESULTS_DIR
  
  echo "moving carbon"
  "$SCRIPTS/C_move/C_move.exe" "$DIR/$INPUT" "$NEW_INPUT_DATA"
  echo; echo "$STARS"
  
  # run lammps script
  echo "running lammps script"; echo " ---"
  lmp -sf omp -in fall_s.in
  echo; echo "$STARS"
  
  # parse carbon z distribution dump
  echo "last 10 steps carbon distribution average calculation"
  "$SCRIPTS/C_z_dist/C_z_dist.exe" "$DIR/$DUMP_C_Z" "$RESULTS_DIR/C_z_dist.vals"
  echo; echo "$STARS"
  
  # parse voro dump
  echo "parsing voronoi time relation dump"
  "$SCRIPTS/Voro_time/Voro_time.exe" "$DIR/$DUMP_VOR" "$RESULTS_DIR/Voro_time.vals"
  echo; echo "$STARS"
done
