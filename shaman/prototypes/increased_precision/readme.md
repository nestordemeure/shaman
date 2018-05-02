
# Shaman++

## Project

This folder contains a quick F# prototype for a version of Shaman that increase the number of error terms when the penultimate one becomes non significativ.

The aim is to prototype quickly in order to find a design that works well and solves examples where Shaman fails.

## Genetic algorithm

It might also be a good place to test a genetic algorithm that produces expressions to try and maximize the :
- numerical error
- difference between the numerical error and the error detected by shaman (to produce simple examples where shaman fails)
- difference between the numerical error and the error detected by any framework (to produce simple examples where the state of the art fails)

Using linear genetic algorithms, we can represnts arithmetic expressions (including loops) in a format that is easy to mutate / crossover.
An expression is an array of declaration (we force the two first ones to be numbers declaration, the last one is considered the return statement) :
x0 <- Num 1
x1 <- Frac 12 13
x2 <- Div x0 x1
x3 <- Loop 100 x1 x2; 100 // loop returns its number of iterations

We should be able to compute the exact result using a (long) fraction type

## List of Shaman's identified shortcomings

It would also be nice to be able to characterise precisely why shaman fails when it does :
- convergent sequence : the slightest error makes 100 the correc answer
- kahan identity (sqrt squared) : the transmission of error in sqrt is approximated using a derivative formula

Unknown :
- Eigenvalue decomposition ?????
- n-body problem ?????
- felix.1.0
