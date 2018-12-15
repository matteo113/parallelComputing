#!/bin/sh
#SBATCH --job-name=laplace
#SBATCH --output=laplace_whole_algo2000x2000x3000x120.o%j
#SBATCH --ntasks=120
#SBATCH --partition=shared,cui
#SBATCH --time=00:30:00
#SBATCH --constraint=E5-2630V4

echo $SLURM_NODELIST

srun ./laplace 2000 2000 3000
