#!/bin/sh
#SBATCH -J heat
#SBATCH -o heat.o%j
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --cpus-per-task=6
#SBATCH -p shared,cui
#SBATCH -t 0:02:00

echo $SLURM_NODELIST

srun ./laplace 100 100 5000 $SLURM_CPUS_PER_TASK
