#!/bin/sh

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
zero_lvl="0"

# computes speeds
speeds=""
speeds="${speeds} 115.7"
speeds="${speeds} 163.6"
speeds="${speeds} 231.4"
speeds="${speeds} 283.4"
speeds="${speeds} 365.88"

x_vels=""
x_vels="${x_vels} 79.1"
x_vels="${x_vels} 148.7"
x_vels="${x_vels} 200.4"
x_vels="${x_vels} 227.9"
x_vels_at() { echo ${x_vels} | cut -d" " -f${1}; }

z_vels=""
z_vels="${z_vels} 217.4"
z_vels="${z_vels} 177.3"
z_vels="${z_vels} 115.7"
z_vels="${z_vels} 40.2"
z_vels_at() { echo ${z_vels} | cut -d" " -f${1}; }

# computes angles
angles=""
#angles="${angles} 20"
#angles="${angles} 40"
angles="${angles} 50"
#angles="${angles} 60"
angles="${angles} 70"
#angles="${angles} 80"
angles_at() { echo ${angles} | cut -d" " -f${1}; }

# string for printing stars
stars="******"

# remove temporary files left after lammps script execution
clean() {
  # rm input.data
  rm -f $script_dir/$input_file
  # rm output.data
  rm -f $script_dir/$output
  # rm .in file
  rm -f $script_dir/$in_file 
  # rm dumps
  rm -f $script_dir/$dump_last_step
  rm -f $script_dir/$dump_last_10
  rm -f $script_dir/$dump_all
  rm -f $script_dir/$dump_vor
  # rm log.lammps
  rm -f $script_dir/$log

  echo "removed temp compute files"; echo; echo "$stars"
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
  "$data_parser" 'a' "$1" "$1" "$2"
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
  compute_dir=$1
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
  #mpirun --use-hwthread-cpus -np 8 lmp_mpi -in "$script_dir/$in_file" 
  #mpirun -np 4 lmp_omp -sf omp -pk omp 4 -in "$script_dir/$in_file"
  lmp_omp -sf omp -in "$script_dir/$in_file" 
}

# parse lammps dump files
parse_dump_files() {
  compute_dir=$1
  #parse carbon z distribution dump
  echo "last 10 steps carbon distribution average calculation"
  $data_parser "c" $script_dir/$dump_last_10 $compute_dir/C_z_dist.vals "temp"
  echo; echo "$stars"
  
  # parse voro dump
  echo "parsing voronoi time relation dump"
  $data_parser "v" $script_dir/$dump_vor $compute_dir/Voro_time.vals "temp"
  echo; echo "$stars"
}

# get number of OpenMP threads
get_omp_num_threads() {
  if [ -z "$1" ]; then
    omp_num_threads=8
  else
    omp_num_threads="$1"
  fi

  printf "%d" $omp_num_threads
}

# auto computation for straight fall with different speeds and positions
straight_fall() {
  set_omp_num_threads $(get_omp_num_threads $1)
  
  # variants loop
  for speed_i in ${speeds}
  do
    for move_i in $(seq 1 5)
    do
      compute_name="moved_${move_i}_speed_${speed_i}"     
      echo "compute: $compute_name"; echo; echo "$stars"
     
      compute_dir="$results_dir/$compute_name"
      new_input_data="$compute_dir/$input_file"
      new_in_data="$compute_dir/$in_file"
      
      rm -rf "$compute_dir"; mkdir "$compute_dir" 

      randomize_carbon_xy_position "$new_input_data" "0"
      change_template_in_file "-$speed_i" "0" 
      
      run_lammps_script 
      
      copy_lammps_results "$compute_dir"
      parse_dump_files "$compute_dir"
      clean
    done
  done
}

angle_fall() {
  set_omp_num_threads $(get_omp_num_threads $1)
  
  # variants loop
  for angle_i in $(seq 1 2)
  do 
    for move_i in $(seq 1 5)
    do
      compute_name="moved_${move_i}_angle_$(angles_at $angle_i)"     
      echo "compute: $compute_name"; echo; echo "$stars"
     
      compute_dir="$results_dir/$compute_name"
      new_input_data="$compute_dir/$input_file"
      new_in_data="$compute_dir/$in_file"
      
      rm -rf "$compute_dir"; mkdir "$compute_dir" 

      randomize_carbon_xy_position "$new_input_data" "$(angles_at $angle_i)"
      change_template_in_file "-$(z_vels_at $angle_i)" "-$(x_vels_at $angle_i)"
      
      run_lammps_script 
      
      copy_lammps_results "$compute_dir"
      parse_dump_files "$compute_dir"
      clean
    done
  done
}

above_surface() {
  vals=$RESULT_DIR/above_surface.vals
  rm -rf $vals
  touch $vals

  #thresholds=("0" "0.2" "0.4" "0.6" "0.8" "1")
  #thresholdss=("0" "0,2" "0,4" "0,6" "0,8" "1")
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

above_surface() {
  TMP=$RESULT_DIR/above_surface.vals
  rm -rf $TMP
  touch $TMP

 # thresholds=("0" "0.2" "0.4" "0.6" "0.8" "1")
 # thresholdss=("0" "0,2" "0,4" "0,6" "0,8" "1")
  printf "************" >> $TMP
  for threshold_i in {0..5}
  do
    printf "__%1.1f" ${thresholdss[threshold_i]} >> $TMP
  done
  echo "" >> $TMP

  for move_i in {1..5}
  do
    for angle_i in {0..3}
    do
      printf "m: %2d,a: %2d|" $move_i ${ANGLES[angle_i]} >> $TMP

      COMPUTE_NAME="angle_${ANGLES[angle_i]}_move_${move_i}"
      echo "compute: $COMPUTE_NAME"; echo; echo "$STARS"
  
      COMPUTE_DIR="$RESULT_DIR/$COMPUTE_NAME"
      for threshold_i in {0..5}
      do
        $DATA_PARSER "u" $COMPUTE_DIR/$DUMP_LAST10 $RESULT_DIR/tmp "${thresholds[threshold_i]}"
        tmp=$(cat $RESULT_DIR/tmp)
        printf "%5d" $tmp >> $TMP
      done
      echo "" >> $TMP
    done
  done
}

echo "### LAMMPS AUTOCOMPUTE SCRIPT ###"; echo; echo "$stars"  
mkdir -p "$results_dir"

"$1" "$2"
