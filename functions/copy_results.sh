#!/bin/sh

copy_lammps_results() {
  compute_dir=$1
  # cp *.data   files
  cp -f ./*.data   $compute_dir
  # cp *.in     files
  cp -f ./*.in     $compute_dir
  # cp *.dump   files
  cp -f ./*.dump   $compute_dir
  # cp *.lammps files
  cp -f ./*.lammps $compute_dir
  echo "copied results"; echo; echo "$stars"
  echo "$compute_dir"
  echo "$compute_dir" > tmp_file
}

copy_lammps_results $1
