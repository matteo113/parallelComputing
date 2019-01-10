#!/bin/sh
#SBATCH -J julia_thread_simple
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH -p shared,cui
#SBATCH -t 0:10:00

echo $SLURM_NODELIST

srun ./julia_thread_simple -2 -2 2 2 0.285 0.013 500 2500 3000 thread_simple_$SLURM_CPUS_PER_TASK.pgm $SLURM_CPUS_PER_TASK
