#!/bin/sh
#SBATCH -J 256cores
#SBATCH -o 256cores.o%j
#SBATCH --ntasks=256
#SBATCH -p shared,cui
#SBATCH -t 00:30:00
#SBATCH --constraint=E5-2630V4

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
