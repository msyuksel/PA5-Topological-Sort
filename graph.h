#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <list>
#include <queue>
#include <sstream>
#include <fstream>

/**
 * @brief A custom exception class inheriting from std::exception for when
 * a cycle is found when attempting to topologically sort. Currently replaced
 * by top_sort() returning FALSE instead of throwing foundCycle
 * 
 */
class foundCycle : public std::exception 
{
    virtual const char* what() const throw()
    {
        return "Found a cycle in this graph!";
    }
};

/**
 * @brief templated class Vertex. Used to compose Graph
 * 
 * @tparam T Vertex type
 */
template <class T>
struct Vertex {
    T label;
    std::vector<T> adj_list;
    int indegree = 0;
    int top_num = 0;
    
    explicit Vertex(T label)
    : label(label) {}
};

/**
 * @brief output the key (label) of a vertex followed by its adjacency vertices
 * 
 * @tparam T type of vertex (int, string, custom, ...)
 * @param os output stream
 * @param v vertex to output
 * @return ostream& the output stream with a vertex and adjacent vertices
 */
template <class T>
std::ostream& operator<<(std::ostream &os, const Vertex<T>& v)
{
    os << v.label << " :";
    for (auto const &i : v.adj_list) {
        os << " " << i;
    }
    return os;
};

/**
 * @brief A compartor class to be passed to std::priority_queue
 * 
 * @tparam T type of vertex (int, string, custom, ...)
 */
template<class T>
class VertexCompare
{
public:
    bool operator()(Vertex<T> v1, Vertex<T> v2)
    {
        return v1.top_num > v2.top_num;
    }
};

/**
 * @brief Utilizes the "has a" relationship in Object Orientated Programming. 
 * A Graph is composed of vertices of Vertext<T> that are stored 
 * inside an std::unordered_map (hash table). Allows for accessing of vertices
 * in O(1).
 * 
 * @tparam T type of vertices (int, string, custom, ...)
 */
template <class T>
class Graph {
    
    private:
    std::unordered_map<T, Vertex<T>> node_set;
    size_t _size = 0;
    std::priority_queue<Vertex<T>, std::vector<Vertex<T>>, VertexCompare<T>> sorted_queue;
    
    public:
    Graph() {}; 
    ~Graph() {};

    /**
     * @brief Reads a set of Vertices and their respespective adjacent vertices.
     * 
     * @param input an istream of a graph adjacency list read from text
     */
    void buildGraph(std::istream &input)
    {
        std::string string_line;
        T data;
        while(getline(input, string_line))
        {
            if (string_line.empty())
            {
                break;
            }
            std::stringstream buf(string_line);
            if (buf >> data)
            {
                Vertex<T> vert(data);
                while(buf >> data)
                {
                    vert.adj_list.push_back(data);
                }
                ++_size;
                auto pair = node_set.insert(make_pair(vert.label, vert));
            }
            
        }
    }

    /**
     * @brief Utilizes operator<< defined for vertex to output 
     * 
     * @param output_stream
     */
    void displayGraph(std::ostream& os)
    {
        for (auto const &pair : node_set)
        {
            os << pair.second << "\n";
        }
    }

    /**
     * @brief Call std::unordered_map::at with given key to access vertex 
     * at constant time O(1)
     * 
     * @param key 
     * @return Vertex<T> associated with key
     */
    Vertex<T> at(T label)
    { 
        return node_set.at(label);
    }

    /**
     * @brief size of the graph
     * 
     * @return _size
     */
    int size() const
    {
        return _size;
    }

    /**
     * @brief Topological sorts a graph object and stores in priority queue.
     * Employs the in-degree method
     * 
     * @return true if no cycle detected
     * @return false if cycle detected
     */
    bool topological_sort()
    {
        std::queue<Vertex<T>> q_of_indegree_0;
        int popped_counter = 0;

        for (auto const &pair : node_set)
        {
            auto const &vert = pair.second;
            if (vert.indegree == 0)
            {
                q_of_indegree_0.push(vert);
            }
        }
        while ( !q_of_indegree_0.empty() )
        {
            T v_label = q_of_indegree_0.front().label;
            q_of_indegree_0.pop();

            node_set.at(v_label).top_num = ++popped_counter; 
            sorted_queue.push(node_set.at(v_label));
            
            for (auto &adj_vertex_label : node_set.at(v_label).adj_list)
            { 
                auto &ref_to_vertex = node_set.at(adj_vertex_label);
                ref_to_vertex.indegree -= 1;
                
                if (ref_to_vertex.indegree == 0)
                {
                    q_of_indegree_0.push(ref_to_vertex);
                }
            }
        }
        
        /* All vertices should eventually have an indegree of 0, therefore
         * counter should = size of graph. If not equal, no vertices with
         * indegree 0 were found, meaning it found a cycle 
         */
        if (popped_counter != _size) 
        {
            return false;
        } 
        return true;
    }

    /**
     * @brief Iterates through unordered_map of vertices and each of their
     * vertices to update all of their indegree values in O(E+V)
     * 
     */
    void compute_indegree()
    {
        for (auto &pair : node_set)
        {
            pair.second.indegree = 0;
        }

        for (auto &pair : node_set) 
        {
            for (auto &vert : pair.second.adj_list) 
            {
                node_set.at(vert).indegree++;
            }
        }
    }
    

    /**
     * @brief output the contents of the sorted_queue into an output stream. The
     * queue should already be filled with vertices ordered according to their
     * top_num values
     * 
     * @param os Output stream to push to
     * @param addNewline Dictate whether to push a newline to stream.
     */
    void print_top_sort(std::ostream& os, bool addNewline = true)
    { 
        for (int i = 0; i < _size; i++ ) 
        {
            os << sorted_queue.top().label << " ";
            sorted_queue.pop();
        }
        if(addNewline) 
        {
            os << '\n';
        }
    }
};

#endif
