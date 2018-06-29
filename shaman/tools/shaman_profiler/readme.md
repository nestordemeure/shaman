# SHAMAN PROFILER

## What is the Shaman profiler ?

The shaman profiler give you a numerical profile of your application with the number of numerical unstabilities detected in each function and the exact line/operations where those unstabilities appeared.

## How to use the Shaman profiler ?

To use the Shaman Profiler :
- use the Shaman types in your application
- add the `NUMERICAL_DEBUGGER` and `-g` flags at compilation (you might also need to reduce optimisations)
- run your program with `gdb -quiet --command shaman_profiler.py --args ./your_program args` (you can also use the `shaman_prof.sh` script as a shortcut)

It will display a message every 100000 numerical unstability detected and output its results to the `executable_name_shaman_profile.txt` file once the program finishes running.

As it relies on breakpoints to collect informations you can expect a slowdown proportional to the number of numerical unstabilities.

## A note of caution on older versions of gdb

Printing more than 50 lines on the screen will cause older versions of gdb to quit before the program finishes its execution.
It means that if you debug with an old version of gdb you might not be able to profile for more than 5000000 numerical unstabilities (you will still get a profile for the unstabilities detected so far).

This bug appears to have been fixed somewhere before gdb-7.11.1.

## Potential improvements

The debugging could stop early if the program is unable to set the breakpoint properly.

The debugging process could be redone by calling the `libunwind` library directly from Shaman which would clean-up the code and drastically improve performances.

The concept of tagged error (which will hopefully be introduced in later versions of shaman) provides a much more powerful solution to the problem that this tool tries to solve : locating the sources of numerical errors in an application.