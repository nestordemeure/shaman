# SHAMAN : A library that bridges the gap between humans and the invisible world of numerical errors

## What is Shaman ?

Shaman is a library that lets you do your computation while running a model of the propagation of error in your code.

It uses EFT (error free transformation) and formulas to deduce the propagation of error on the most common operations (+, -, *, /, sqrt).
On other operations (exp ,cos, etc) we fall back to higher precision to compute the error propagated.

The underlying number type works like the pair arithmetic introduced in Faithfully Rounded Floating-point Computations by MARKO LANGE and SIEGFRIED M. RUMP.
The main difference is that their aim is to do high precision computations while we are interested in error propagation.

## How to use Shaman ?

### In a sequential code

Put the 'shaman' folder in your project.

Use the 'Sfloat', 'Sdouble' and 'Slong_double' types in your program.

If you use 'printf' you can either switch to the C++ streaming operator or explicitely cast shaman types into strings.

To forbid any implicit casting (as usually done by C++) you can add the 'EXPLICIT_CASTING' flag to the compilation (note that the resulting error messages might not be very clear).

Don't forget to enable FMA at compilation ('-mfma'), otherwise some operations (*, /, sqrt) will be much slower.

The 'NO_SHAMAN' flag let you deactivate SHAMAN and use the usual types instead.

See also `Shamanizer`, a tool that automaticely converts your code to Shaman.

### In a parallel code

#### OpenMP

Just add the '-fopenmp' flag as usual.

You will need openMP 4.0 or more to use reduce operations.

#### MPI

To use :
- include Shaman_mpi.h
- replace 'MPI_Init' with 'MPI_Shaman_Init'
- replace 'MPI_Finalize' with 'MPI_Shaman_Finalize'
- use the shaman MPI types ('MPI_FLOAT' -> 'MPI_SFLOAT')
- use the shaman MPI operations ('MPI_SUM' -> 'MPI_SSUM')

You can also use Shamanizer.py to do your conversion automaticely.

## Numerical Debugger

### How to use the numerical debugger ?

Add the 'NUMERICAL_DEBUGGER' flag to your compilation, your program will now print the number of unstabilities and cancelations when it terminates.

Put a breakpoint on the 'unstability' function in the 'NumericalDebugger.h' file to debug single operations.

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
  The chosen branching statement is associated with the equality.
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
