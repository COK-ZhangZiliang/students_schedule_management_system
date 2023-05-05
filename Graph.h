#include <bits/stdc++.h>
using namespace std;
static const int N = 127;          // �ڵ������ֵ
static const int INF = 0x3f3f3f3f; // �������ֵ
typedef pair<int, int> PII;        // �洢����
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
static bool collison[N] = {0}; // ��¼hash�Ƿ�����ͻ
static path dis[N][N];         // ��¼��������֮��ľ���
static int tabu[N][N];         // ���ɱ�
static int tabulen[N][N];      // �����ڣ����ֵΪ5

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
    void getPath(unsigned int s, int *prev, int *temp_dist);

    // ������·��
    void printPath(unsigned int s, unsigned int d);

    // ���������㷨
    // ���ɣ�Tabu Search���㷨��һ��������ʽ(meta-heuristic)��������㷨
    // ����һ����ʼ���н������ѡ��һϵ�е��ض����������ƶ�����Ϊ��̽��ѡ��ʵ�����ض���Ŀ�꺯��ֵ�仯�����ƶ�
    // Ϊ�˱�������ֲ����Ž⣬TS�����в�����һ�����ġ����䡱����
    // ���Ѿ����е��Ż����̽��м�¼��ѡ��ָ����һ�����������������Tabu��Ľ���

    // ����·������
    int calcPathLen(const vector<int> &path);
    // ��������λ�õ�ֵ
    void swapVal(int &a, int &b);
    // ���ɺ�ѡ��
    vector<int> generateCandidate(const vector<int> &cur_path);
    // �ж�·���Ƿ��ڽ��ɱ���
    bool isInTabuList(const vector<int> &path);
    // ���½��ɱ�ͽ�����
    void updateTabuList(const vector<int> &cur_path, const vector<int> &new_path);
    // ���������㷨
    void tabuSearch(string src, vector<string> dst);

    // ��Ҫ�޸�Ϊprivate
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

void Graph::printPath(unsigned int s, unsigned int d)
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
    cout << "����Ŀ�ĵ� " << array[dis[s][d].s[dis[s][d].cnt - 1]].place_name << endl;
}

void Graph::getPath(unsigned int s, int *prev, int *temp_dist)
{
    // �޸�:˫�������·��
    for (int i = 0; i < N; i++)
    {
        dis[s][i].dist = temp_dist[i];
        int temp = i;
        while (temp != -1)
        {
            dis[s][i].s[dis[s][i].cnt++] = temp;
            temp = prev[temp];
        }

        // ˫�������·��
        memcpy(dis[i][s].s, dis[s][i].s, sizeof(dis[s][i].s));
        dis[i][s].dist = dis[s][i].dist;
        dis[i][s].cnt = dis[s][i].cnt;

        // ��ת·��
        reverse(dis[s][i].s, dis[s][i].s + dis[s][i].cnt);
    }
}

void Graph::dijkstra(string src, string dest)
{
    unsigned int s = getIndex(src);
    unsigned int d = getIndex(dest);
    if (dis[s][d].cnt) // ���Ѿ�������·����ֱ�����
    {
        // printPath(s, d);
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

    // printPath(s, d);
    return;
}

Graph::~Graph()
{
    delete[] array;
}

int Graph::calcPathLen(const vector<int> &path)
{
    int res = 0;
    for (int i = 0; i < path.size() - 1; i++)
    {
        dijkstra(array[path[i]].place_name, array[path[i + 1]].place_name);
        res += dis[path[i]][path[i + 1]].dist;
    }
    res += dis[path[path.size() - 1]][path[0]].dist;
    return res;
}

void Graph::swapVal(int &a, int &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

vector<int> Graph::generateCandidate(const vector<int> &cur_path)
{
    vector<int> candidate(cur_path);
    srand(unsigned(time(0))); // �����������
    int x = rand() % cur_path.size();
    int y = rand() % cur_path.size();
    while (x == y)
    {
        y = rand() % cur_path.size();
    }
    swapVal(candidate[x], candidate[y]);
    return candidate;
}

bool Graph::isInTabuList(const vector<int> &path)
{
    for (int i = 1; i < path.size(); i++)
    {
        if (tabu[path[i - 1]][path[i]])
        {
            return true;
        }
    }
    if (tabu[path[path.size() - 1]][path[0]])
    {
        return true;
    }
    return false;
}

void Graph::updateTabuList(const vector<int> &cur_path, const vector<int> &new_path)
{
    // tabulen_max=5
    for (int i = 1; i < cur_path.size(); i++)
    {
        tabulen[cur_path[i - 1]][cur_path[i]]--;
        if (tabulen[cur_path[i - 1]][cur_path[i]] <= 0)
        {
            tabu[cur_path[i - 1]][cur_path[i]] = 0;
        }
        tabulen[new_path[i - 1]][new_path[i]] = 5;
        tabu[new_path[i - 1]][new_path[i]] = 1;
    }
    tabulen[cur_path[cur_path.size() - 1]][cur_path[0]]--;
    if (tabulen[cur_path[cur_path.size() - 1]][cur_path[0]] <= 0)
    {
        tabu[cur_path[cur_path.size() - 1]][cur_path[0]] = 0;
    }
    tabulen[new_path[cur_path.size() - 1]][new_path[0]] = 5;
    tabu[new_path[cur_path.size() - 1]][new_path[0]] = 1;
}

void Graph::tabuSearch(string src, vector<string> dst)
{
    // ��ʼ����ǰ��
    vector<int> cur_path;
    cur_path.push_back(getIndex(src));
    for (int i = 0; i < dst.size(); i++)
    {
        cur_path.push_back(getIndex(dst[i]));
    }

    // �������һ��������
    srand(unsigned(time(0))); // �����������
    random_shuffle(cur_path.begin() + 1, cur_path.end());
    vector<int> best_path = cur_path;

    // ��ʼ�����ɱ�ͽ�����
    memset(tabu, 0, sizeof(tabu));
    memset(tabulen, 0, sizeof(tabulen));

    // �����㷨����
    int maxiter = 500;

    // ��������
    for (int iter = 0; iter < maxiter; iter++)
    {
        // ���ɺ�ѡ�Ⲣѡ�����е�һ�����Ž�
        vector<int> new_path;
        int cnt = 0;
        while (cnt < maxiter)
        {
            new_path = generateCandidate(cur_path);
            if (!isInTabuList(new_path))
            {
                break;
            }
            cnt++;
        }

        // ���½��ɱ�ͽ�����
        updateTabuList(cur_path, new_path);

        // ���µ�ǰ������Ž�
        cur_path = new_path;
        if (calcPathLen(cur_path) < calcPathLen(best_path))
        {
            best_path = cur_path;
        }
    }

    // �������·��
    for (int i = 0; i < best_path.size() - 1; i++)
    {
        printPath(best_path[i], best_path[i + 1]);
    }
    printPath(best_path[best_path.size() - 1], best_path[0]);
}