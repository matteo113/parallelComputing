#!/bin/sh
#SBATCH -J julia_mpi_simple
#SBATCH -p shared,cui
#SBATCH -t 0:10:00

echo $SLURM_NODELIST

srun ./julia_thread_simple -2 -2 2 2 0.285 0.013 5000 15000 20000 mpi_simple_$SLURM_NTASKS.pgm
