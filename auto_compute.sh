#!/bin/sh

### PATHS ###

# get script path
script_dir="$(dirname "$0")"

# sources
. "$script_dir/functions/run_lammps.sh"

# folders
results_dir="$script_dir/results"
in_files_dir="$script_dir/in_files"
input_files_dir="$script_dir/input_files"

# lammps data parser
data_parser="$script_dir/lammps_data_parser/lammps_data_parser"

# templates
in_template="$in_files_dir/fall.in"
input_template="$input_files_dir/fall700.input.data"

### FILE NAMES ###

# .in file
fall_in="fall.in"

# input file
fall_input="fall.input.data"

### OTHER VARS

# zero level
zero_lvl="0.5"

# computes speeds
speeds=""
#speeds="${speeds} 115.7"  # 500  eV
#speeds="${speeds} 163.6"  # 1000 eV
speeds="${speeds} 231.4"  # 2000 eV
#speeds="${speeds} 283.4"  # 3000 eV
#speeds="${speeds} 365.88" # 5000 eV

x_vels=""
#x_vels="${x_vels} 79.1"  # 20
#x_vels="${x_vels} 148.7" # 40
x_vels="${x_vels} 177.3" # 50
#x_vels="${x_vels} 200.4" # 60
x_vels="${x_vels} 217.4" # 70
#x_vels="${x_vels} 227.9" # 80
x_vels_at() { echo ${x_vels} | cut -d" " -f${1}; }

z_vels=""
#z_vels="${z_vels} 217.4" # 20
#z_vels="${z_vels} 177.3" # 40
z_vels="${z_vels} 148.7" # 50
#z_vels="${z_vels} 115.7" # 60
z_vels="${z_vels} 79.1"  # 70
#z_vels="${z_vels} 40.2"  # 80
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
  # rm *.data files
  rm -f "$script_dir/"*.data
  # rm *.in files
  rm -f "$script_dir/"*.in
  # rm *.dump files
  rm -f "$script_dir/"*.dump
  # rm *.lammps
  rm -f "$script_dir/"*.lammps

  echo "removed temp compute files"; echo; echo "$stars"
}

# auto computation for straight fall with different speeds and positions
straight_fall() {
  # variants loop
  for speed_i in ${speeds}
  do
    echo "compute: $compute_name"; echo; echo "$stars"

    cp $in_template $script_dir/$fall_in
    cp $input_template $script_dir/$fall_input

    run_lammps_script "$1" "$script_dir/$fall_in" "$compute_dir" "$speed_i"
  done
}

angle_fall() {
  # variants loop
  for angle_i in $(seq 1 2)
  do
    for move_i in $(seq 1 5)
    do
      compute_name="moved_${move_i}_angle_$(angles_at $angle_i)"
      echo "compute: $compute_name"; echo; echo "$stars"

      compute_dir="$results_dir/$compute_name"
      new_input_data="$compute_dir/$fall_input"
      new_in_data="$compute_dir/$fall_in"

      create_compute_results_dir "$compute_dir"

      randomize_carbon_xy_position "$new_input_data" "$(angles_at $angle_i)"
      change_template_in_file "-$(z_vels_at $angle_i)" "-$(x_vels_at $angle_i)"

      run_lammps_script "$1" "$script_dir/$fall_in" "$compute_dir"
    done
  done
}

#above_surface() {
#  vals=$results_dir/above_surface.vals
#  rm -rf $vals
#  touch $vals
#
#  #thresholds=("0" "0.2" "0.4" "0.6" "0.8" "1")
#  #thresholdss=("0" "0,2" "0,4" "0,6" "0,8" "1")
#  printf "****************" >> $vals
#  for threshold_i in {0..5}
#  do
#    printf "__%1.1f" ${thresholdss[threshold_i]} >> $vals
#  done
#  echo "" >> $vals
#
#  for speed_i in "${speeds[@]}"
#  do
#    for move_i in {1..5}
#    do
#      printf "m: %2d,a: %3.2f|" $move_i $speed_i >> $vals
#
#      compute_name="moved_${move_i}_speed_${speed_i}"
#      echo "compute: $compute_name"; echo; echo "$stars"
#
#      compute_dir="$results_dir/$compute_name"
#      for threshold_i in {0..5}
#      do
#        $data_parser "u" $compute_dir/$dump_last_10 $results_dir/tmp "${thresholds[threshold_i]}"
#        data=$(cat $results_dir/tmp)
#        printf "%5d" $data >> $vals
#      done
#      echo "" >> $vals
#    done
#  done
#}

above_surface() {
  tmp=$results_dir/above_surface.vals
  rm -rf $tmp
  touch $tmp

  numeric_backup=$LC_NUMERIC
  export LC_NUMERIC=en_US.utf8

  thresholds=""
  thresholds="${thresholds} 0.0"
  thresholds="${thresholds} 0.2"
  thresholds="${thresholds} 0.4"
  thresholds="${thresholds} 0.6"
  thresholds="${thresholds} 0.8"
  thresholds="${thresholds} 1.0"
  thresholds_at() { echo ${thresholds} | cut -d" " -f${1}; }

  printf "************" >> $tmp
  for threshold_i in $(seq 1 6)
  do
    printf "__%1.1f" $(thresholds_at $threshold_i) >> $tmp
  done
  echo "" >> $tmp

  for move_i in $(seq 1 5)
  do
    for angle_i in $(seq 1 2)
    do
      printf "m: %2d,a: %2d|" $move_i $(angles_at $angle_i) >> $tmp

      compute_name="moved_${move_i}_angle_$(angles_at $angle_i)"
      echo "compute: $compute_name"; echo; echo "$STARS"

      compute_dir="$results_dir/$compute_name"
      for threshold_i in $(seq 1 6)
      do
        debug="$data_parser 'u' $compute_dir/$dump_last_10 $results_dir/tmp $(thresholds_at $threshold_i)"
        $data_parser 'u' $compute_dir/$dump_last_10 $results_dir/tmp $(thresholds_at $threshold_i)
        echo $debug
        vals=$(cat $results_dir/tmp)
        printf "%5d" $vals >> $tmp
      done
      echo "" >> $tmp
    done
  done

  LC_NUMERIC=$numeric_backup
}

echo "### LAMMPS AUTOCOMPUTE SCRIPT ###"; echo; echo "$stars"
mkdir -p "$results_dir"

"$1" "$2"
