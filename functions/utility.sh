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
