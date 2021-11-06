#!/bin/bash

### PATHS ###

# get script path
script_dir="$(dirname "$0")"

# folders
results_dir="$script_dir/results"
templates_dir="$script_dir/templates"

# lammps data parser
data_parser="$script_dir/lammps_data_parser/lammps_data_parser"

# templates 
in_template="$templates_dir/template.in"
input_template="$templates_dir/template.input.data"

### FILE NAMES ###

# log file
log="log.lammps"

# output file 
output="fall.output.data"

# .in file
in_file="fall.in"

# input file
input_file="fall.input.data"

# dump files
dump_all="all.dump"
dump_last_10="last_10.dump"
dump_last_step="last_step.dump"
dump_vor="vor_time.dump"

### OTHER VARS

# zero level
zero_lvl="-0.0184635"

# computes speeds
speeds=("115.7" "163.6" "231.4" "283.4" "365.88")
x_velocities=("79.1" "148.7" "200.4" "227.9")
z_velocities=("217.4" "177.3" "115.7" "40.2")

# computes angles
angles=(20 40 60 80)

# string for printing stars
stars="******"

# remove temporary files left after lammps script execution
clean() {
  # rm input.data
  rm $script_dir/$input_file
  # rm output.data
  rm $script_dir/$output
  # rm .in file
  rm $script_dir/$in_file 
  # rm dumps
  rm $script_dir/$dump_last_step
  rm $script_dir/$dump_last_10
  rm $script_dir/$dump_all
  rm $script_dir/$dump_vor
  # rm log.lammps
  rm $script_dir/$log
}

# get lammps data parser
get_parser() {
  rm -rf "lammps_data_parser"
  git clone https://github.com/denisstrizhkin/lammps_data_parser
  cd lammps_data_parser
  cmake .
  make lammps_data_parser
}

# set number of OpenMP threads
set_omp_num_threads() {
  export OMP_NUM_THREADS="$1"
  echo "set number of OpenMP threads to $OMP_NUM_THREADS";
  echo; echo "$stars" 
}

# move x and y positions of all carbon atoms by a small random delta
randomize_carbon_xy_position() {
  echo "moving carbon"
  "$data_parser" 'r' "$input_template" "$1" "temp"
  cp "$1" $script_dir/$input_file
  echo; echo "$stars" 
}

# change template .in file by assigning zero_lvl and x, z velocities for simulation
change_template_in_file() {
  echo "changing .in file"
  echo "# CONSTANTS" > $script_dir/$in_file
  echo 'variable zero_lvl equal "'$zero_lvl'"' >> $script_dir/$in_file 
  echo 'variable carbon_vz equal "'"$1"'"' >> $script_dir/$in_file
  echo 'variable carbon_vx equal "'"$2"'"' >> $script_dir/$in_file
  awk "NR >= 5" $in_template >> $script_dir/$in_file
  echo; echo "$stars"
}

copy_lammps_results() {
  # cp output.data
  cp $script_dir/$output $compute_dir/$output
  # cp .in file
  cp $script_dir/$in_file $compute_dir/$in_file 
  # cp dumps
  cp $script_dir/$dump_last_step $compute_dir/$dump_last_step
  cp $script_dir/$dump_last_10 $compute_dir/$dump_last_10
  cp $script_dir/$dump_all $compute_dir/$dump_all
  cp $script_dir/$dump_vor $compute_dir/$dump_vor
  # cp log.lammps
  cp $script_dir/$log $compute_dir/$log
  echo; echo "$stars"
}

# running lammps script
run_lammps_script() {
  echo "running lammps script"; echo " ---"
  lmp -sf omp -in "$script_dir/$in_file" 
}

# parse lammps dump files
parse_dump_files() {
  #parse carbon z distribution dump
  echo "last 10 steps carbon distribution average calculation"
  $data_parser "c" $script_dir/$dump_last_10 $compute_dir/C_z_dist.vals "temp"
  echo; echo "$stars"
  
  # parse voro dump
  echo "parsing voronoi time relation dump"
  $data_parser "v" $script_dir/$dump_vor $compute_dir/Voro_time.vals "temp"
  echo; echo "$stars"
}

setup_compute_dir() {
  echo "compute: $compute_name"; echo; echo "$stars"
  
  compute_dir="$results_dir/$compute_name"
  new_input_data="$compute_dir/$input_file"
  new_in_data="$compute_dir/$in_file"
  
  rm -rf "$compute_dir"; mkdir "$compute_dir" 
}

# auto computation for straight fall with different speeds and positions
straight_fall() {
  set_omp_num_threads 8
  
  # variants loop
  for speed_i in "${speeds[@]}"
  do
    for move_i in {1..5}
    do
      compute_name="moved_${move_i}_speed_${speed_i}"     
      setup_compute_dir
     
      randomize_carbon_xy_position "$new_input_data"
      change_template_in_file "-$speed_i" "0" 
      
      run_lammps_script 
      
      copy_lammps_results  
      parse_dump_files 
      clean
    done
  done
}

above_surface() {
  vals=$RESULT_DIR/above_surface.vals
  rm -rf $vals
  touch $vals

  thresholds=("0" "0.2" "0.4" "0.6" "0.8" "1")
  thresholdss=("0" "0,2" "0,4" "0,6" "0,8" "1")
  printf "****************" >> $vals
  for threshold_i in {0..5}
  do
    printf "__%1.1f" ${thresholdss[threshold_i]} >> $vals
  done
  echo "" >> $vals

  for speed_i in "${speeds[@]}"
  do
    for move_i in {1..5}
    do
      printf "m: %2d,a: %3.2f|" $move_i $speed_i >> $vals

      COMPUTE_NAME="moved_${move_i}_speed_${speed_i}"
      echo "compute: $COMPUTE_NAME"; echo; echo "$stars"
  
      COMPUTE_DIR="$RESULT_DIR/$COMPUTE_NAME"
      for threshold_i in {0..5}
      do
        $data_parser "u" $COMPUTE_DIR/$dump_last_10 $RESULT_DIR/tmp "${thresholds[threshold_i]}"
        data=$(cat $RESULT_DIR/tmp)
        printf "%5d" $data >> $vals
      done
      echo "" >> $vals
    done
  done
}

echo "### LAMMPS AUTOCOMPUTE SCRIPT ###"; echo; echo "$stars"  
mkdir "$results_dir"

if declare -f "$1" > /dev/null
then
  # call arguments verbatim
  "$@"
else
  # Show a helpful error
  echo "'$1' is not a known function name" >&2
  exit 1
fi
