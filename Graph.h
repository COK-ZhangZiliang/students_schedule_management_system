#include <bits/stdc++.h>
using namespace std;
const int N = 127;          // �ڵ������ֵ
const int INF = 0x3f3f3f3f; // �������ֵ
typedef pair<int, int> PII; // �洢����
#define x first
#define y second

// ����һ���ṹ�����洢���·�������Ϣ
struct path
{
    int cnt = 0;  // ·���ϵĽڵ�����
    int s[N];     // ·���ϵĽڵ���
    int dist = 0; // ·������
};

// ����һ���ṹ������ʾ�ڽӱ��е�һ���ڵ�
struct Node
{
    int dest;
    Node *next;
};

// ����һ���ṹ������ʾ�ڽӱ�
struct AdjList
{
    string place_name;
    PII loc;
    Node *head;
};

// ��Ҫ�޸ģ���װ������
bool collison[N] = {0}; // ��¼hash�Ƿ�����ͻ
path dis[N][N];         // ��¼��������֮��ľ���

// ����һ��������ʾͼ
class Graph
{
public:
    // ���캯��
    Graph();

    // ��������
    ~Graph();

    // ��ʼ�������
    void initVNode();

    // BKDRHash
    // ���ڽ��ص����Ƶ��ַ���ӳ�䵽һ���Ǹ�������Ϊ������±�ֵ
    unsigned int BKDRHash(char *str);

    // ����������ͼ�����һ����
    void addEdge(string src, string dest);

    // ���������Ӧ�������±�ֵ
    unsigned int getIndex(string name);

    // ����������ӡͼ���ڽӱ��ʾ
    void printGraph();

    // dijkstra�㷨
    // ʱ�临�Ӷ�O(mlogn)������mΪ������nΪ����
    void dijkstra(string src, string dest);

    // �ҵ����·��
    void getPath(int s, int *prev, int *temp_dist);

    // ������·��
    void printPath(int s, int d);

    // ��Ҫ��Ϊprivate
public:
    AdjList *array;
};

void Graph::initVNode()
{
    ifstream file("�㼯.txt");
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            // ����Ч������ȡ����
            int loc1 = line.find(' ');
            string name = line.substr(0, loc1);

            // ����Ч������ȡ����
            int loc2 = line.find('(');
            int x = 0, y = 0;
            // ��ȡx
            int flag = 1;
            while (line[++loc2] != ',')
            {
                if (line[loc2] == '-') // ���и���
                {
                    flag = -1;
                    loc2++;
                }
                x = x * 10 + flag * (int)(line[loc2] - '0');
            }
            // ��ȡy
            flag = 1;
            while (line[++loc2] != ')')
            {
                if (line[loc2] == '-') // ���и���
                {
                    flag = -1;
                    loc2++;
                }
                y = y * 10 + flag * (int)(line[loc2] - '0');
            }

            // ���㶥�������ַ�����hash����ֵ
            char c[50];
            strcpy(c, name.c_str()); // ��stringתΪchar *
            unsigned int index = BKDRHash(c);
            // ����̽��
            while (collison[index])
            {
                index = (index + 1) % N;
            }
            collison[index] = true;

            // ����Ϣ�����ڽӱ�
            array[index].place_name = name;
            array[index].loc = make_pair(x, y);
            array[index].head = nullptr;
        }
        file.close();
    }
    else
        cout << "���ļ�ʧ��" << endl;
}

Graph::Graph()
{
    ifstream file("�߼�.txt");
    string line;
    array = new AdjList[N];
    initVNode();

    if (file.is_open())
    {
        while (getline(file, line))
        {
            // ����ÿһ�е��ַ�������ȡ��������Ϣ
            int loc1 = line.find(' ');
            int loc2 = line.find('~');
            int loc3 = line.find(' ', loc1 + 1);

            // ��ȡsrc����
            string name1 = line.substr(0, loc1);
            // ��ȡdest����
            string name2 = line.substr(loc2 + 1, loc3 - loc2 - 1);

            // ���һ����
            addEdge(name1, name2);
        }
        file.close();
    }
    else
        cout << "���ļ�ʧ��" << endl;
}

