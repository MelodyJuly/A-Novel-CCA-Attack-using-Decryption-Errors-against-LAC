#!/bin/sh
#SBATCH -t 96:00:00
#SBATCH -N 10
#SBATCH -n 200
#SBATCH --exclusive
#SBATCH -J jobFarm
for ((i=0; i<200; i++))
do
	srun -n 1 -N 1 python lacv2.py ${i} &
	sleep 5
done
wait



