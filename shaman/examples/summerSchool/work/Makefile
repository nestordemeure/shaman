
all: cvPlot.pdf

unitTest: unitTest.cxx integrate.hxx
	g++ -Wall -g -O3 -o $@ $<

cvPlot.pdf: unitTest cvPlot.gp
	./cvPlot
