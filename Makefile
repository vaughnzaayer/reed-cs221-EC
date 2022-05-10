CXX=g++
CXXFLAGS= -std=c++17 -O3 -pthread
# -pedantic -Wextra -Wall -g
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)

all:  tsp

tsp: tsp.o chromosome.o deme.o cities.o climb_chromosome.o tournament_deme.o
	$(CXX) $(LDFLAGS) -o $@ $^

unit_test: chromosome.o cities.o ch_unit_test.o 
	$(CXX) $(LDFAGS) -o $@ $^

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o tsp
