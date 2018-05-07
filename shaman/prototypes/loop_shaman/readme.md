# Loop Shaman

A prototype to try and deal with pessimism when running on loop :
- we run the loop in classical floating point arithmetic
- if the loop would stop at an iteration when taking into account its numerical error, we record the values
- after the loop we correct the error on our output by looking at the output after correction

This prototype is limited to loop that should end earlier, we need to run the loop longuer and with increased precision to deal with loops that should end later.

The current prototype does not solve the problem. Why ?
Because the IEEE value and the mathematical value will live different computation.
Hence we cannot hope to encapsulate both in the shaman type :
We truly need to build two loops, one with IEEE/shaman
and a second one that runs with more precision to be able to compare both and determine the actual error