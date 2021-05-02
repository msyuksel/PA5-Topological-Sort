#include <iostream>
#include "graph.h"

int main(int argc, const char * argv[])
{
    if ( argc != 2 )
    {
        std::cout << "usage: " << argv[0] << " input filename\n";
        return 1;
    }
    std::string input_name = argv[1];
    std::ifstream input("input/" + input_name);
    if (!input) 
    {
        std::cout << "Wrong or nonexisting input file\n";
        return 1;
    }
    
    Graph<std::string> graph;
    std::cout << "Building graph...\n";
    graph.buildGraph(input);
    
    std::cout << "\nDisplaying graph...\n";
    if(graph.size() < 20)
    {
        graph.displayGraph(std::cout);
    }

    std::cout << "\nTopologically sorting using in-degree method...\n";

    graph.compute_indegree();
    bool can_top_sort = graph.topological_sort(); 
    if(can_top_sort)
    {
        if(graph.size() < 1000)
        {
            graph.print_top_sort(std::cout); 
        }
        else
        {
            std::cout << "Graph is too big. I refuse to print\n";
        }
    }
    else
    {
        std::cout << "Cycle detected, cannot topologically sort...\n";
    }
}
