#!/bin/sh
#SBATCH -J julia_mpi_simple
#SBATCH -p shared,cui
#SBATCH -t 0:10:00

echo $SLURM_NODELIST

srun ./julia_thread_static -2 -2 2 2 0.285 0.013 500 2500 3000 mpi_static_$SLURM_NTASKS.pgm