unsigned int Graph::getIndex(string name)
{
    char c[50];
    strcpy(c, name.c_str()); // ��stringתΪchar *
    unsigned int index = BKDRHash(c);
    while (array[index].place_name != name)
    {
        index = (index + 1) % N;
    }
    return index;
}

void Graph::addEdge(string src, string dest)
{
    // ����src��Ӧ���±�
    int index1 = getIndex(src);
    // ����dest��Ӧ���±�
    int index2 = getIndex(dest);

    //  ���һ��src��dest�ı�
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
        // x�����ϵľ���
        int xd = array[dis[s][d].s[i + 1]].loc.x - array[dis[s][d].s[i]].loc.x;
        // y�����ϵľ���
        int yd = array[dis[s][d].s[i + 1]].loc.y - array[dis[s][d].s[i]].loc.y;
        if (xd != 0)
        {
            if (xd > 0)
                cout << "�� " << array[dis[s][d].s[i]].place_name << " ������ " << xd << "�� "
                     << "���� " << array[dis[s][d].s[i + 1]].place_name << endl;
            else
                cout << "�� " << array[dis[s][d].s[i]].place_name << " ���� " << -xd << "�� "
                     << "���� " << array[dis[s][d].s[i + 1]].place_name << endl;
        }
        else
        {
            if (yd > 0)
                cout << "�� " << array[dis[s][d].s[i]].place_name << " ���� " << yd << "�� "
                     << "���� " << array[dis[s][d].s[i + 1]].place_name << endl;
            else
                cout << "�� " << array[dis[s][d].s[i]].place_name << " ������ " << -yd << " �� "
                     << "���� " << array[dis[s][d].s[i + 1]].place_name << endl;
        }
    }
    cout << "����Ŀ�ĵ�" << endl;
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

        // ��ת·��
        reverse(dis[s][i].s, dis[s][i].s + dis[s][i].cnt);
    }
}

void Graph::dijkstra(string src, string dest)
{
    int s = getIndex(src);
    int d = getIndex(dest);
    if (dis[s][d].cnt) // ���Ѿ�������·����ֱ�����
    {
        printPath(s, d);
        return;
    }

    // С�������ȶ���
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // С����ά������
    pq.push(make_pair(0, s));                                                           // ��������

    int temp_dis[N];                         // ���Դ�㵽����������̾���
    memset(temp_dis, INF, sizeof(temp_dis)); // ��ʼ������ΪINF
    temp_dis[s] = 0;                         // ���Լ��ľ���Ϊ0

    int visit[N];                    // ��¼�Ƿ��Ѿ��ҵ���i����̾���
    memset(visit, 0, sizeof(visit)); // ��ʼ��Ϊû���ҵ�

    int prev[N];                    // ��¼���·����ǰ���ڵ�
    memset(prev, -1, sizeof(prev)); // ÿ���ڵ��ǰ���ڵ��ʼ��Ϊ-1

    while (!pq.empty())
    {
        //   �Ѳ���
        int u = pq.top().y;
        visit[u] = 1;
        pq.pop();
        Node *temp = array[u].head;
        while (temp != nullptr)
        {
            //  ö�����г���
            int v = temp->dest, w = abs(array[u].loc.x - array[v].loc.x) + abs(array[u].loc.y - array[v].loc.y);
            if (!visit[v] && temp_dis[v] > temp_dis[u] + w)
            {
                // �ɳڲ���
                temp_dis[v] = temp_dis[u] + w;
                prev[v] = u;
                pq.push(make_pair(temp_dis[v], v));
            }
            temp = temp->next;
        }
    }

    // �ҳ�s���������е�����·��
    getPath(s, prev, temp_dis);
    printPath(s, d);
    return;
}

Graph::~Graph()
{
    delete[] array;
}