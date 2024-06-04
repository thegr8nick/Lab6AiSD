#include "Graph.h"

int main() {
    Graph<int, double> g;

    for (int i = 1; i <= 6; ++i) {
        g.add_vertex(i);
    }

    g.add_edge(1, 2, 7);
    g.add_edge(1, 3, 9);
    g.add_edge(1, 6, 14);
    g.add_edge(2, 3, 10);
    g.add_edge(2, 4, 15);
    g.add_edge(3, 4, 11);
    g.add_edge(3, 6, 2);
    g.add_edge(4, 5, 6);
    g.add_edge(5, 6, 9);

    int best_location = g.find_warehouse();

    cout << best_location << endl;

    return 0;
}