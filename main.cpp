#include "Graph.h"
using namespace std;

int main()
{
    clock_t start, end; // 定义clock_t变量
    start = clock();    // 开始时间

    Graph gh;

    // for (int i = 0; i < N; i++)
    //     for (int j = 0; j < N; j++)
    //     {
    //         if (collison[i] && collison[j] && i != j)
    //             gh.dijkstra(gh.array[i].place_name, gh.array[j].place_name);
    //         cout << endl;
    //     }

    // dijkstra算法测试
    // int mode = 0;
    // gh.dijkstra("路口1", "教三楼北门", mode);
    // gh.printPath(gh.getIndex("路口1"), gh.getIndex("教三楼北门"), mode);

    // tabu算法测试
    
    int cnt = 0;
    vector<string> dst;
    for (int i = 120; i < 127; i++)
        if (collison[i] && gh.array[i].place_name != "路口1")
        {
            dst.push_back(gh.array[i].place_name);
            cnt++;
        }
    cout << cnt << endl;
    gh.tabuSearch("路口1", dst, 0);

    end = clock();                                                            // 结束时间
    cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl; // 输出时间（单位：ｓ）
    return 0;
}