#!/bin/sh

data_parser=lammps_data_parser/lammps_data_parser
dump_last_10="last_10.dump"
dump_vor="vor_time.dump"

# parse lammps dump files
parse_dump_files() {
  compute_dir=$1
  #parse carbon z distribution dump
  echo "last 10 steps carbon distribution average calculation"
  $data_parser "c" $compute_dir/$dump_last_10 $compute_dir/C_z_dist.vals "temp"
  echo; echo "$stars"

  # parse voro dump
  echo "parsing voronoi time relation dump"
  $data_parser "v" $compute_dir/$dump_vor $compute_dir/Voro_time.vals "temp"
  echo; echo "$stars"
}

parse_dump_files $1
