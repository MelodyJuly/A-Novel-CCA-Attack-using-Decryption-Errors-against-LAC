#!/bin/sh
#SBATCH -t 168:00:00
#SBATCH -N 1
#SBATCH --tasks-per-node=1
#SBATCH --mail-user= email_example
#SBATCH --mail-type=ALL

./lac

