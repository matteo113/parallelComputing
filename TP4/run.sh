#!/bin/sh
#SBATCH --job-name=laplace
#SBATCH --output=laplace_whole_algo4000x4000x3000x120.o%j
#SBATCH --ntasks=120
#SBATCH --partition=shared,cui
#SBATCH --time=00:05:00

echo $SLURM_NODELIST

srun ./laplace 4000 4000 3000
