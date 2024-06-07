#include "graph.h"

using namespace graph;
using namespace std;

int main() {
    Graph<int> g;

    for (int i = 1; i <= 6; ++i) {
        g.add_vertex(i);
    }

    g.add_edge(6, 1, 7);
    g.add_edge(6, 1, 7);
    g.add_edge(6, 2, 8);
    g.add_edge(6, 3, 9);
    g.add_edge(5, 1, 4);
    g.add_edge(5, 2, 3);
    g.add_edge(5, 3, 2);

    g.print();

    g.remove_edge(6, 1, 7);
    

    g.print();
    
     
    int best_location = g.find_warehouse();

    cout << best_location << endl;

    return 0;
}