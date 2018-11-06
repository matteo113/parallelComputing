#!/bin/sh
#SBATCH --job-name=laplace
#SBATCH --output=test2000x2000x2000.o%j
#SBATCH --ntasks=20
#SBATCH --partition=shared,cui
#SBATCH --time=00:15:00

echo $SLURM_NODELIST

srun ./tp2 2000 2000 2000
