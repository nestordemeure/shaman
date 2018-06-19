
all: cvPlot.pdf check

unitTest: unitTest.cxx integrate.hxx
	g++ -Wall -g -O3 -o $@ $<

cvPlot.pdf: unitTest cvPlot.gp
	./cvPlot

check: unitTest
	./unitTest                                                > out.ref
	valgrind --tool=none ./unitTest                           > out.vg-none
	valgrind ./unitTest                                       > out.vg-mem
	valgrind --tool=verrou --rounding-mode=nearest ./unitTest > out.vg-verrou
	diff out.ref out.vg-none
	diff out.ref out.vg-mem
	diff out.ref out.vg-verrou
