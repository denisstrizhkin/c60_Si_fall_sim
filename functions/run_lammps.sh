# get number of OpenMP threads
get_omp_num_threads() {
  if [ -z "$1" ]; then
    omp_num_threads=8
  else
    omp_num_threads="$1"
  fi

  printf "%d" $omp_num_threads
}

# running lammps script
run_lammps_script() {
  local omp_num_threads=$1
  local in_file=$2
  local output_dir=$3

  set_omp_num_threads $(get_omp_num_threads $omp_num_threads)

  echo "running lammps script"; echo " ---"
  #mpirun --use-hwthread-cpus -np 8 lmp_mpi -in "$script_dir/$in_file" 
  #mpirun -np 4 lmp_omp -sf omp -pk omp 4 -in "$script_dir/$in_file"
  lmp_omp -sf omp -in "$in_file"
  echo; echo "$stars"

  copy_lammps_results $output_dir

  clean
}

# set number of OpenMP threads
set_omp_num_threads() {
  export OMP_NUM_THREADS="$1"
  echo "set number of OpenMP threads to $OMP_NUM_THREADS";
  echo; echo "$stars" 
}
