#!/bin/sh
#SBATCH -J 64core
#SBATCH -o 3--64core1.o%j
#SBATCH --ntasks=64
#SBATCH -p shared, cui
#SBATCH --mail-user=Matteo.Besancon@etu.unige.ch
#SBATCH --mail-type=ALL
#SBATCH -t 00:15:00

module load foss/2018a
module load CUDA

echo "============================================="
echo "mpi_bcast"
echo "============================================="

srun ./mpi
srun ./mpi
srun ./mpi
srun ./mpi
srun ./mpi


echo "============================================="
echo "simple_bcast"
echo "============================================="

srun ./simple
srun ./simple
srun ./simple
srun ./simple
srun ./simple



echo "============================================="
echo "ring_bcast"
echo "============================================="

srun ./ring
srun ./ring
srun ./ring
srun ./ring
srun ./ring


echo "============================================="
echo "hypercube_bcast"
echo "============================================="

srun ./hypercube
srun ./hypercube
srun ./hypercube
srun ./hypercube
srun ./hypercube
