perf record -g time -f "\nTime : %es (CPU usage : %P, Max Memory usage : %MkB)\n" $1
perf report -g 'graph,0.5,callee'