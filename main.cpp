#include "Graph.h"
using namespace std;

int main()
{
    Graph gh;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            if (collison[i] && collison[j] && i != j)
                gh.dijkstra(gh.array[i].place_name, gh.array[j].place_name);
            cout << endl;
        }
    return 0;
}