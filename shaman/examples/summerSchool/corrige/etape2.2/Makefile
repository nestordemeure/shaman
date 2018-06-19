
all: gcov cvPlot.pdf

unitTest: unitTest.cxx integrate.hxx
	g++ -Wall -g -O3 -fprofile-arcs -ftest-coverage -o $@ $<

cvPlot.pdf: unitTest cvPlot.gp libm.ex
	./cvPlot

check: unitTest
	./unitTest                                                > out.ref
	valgrind --tool=none ./unitTest                           > out.vg-none
	valgrind ./unitTest                                       > out.vg-mem
	valgrind --tool=verrou --rounding-mode=nearest ./unitTest > out.vg-verrou
	diff out.ref out.vg-none
	diff out.ref out.vg-mem
	diff out.ref out.vg-verrou

libm.ex:
	./util/verrou-exclude unitTest libm.so > $@

gcov: unitTest libm.ex
	./runGcov
