#!/bin/sh
#SBATCH -J 1core
#SBATCH -o 1core1.o%j
#SBATCH --ntasks=1
#SBATCH -p shared,cui
#SBATCH -t 00:30:00
#SBATCH --constraint=E5-2660V0

echo "============================================="
echo "mpi_bcast"
echo "============================================="

srun ./mpi
srun ./mpi
srun ./mpi
srun ./mpi
srun ./mpi
srun ./mpi
srun ./mpi
srun ./mpi
srun ./mpi
srun ./mpi
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
srun ./simple
srun ./simple
srun ./simple
srun ./simple
srun ./simple
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
srun ./ring
srun ./ring
srun ./ring
srun ./ring
srun ./ring
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
srun ./hypercube
srun ./hypercube
srun ./hypercube
srun ./hypercube
srun ./hypercube
srun ./hypercube
srun ./hypercube
srun ./hypercube
srun ./hypercube
srun ./hypercube
