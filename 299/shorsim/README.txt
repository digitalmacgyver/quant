1. The Shor's algorithm simulator can be built with:

make all

Which will produce the shor-simulator binary, which can be run with:

./shor-simulator

2. The simulator is defined in these files:

complex.h, complex.C - Simple complex number implementation.
qureg.h, qureg.C - Simple quantum register implementation.
util.h, util.C - Utility functions used by the simulator.
shor.C - Main logic of the simulator

3. There is also included a simple qubit.C program which defines a
single qubit, it is not used bit the simulation.
