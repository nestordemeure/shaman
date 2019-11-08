# How to instrument a code with Shaman ?

Compile Shaman, link it to your project compilation and include `shaman.h` in your source code.

Use the `Sfloat`, `Sdouble` and `Slong_double` types in your program.

See also `Shamanizer`, a tool that automatically instrument your code with Shaman.

## Flags :

Compile shaman with the `SHAMAN_TAGGED_ERROR` flag to enable tagged error.

Use the `SHAMAN_UNSTABLE_BRANCH` flag to enable the count and detection of unstable branches.
The `Shaman::displayUnstableBranches` function can then be used to print the number of unstable tests performed by the application (and additional localisation informations if tagged error is activated).

The `NO_SHAMAN` flag let you deactivate Shaman to run your computation as if you used the base floating point arithmetic.

Don't forget to enable Fused-Multiply-Add at compilation (`-mfma`), otherwise some operations (`*`, `/`, `sqrt`) will be much slower.

## Functions :

### std

We provide drop-in replacements for all mathematical functions from the standard library in the `Sstd` namespace.

### Printf

If you use `printf` you can either switch to the C++ streaming operator, explicitly cast shaman types into strings or use something like the [tinyformat](https://github.com/c42f/tinyformat) implementation of printf as a drop-in replacement.

### OpenMP

Just add the `-fopenmp` flag as usual.

You will need OpenMP 4.0 or more to use reduce operations on Shaman's type.

### MPI

To use :
- include Shaman_mpi.h
- replace `MPI_Init` with `MPI_Shaman_Init` and `MPI_Finalize` with `MPI_Shaman_Finalize`
- use the shaman MPI types (`MPI_FLOAT` -> `MPI_SFLOAT`)
- use the shaman MPI operations instead of the corresponding `MPI_Op` (`MPI_SUM` -> `MPI_SSUM`)

Note that you currently cannot use both MPI and Shaman's tagged error (which is used to trace the sources of numerical error).

