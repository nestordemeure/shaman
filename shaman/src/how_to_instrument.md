# How to instrument a code with Shaman ?

Put the `shaman` folder in your project and the `shaman.h` in your file.

Use the `Sfloat`, `Sdouble` and `Slong_double` types in your program.

See also `Shamanizer`, a tool that automatically instrument your code with Shaman.

## Flags :

To forbid any implicit casting (as usually done by C++) you can add the `EXPLICIT_CASTING` flag to the compilation (note that the resulting error messages might not be very clear).

Don't forget to enable Fused-Multiply-Add at compilation (`-mfma`), otherwise some operations (*, /, sqrt) will be much slower.

The `NO_SHAMAN` flag let you deactivate Shaman to run your computation as if you used the base floating point arithmetic.

## Functions :

### std

We provide drop-in replacements for the majority of mathematical functions from the standard library.

### Printf

If you use `printf` you can either switch to the C++ streaming operator, explicitly cast shaman types into strings or use the `tinyformat` implementation of printf (provided in the `lib` folder).

### OpenMP

Just add the `-fopenmp` flag as usual.

You will need openMP 4.0 or more to use reduce operations.

### MPI

To use :
- include Shaman_mpi.h
- replace `MPI_Init` with `MPI_Shaman_Init` and `MPI_Finalize` with `MPI_Shaman_Finalize`
- use the shaman MPI types (`MPI_FLOAT` -> `MPI_SFLOAT`)
- use the shaman MPI operations instead of the corresponding `MPI_Op` (`MPI_SUM` -> `MPI_SSUM`)

You can also use `Shamanizer.py` to do your conversion automatically.

### Malloc

Using `malloc` and `free` could lead to segfaults and memory leaks since shaman's type is not a POCO type.

since C++ provides the `new []` and `delete []` operations, these functions should be rare.
However, if the original code relies on `malloc` and to stay as close as possible to the original implementation, we provide safe equivalents in the `lib/Shaman_malloc.h` file.
