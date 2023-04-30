#include <bits/stdc++.h>
using namespace std;
const int N = 127;          // 节点数最大值
const int INF = 0x3f3f3f3f; // 距离最大值
typedef pair<int, int> PII; // 存储坐标
#define x first
#define y second

// 定义一个结构体来存储最短路径相关信息
struct path
{
    int cnt = 0;  // 路径上的节点总数
    int s[N];     // 路径上的节点编号
    int dist = 0; // 路径长度
};

// 定义一个结构体来表示邻接表中的一个节点
struct Node
{
    int dest;
    Node *next;
};

// 定义一个结构体来表示邻接表
struct AdjList
{
    string place_name;
    PII loc;
    Node *head;
};

// 需要修改，封装到类中
bool collison[N] = {0}; // 记录hash是否发生冲突
path dis[N][N];         // 记录任意两点之间的距离

// 定义一个类来表示图
class Graph
{
public:
    // 构造函数
    Graph();

    // 析构函数
    ~Graph();

    // 初始化顶点表
    void initVNode();

    // BKDRHash
    // 用于将地点名称的字符串映射到一个非负整数作为数组的下标值
    unsigned int BKDRHash(char *str);

    // 函数用来向图中添加一条边
    void addEdge(string src, string dest);

    // 计算地名对应的数组下标值
    unsigned int getIndex(string name);

    // 函数用来打印图的邻接表表示
    void printGraph();

    // dijkstra算法
    // 时间复杂度O(mlogn)，其中m为边数，n为点数
    void dijkstra(string src, string dest);

    // 找到最短路径
    void getPath(int s, int *prev, int *temp_dist);

    // 输出最短路径
    void printPath(int s, int d);

    // 需要改为private
public:
    AdjList *array;
};

void Graph::initVNode()
{
    ifstream file("点集.txt");
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            // 将有效名称提取出来
            int loc1 = line.find(' ');
            string name = line.substr(0, loc1);

            // 将有效坐标提取出来
            int loc2 = line.find('(');
            int x = 0, y = 0;
            // 读取x
            int flag = 1;
            while (line[++loc2] != ',')
            {
                if (line[loc2] == '-') // 特判负数
                {
                    flag = -1;
                    loc2++;
                }
                x = x * 10 + flag * (int)(line[loc2] - '0');
            }
            // 读取y
            flag = 1;
            while (line[++loc2] != ')')
            {
                if (line[loc2] == '-') // 特判负数
                {
                    flag = -1;
                    loc2++;
                }
                y = y * 10 + flag * (int)(line[loc2] - '0');
            }

            // 计算顶点名称字符串的hash函数值
            char c[50];
            strcpy(c, name.c_str()); // 将string转为char *
            unsigned int index = BKDRHash(c);
            // 线性探测
            while (collison[index])
            {
                index = (index + 1) % N;
            }
            collison[index] = true;

            // 将信息存入邻接表
            array[index].place_name = name;
            array[index].loc = make_pair(x, y);
            array[index].head = nullptr;
        }
        file.close();
    }
    else
        cout << "打开文件失败" << endl;
}

Graph::Graph()
{
    ifstream file("边集.txt");
    string line;
    array = new AdjList[N];
    initVNode();

    if (file.is_open())
    {
        while (getline(file, line))
        {
            // 处理每一行的字符串，提取出有用信息
            int loc1 = line.find(' ');
            int loc2 = line.find('~');
            int loc3 = line.find(' ', loc1 + 1);

            // 读取src地名
            string name1 = line.substr(0, loc1);
            // 读取dest地名
            string name2 = line.substr(loc2 + 1, loc3 - loc2 - 1);

            // 添加一条边
            addEdge(name1, name2);
        }
        file.close();
    }
    else
        cout << "打开文件失败" << endl;
}

unsigned int Graph::getIndex(string name)
{
    char c[50];
    strcpy(c, name.c_str()); // 将string转为char *
    unsigned int index = BKDRHash(c);
    while (array[index].place_name != name)
    {
        index = (index + 1) % N;
    }
    return index;
}

