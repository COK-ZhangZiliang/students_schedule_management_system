#include <bits/stdc++.h>
using namespace std;
const int N = 127;          // 节点数最大值
typedef pair<int, int> PII; // 存储坐标
#define x first
#define y second
bool collison[N] = {0};

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

private:
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

Graph::~Graph()
{
    delete[] array;
}