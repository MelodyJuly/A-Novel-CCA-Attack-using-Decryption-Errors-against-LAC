The code for collecting errors of LAC v2.

"ctype" is used to in the code.

lacv2.py: the python script calling the C functions
lacv2.c: the main function for collecting the errors
Lacv2.h, random.h: hearder files
Lacv2.o, Lacv2.so: the object files for lacv2.py
lacv2.sh: sbatch example on how to run several instances in a single job submission at a server

You can run the code by commands:
make // run the Makefile to generate .o and .so files
sbatch lacv2.sh // run the lacv2.py to load the .so file to run several instances
