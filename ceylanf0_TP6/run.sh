#!/bin/sh
#SBATCH -J julia_cuda
#SBATCH -o julia_cuda.o%j
#SBATCH --ntasks=1
#SBATCH -p shared-gpu
#SBATCH -t 00:10:00
#SBATCH --gres=gpu:pascal:1

echo $SLURM_NODELIST

srun ./julia_cuda -2 -2 2 2 0.285 0.013 5000 15000 20000 /scratch/julia_cuda.pgm
