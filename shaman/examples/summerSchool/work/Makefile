SHAMAN_FLAGS=-std=c++11 -mfma
SHAMAN_DEBUG_FLAGS=-DUNSTABLE_BRANCH_DEBUGGER -DCANCELATION_DEBUGGER -DUNSTABLE_OP_DEBUGGER

all: cvPlot.pdf

unitTest: unitTest.cxx integrate.hxx
	g++ $(SHAMAN_FLAGS) -Wall -g -O3 -o $@ $<

cvPlot.pdf: unitTest cvPlot.gp
	./cvPlot
