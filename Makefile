#OBJ = main.o topological_sort.o # Part 2
OBJ = main.o
MAIN = main

$(MAIN): $(OBJ) main.cpp
	$(CXX) -g -std=c++11 -Wall -Wextra -pedantic -Weffc++ -o $(MAIN) $(OBJ)

1: $(MAIN)
	./$(MAIN) input.data

2: $(MAIN)
	./$(MAIN) input2_names.data

3: $(MAIN)
	./$(MAIN) input3.data

cycle: $(MAIN)
	./$(MAIN) input-cycle.data

random: $(MAIN)
	./$(MAIN) randomTest.data

main.o: main.cpp graph.h
	$(CXX) -g -std=c++11 -c main.cpp

topological_sort.o: topological_sort.cpp graph.h
	$(CXX) -g -std=c++11 -c topological_sort.cpp

clean: 
	rm -f $(OBJ) $(MAIN)
