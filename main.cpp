#include "Graph.h"
using namespace std;

int main()
{
    clock_t start, end; // ����clock_t����
    start = clock();    // ��ʼʱ��

    Graph gh;

    // for (int i = 0; i < N; i++)
    //     for (int j = 0; j < N; j++)
    //     {
    //         if (collison[i] && collison[j] && i != j)
    //             gh.dijkstra(gh.array[i].place_name, gh.array[j].place_name);
    //         cout << endl;
    //     }

    // dijkstra�㷨����
    // int mode = 0;
    // gh.dijkstra("·��1", "����¥����", mode);
    // gh.printPath(gh.getIndex("·��1"), gh.getIndex("����¥����"), mode);

    // tabu�㷨����
    
    int cnt = 0;
    vector<string> dst;
    for (int i = 120; i < 127; i++)
        if (collison[i] && gh.array[i].place_name != "·��1")
        {
            dst.push_back(gh.array[i].place_name);
            cnt++;
        }
    cout << cnt << endl;
    gh.tabuSearch("·��1", dst, 0);

    end = clock();                                                            // ����ʱ��
    cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl; // ���ʱ�䣨��λ����
    return 0;
}