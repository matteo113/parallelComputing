#!/bin/sh
#SBATCH -J 16cores
#SBATCH -o 16cores.o%j
#SBATCH --ntasks=16
#SBATCH -p shared,cui
#SBATCH -t 00:30:00
#SBATCH --constraint=E5-2660V0

echo "mpi_bcast"

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

echo "simple_bcast"

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

echo "ring_bcast"

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

echo "hypercube_bcast"

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
