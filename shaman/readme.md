# SHAMAN : A library that bridges the gap between humans and the invisible world of numerical errors

## What is Shaman ?

Shaman is a header-only library that lets you do a computation while running a model of the propagation of numerical error in your code.
In short you instrument your code, you run it and it will only display the significant digits of your outputs.

For common operations (+, -, *, /, sqrt), it uses Error Free Transforms to deduce the propagation of error.
On other operations (exp ,cos, etc) we fall back to higher precision to compute the error propagated.

The underlying number type is similar to some double-double arithmetics, the main differences being that their aim is to do high precision computations while we are interested in error propagation.
Hence we garantee the separation between the computed number and the numerical error and we can use higher precision arithmetic on operations were the error cannot be computed with the base precision (implementing all usual operations contrary to usual double-double types).

## What will you find in this repository ?

You will find :
 - the Shaman library (and a well documented readme)
 - examples of usage on simple computations
 - tests
 - prototypes for future developments
 - tools to help with the collect of results and instrument a library with Shaman
