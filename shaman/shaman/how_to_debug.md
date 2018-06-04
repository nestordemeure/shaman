# Numerical Debugger

## How to use the numerical debugger ?

You can add flags to print the number of unstabilities and cancelations when the program terminates : 
- `NUMERICAL_DEBUGGER` enable all subsecent flags
- `UNSTABLE_BRANCH_DEBUGGER` detects unstable boolean operations
- `CANCELATION_DEBUGGER` detects cancelations
- `NUMERICAL_ZERO_DEBUGGER` counts the number of numerical zeros appearing in the code
- `UNSTABLE_OP_DEBUGGER` detects unstable operations
- `DOUBT_LEVEL_FIELD_ENABLED` to count and print the number of unstable operations leading to a number

Put a breakpoint on the `unstability` function in the `Debugger.h` file to beaks on single operations.

As an example, you can compile with the `UNSTABLE_BRANCH_DEBUGGER` flag and put a breakpoint on the `unstability` function with gdb to break on all unstable branches.

## What are numerical unstabilities and cancelations ?

An unstability can be :
- unstable division :
  the divisor is non-significant
- unstable power function :
  one operand of the power function is non-significant
- unstable multiplication :
  both operands are non-significant
- unstable branching :
  the difference between the two operands is non-significant (a computational zero).
  Note that unstable branchings can lead to a pessimistic estimation of the final errors.
- unstable mathematical function :
  in the log, sqrt, exp or log10 function, the argument is non-significant.
- unstable instrinsic function :
  - when using integer cast functions, the integral part of the argument can not be exactly determined due to the round-off errors propagation
  - in the fabs function: the argument is non-significant
  - the floor, ceil or trunc function returns different values for each component of the stochastic argument.
- loss of accuracy due to cancelation :
  the difference between the number of exact significant digits of the result of an addition or a subtraction
  and the minimimum of the number of exact significant digits of the two operands is greater than the cancel level argument (4 by default).
  when one loses more than cancel level significant digits in one addition or subtraction

