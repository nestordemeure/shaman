
all: cvPlot.pdf dd

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

dd: unitTest libm.ex ddRun ddCmp
	rm -rf dd.*
	verrou_dd ${PWD}/ddRun ${PWD}/ddCmp
	./util/dd-synthese

dd2: unitTest libm.ex ddRun ddCmp
	rm -rf dd.*
	VERROU_DD_NRUNS=1 verrou_dd ${PWD}/ddRun2 ${PWD}/ddCmp2
	./util/dd-synthese