void Graph::addEdge(string src, string dest)
{
    // 计算src对应的下标
    int index1 = getIndex(src);
    // 计算dest对应的下标
    int index2 = getIndex(dest);

    //  添加一条src到dest的边
    Node *newNode = new Node{index2, nullptr};
    newNode->next = array[index1].head;
    array[index1].head = newNode;
}

void Graph::printGraph()
{
    for (int v = 0; v < N; ++v)
    {
        if (collison[v])
        {
            Node *pCrawl = array[v].head;
            cout << array[v].place_name << ":";
            while (pCrawl)
            {
                cout << " " << array[pCrawl->dest].place_name;
                pCrawl = pCrawl->next;
            }
            cout << endl;
        }
    }
}

unsigned int Graph::BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF) % N;
}

void Graph::printPath(int s, int d)
{
    for (int i = 0; i < dis[s][d].cnt - 1; i++)
    {
        // x方向上的距离
        int xd = array[dis[s][d].s[i + 1]].loc.x - array[dis[s][d].s[i]].loc.x;
        // y方向上的距离
        int yd = array[dis[s][d].s[i + 1]].loc.y - array[dis[s][d].s[i]].loc.y;
        if (xd != 0)
        {
            if (xd > 0)
                cout << "从 " << array[dis[s][d].s[i]].place_name << " 向南走 " << xd << "米 "
                     << "到达 " << array[dis[s][d].s[i + 1]].place_name << endl;
            else
                cout << "从 " << array[dis[s][d].s[i]].place_name << " 向北走 " << -xd << "米 "
                     << "到达 " << array[dis[s][d].s[i + 1]].place_name << endl;
        }
        else
        {
            if (yd > 0)
                cout << "从 " << array[dis[s][d].s[i]].place_name << " 向东走 " << yd << "米 "
                     << "到达 " << array[dis[s][d].s[i + 1]].place_name << endl;
            else
                cout << "从 " << array[dis[s][d].s[i]].place_name << " 向西走 " << -yd << " 米 "
                     << "到达 " << array[dis[s][d].s[i + 1]].place_name << endl;
        }
    }
    cout << "到达目的地" << endl;
}

void Graph::getPath(int s, int *prev, int *temp_dist)
{
    for (int i = 0; i < N; i++)
    {
        dis[s][i].dist = temp_dist[i];
        int temp = i;
        while (temp != -1)
        {
            dis[s][i].s[dis[s][i].cnt++] = temp;
            temp = prev[temp];
        }

        // 翻转路径
        reverse(dis[s][i].s, dis[s][i].s + dis[s][i].cnt);
    }
}

void Graph::dijkstra(string src, string dest)
{
    int s = getIndex(src);
    int d = getIndex(dest);
    if (dis[s][d].cnt) // 若已经求得最短路径则直接输出
    {
        printPath(s, d);
        return;
    }

    // 小根堆优先队列
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // 小根堆维护距离
    pq.push(make_pair(0, s));                                                           // 将起点入队

    int temp_dis[N];                         // 存放源点到各个结点的最短距离
    memset(temp_dis, INF, sizeof(temp_dis)); // 初始化距离为INF
    temp_dis[s] = 0;                         // 到自己的距离为0

    int visit[N];                    // 记录是否已经找到到i的最短距离
    memset(visit, 0, sizeof(visit)); // 初始化为没有找到

    int prev[N];                    // 记录最短路径的前驱节点
    memset(prev, -1, sizeof(prev)); // 每个节点的前驱节点初始化为-1

    while (!pq.empty())
    {
        //   堆不空
        int u = pq.top().y;
        visit[u] = 1;
        pq.pop();
        Node *temp = array[u].head;
        while (temp != nullptr)
        {
            //  枚举所有出边
            int v = temp->dest, w = abs(array[u].loc.x - array[v].loc.x) + abs(array[u].loc.y - array[v].loc.y);
            if (!visit[v] && temp_dis[v] > temp_dis[u] + w)
            {
                // 松弛操作
                temp_dis[v] = temp_dis[u] + w;
                prev[v] = u;
                pq.push(make_pair(temp_dis[v], v));
            }
            temp = temp->next;
        }
    }

    // 找出s到其余所有点的最短路径
    getPath(s, prev, temp_dis);
    printPath(s, d);
    return;
}

Graph::~Graph()
{
    delete[] array;
}