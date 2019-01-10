#!/bin/sh
#SBATCH -J julia_thread_dynamic
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH -p shared,cui

echo $SLURM_NODELIST

srun ./julia_thread_dynamic -2 -2 2 2 0.285 0.013 5000 15000 20000 thread_dynamic_$SLURM_CPUS_PER_TASK.pgm $SLURM_CPUS_PER_TASK $1
