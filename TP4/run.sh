#!/bin/sh
#SBATCH --job-name=laplace
#SBATCH --output=laplace_whole_algo3000x3000x3000x1_V2.o%j
#SBATCH --ntasks=1
#SBATCH --partition=shared,cui
#SBATCH --time=02:00:00
#SBATCH --constraint=E5-2630V4

echo $SLURM_NODELIST

srun ./laplace 3000 3000 3000
