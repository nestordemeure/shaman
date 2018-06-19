SHAMAN_FLAGS=-std=c++11 -mfma
SHAMAN_DEBUG_FLAGS=-DUNSTABLE_BRANCH_DEBUGGER -DCANCELATION_DEBUGGER -DUNSTABLE_OP_DEBUGGER

all: cvPlot.pdf

unitTest.exe: unitTest.cxx integrate.hxx
	g++ $(SHAMAN_FLAGS) -Wall -g -O3 -o unitTest.exe $<

unitTest_debug.exe: unitTest.cxx integrate.hxx
	g++ $(SHAMAN_FLAGS) $(SHAMAN_DEBUG_FLAGS) -Wall -g -O3 -o unitTest_debug.exe $<

cvPlot.pdf: unitTest.exe unitTest_debug.exe cvPlot.gp
	./cvPlot
