# SHAMANIZER

## What is Shamanizer ?

The shamanizer.py python3 script can be used to automaticely add Shaman to a file or folder of C++ files.

It changes the types, adds the required headers and fixes the `printf`.

## How to use Shamanizer ?

Just run `$ python shamanizer.py source` and Shamanizer will produce a new file/folder `shaman_source` (don't forget to make sure that you use python3 and not python2).

## Important notes :

As it relies on string substitution, shamanizer might break some function calls (if an external fucntion requires a double).

If a `printf` is detected, a warning is thrown and the function is replaced by the `tfm::printf` function from the [tinyformat](https://github.com/c42f/tinyformat) library (a single header file).

