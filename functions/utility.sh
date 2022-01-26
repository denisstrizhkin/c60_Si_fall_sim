# move x and y positions of all carbon atoms by a small random delta
randomize_carbon_xy_position() {
  echo "moving carbon"
  "$data_parser" 'r' "$input_template" "$1" "temp"
  "$data_parser" 'a' "$1" "$1" "$2"
  cp "$1" $script_dir/$fall_input
  echo; echo "$stars"
}

# change template .in file by assigning zero_lvl and x, z velocities for simulation
change_template_in_file() {
  echo "changing .in file"
  local dest="$script_dir/$fall_in"

  echo "# CONSTANTS"                           >  $dest
  echo 'variable zero_lvl equal "'$zero_lvl'"' >> $dest
  echo 'variable carbon_vz equal "'"$1"'"'     >> $dest
  echo 'variable carbon_vx equal "'"$2"'"'     >> $dest

  awk "NR >= 5" $in_template >> $dest
  echo; echo "$stars"
}

create_compute_results_dir() {
  rm -rf   $1
  mkdir -p $1
}

copy_lammps_results() {
  compute_dir=$1
  # cp *.data   files
  cp -f $script_dir/*.data   $compute_dir
  # cp *.in     files
  cp -f $script_dir/*.in     $compute_dir
  # cp *.dump   files
  cp -f $script_dir/*.dump   $compute_dir
  # cp *.lammps files
  cp -f $script_dir/*.lammps $compute_dir
  echo "copied results"; echo; echo "$stars"
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
