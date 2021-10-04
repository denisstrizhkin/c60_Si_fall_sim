#!/bin/bash

# get script directory
DIR=$(dirname "$0")
SCRIPTS="$DIR/calc_scripts"
# string for printing stars
STARS="******"
# output file name
OUTPUT=fall.output.data
# in file name
IN=fall.in
IN_TEMPLATE=template.in
# input file name
INPUT_TEMPLATE=template.input.data
INPUT=fall.input.data
# dump file names
DUMP_ALL=all.dump
DUMP_C_Z=c60_z.dump
DUMP_VOR=vor_time.dump
# zero level
ZERO_LVL="-0.0184635"
# computes speeds
SPEEDS=("115.7" "163.6" "231.4")

# print title
echo "### LAMMPS AUTOCOMPUTE SCRIPT ###"; echo; echo "$STARS"

# set number of OpenMP threads
export OMP_NUM_THREADS=8
echo "set number of OpenMP threads to $OMP_NUM_THREADS"; echo; echo "$STARS"

# make a directory to store results
rm -rf result
mkdir result

# variants loop
for speed_i in "${SPEEDS[@]}"
do
  for move_i in {1..5}
  do
    echo "compute: moved_${move_i}_speed_${speed_i}"; echo; echo "$STARS"
  
    RESULTS_DIR="$DIR/result/moved_${move_i}_speed_${speed_i}"
    NEW_INPUT_DATA="$RESULTS_DIR/$INPUT"
    NEW_IN_DATA="$RESULTS_DIR/$IN"
  
    mkdir $RESULTS_DIR
    
    echo "moving carbon"
    "$SCRIPTS/C_move/C_move.exe" "$INPUT_TEMPLATE" "$NEW_INPUT_DATA"
    cp $NEW_INPUT_DATA $DIR/$INPUT
    echo; echo "$STARS"

    echo "changing .in file"
    echo "# CONSTANTS" > $DIR/$IN
    echo 'variable zero_lvl equal "-0.0184635"' >> $DIR/$IN 
    echo 'variable carbon_speed_down equal "'-${speed_i}'"' >> $DIR/$IN
    awk "NR >= 4" $DIR/$IN_TEMPLATE >> $DIR/$IN
    echo; echo "$STARS"
    
    # run lammps script
    echo "running lammps script"; echo " ---"
    lmp -sf omp -in fall.in
    cp $DIR/$OUTPUT $RESULTS_DIR/$OUTPUT
    cp $DIR/$IN $RESULTS_DIR/$IN
    cp $DIR/$DUMP_C_Z $RESULTS_DIR/$DUMP_C_Z
    cp $DIR/$DUMP_ALL $RESULTS_DIR/$DUMP_ALL
    cp $DIR/$DUMP_VOR $RESULTS_DIR/$DUMP_VOR
    rm $DIR/$INPUT
    echo; echo "$STARS"
    
    # parse carbon z distribution dump
    echo "last 10 steps carbon distribution average calculation"
    $SCRIPTS/C_z_dist/C_z_dist.exe $DIR/$DUMP_C_Z $RESULTS_DIR/C_z_dist.vals
    echo; echo "$STARS"
    
    # parse voro dump
    echo "parsing voronoi time relation dump"
    $SCRIPTS/Voro_time/Voro_time.exe $DIR/$DUMP_VOR $RESULTS_DIR/Voro_time.vals
    echo; echo "$STARS"
  done
done
