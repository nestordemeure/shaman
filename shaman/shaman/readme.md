# SHAMAN : A library that bridges the gap between humans and the invisible world of numerical errors

## What is Shaman ?

Shaman is a header-only library that lets you do a computation while running a model of the propagation of numerical error in your code.
In short you instrument your code, you run it and it will only display the significant digits of your outputs.

## How to use Shaman ?

### In a sequential code

Put the `shaman` folder in your project and the `shaman.h` in your file.

Use the `Sfloat`, `Sdouble` and `Slong_double` types in your program.

#### Notes :

If you use `printf` you can either switch to the C++ streaming operator, explicitely cast shaman types into strings or use the `tinyformat` implementation of printf.

To forbid any implicit casting (as usually done by C++) you can add the `EXPLICIT_CASTING` flag to the compilation (note that the resulting error messages might not be very clear).

Don't forget to enable Fused-Multiply-Add at compilation (`-mfma`), otherwise some operations (*, /, sqrt) will be much slower.

The `NO_SHAMAN` flag let you deactivate Shaman to run your computation as if you used the base floating point arithmetic.

See also `Shamanizer`, a tool that automatically instrument your code with Shaman.

### In a parallel code

#### OpenMP

Just add the `-fopenmp` flag as usual.

You will need openMP 4.0 or more to use reduce operations.

#### MPI

To use :
- include Shaman_mpi.h
- replace `MPI_Init` with `MPI_Shaman_Init` and `MPI_Finalize` with `MPI_Shaman_Finalize`
- use the shaman MPI types (`MPI_FLOAT` -> `MPI_SFLOAT`)
- use the shaman MPI operations instead of the corresponding `MPI_Op` (`MPI_SUM` -> `MPI_SSUM`)

You can also use `Shamanizer.py` to do your conversion automatically.

## Numerical Debugger

### How to use the numerical debugger ?

You can add flags to print the number of unstabilities and cancelations when the program terminates : 
- `NUMERICAL_DEBUGGER` enable all subsecent flags
- `UNSTABLE_BRANCH_DEBUGGER` detects unstable boolean operations
- `CANCELATION_DEBUGGER` detects cancelations
- `NUMERICAL_ZERO_DEBUGGER` counts the number of numerical zeros appearing in the code
- `UNSTABLE_OP_DEBUGGER` detects unstable operations
- `DOUBT_LEVEL_FIELD_ENABLED` to count and print the number of unstable operations leading to a number

Put a breakpoint on the `unstability` function in the `Debugger.h` file to beaks on single operations.

As an example, you can compile with the `UNSTABLE_BRANCH_DEBUGGER` flag and put a breakpoint on the `unstability` function with gdb to break on all unstable branches.

### What are numerical unstabilities and cancelations ?

An unstability can be :
- unstable division :
  the divisor is non-significant
- unstable power function :
  one operand of the power function is non-significant
- unstable multiplication :
  both operands are non-significant
- unstable branching :
  the difference between the two operands is non-significant (a computational zero).
  Note that unstable branchings can lead to a pessimistic estimation of the final error.
- unstable mathematical function :
  in the log, sqrt, exp or log10 function, the argument is non-significant.
- unstable instrinsic function :
  - when using integer cast functions, the integral part of the argument can not be exactly determined due to the round-off error propagation
  - in the fabs function: the argument is non-significant
  - the floor, ceil or trunc function returns different values for each component of the stochastic argument.
- loss of accuracy due to cancelation :
  the difference between the number of exact significant digits of the result of an addition or a subtraction
  and the minimimum of the number of exact significant digits of the two operands is greater than the cancel level argument (4 by default).
  when one loses more than cancel level significant digits in one addition or subtraction

## How does Shaman works ?

For common operations (+, -, *, /, sqrt), it uses Error Free Transforms to deduce the propagation of error.
On other operations (exp ,cos, etc) we fall back to higher precision to compute the error propagated.

The underlying number type is similar to some implementations of double-double arithmetic, the main differences being that their aim is to do high precision computations while we are interested in error propagation.
Hence we garantee the separation between the computed number and the numerical error and we can use higher precision arithmetic on operations were the error cannot be computed with the base precision (implementing all usual operations contrary to usual double-double types).
