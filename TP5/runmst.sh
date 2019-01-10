#!/bin/sh
#SBATCH -J julia_mpi_simple
#SBATCH -p shared,cui
#SBATCH --constraint=E5-2630V4

echo $SLURM_NODELIST

srun ./julia_thread_static -2 -2 2 2 0.285 0.013 5000 15000 20000 mpi_static_$SLURM_NTASKS.pgm
