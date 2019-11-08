# SHAMANIZER

**WARNING: This is an obsolete python version. We now have a much more efficient tool based on the [Clang](https://en.wikipedia.org/wiki/Clang) compiler.**

## What is Shamanizer ?

The shamanizer.py python3 script can be used to automatically instruments a file or folder of C++ files with Shaman.

It changes the types, adds the required headers and fixes the `printf`.

## How to use Shamanizer ?

Just run `python shamanizer.py source` and Shamanizer will produce a new file/folder `shaman_source` (don't forget to make sure that you use python3 and not python2).

## Important notes :

As it relies on string substitution, shamanizer might break some function calls (if an external function requires a double). Those will result in compilation crash.

If a `printf` is detected, a warning is thrown and the function is replaced by the `tfm::printf` function from the [tinyformat](https://github.com/c42f/tinyformat) library (a single header file).
A similar warning is thrown if MPI is detected in the code.

## Potential improvements

Shamanizer converts the types but it will not deal with numbers in intermediate computations (i.e. in `2. + x` 2. will not be switched by `Sdouble(2.)`).
since those are actually the source of most compilation crash on a program instrumented with Shamanizer, it might be interesting to convert them.

