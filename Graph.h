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
    int cnt = 0;     // ·���ϵĽڵ�����
    int s[N];        // ·���ϵĽڵ���
    double dist = 0; // ·������
    int mode;
    double congest; // ӵ���̶�
};

// ����һ���ṹ������ʾ�ڽӱ��е�һ���ڵ�
struct Node
{
    int dest;
    Node *next;
    // flag���ڱ���Ƿ���ֻ�ɲ��еı� =1ֻ�ɲ��� =0���ɲ����ֿ����ﳵ
    int flag;
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
static path dis1[N][N];        // ��¼��������֮��ľ���
static path dis2[N][N];        // ��¼�ɲ����ﳵ��ʽ����̾���
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
    void addEdge(string src, string dest, int flag);

    // ���������Ӧ�������±�ֵ
    unsigned int getIndex(string name);

    // ����������ӡͼ���ڽӱ��ʾ
    void printGraph();

    // dijkstra�㷨
    // ʱ�临�Ӷ�O(mlogn)������mΪ������nΪ����
    void dijkstra(string src, string dest, int mode);

    // �ҵ����·��
    void getPath(unsigned int s, int *prev, int *transportation, int *temp_dist, int mode, double *congestion);

    // ������·��
    void printPath(unsigned int s, unsigned int d, int mode);

    // ���������㷨
    // ���ɣ�Tabu Search���㷨��һ��������ʽ(meta-heuristic)��������㷨
    // ����һ����ʼ���н������ѡ��һϵ�е��ض����������ƶ�����Ϊ��̽��ѡ��ʵ�����ض���Ŀ�꺯��ֵ�仯�����ƶ�
    // Ϊ�˱�������ֲ����Ž⣬TS�����в�����һ�����ġ����䡱����
    // ���Ѿ����е��Ż����̽��м�¼��ѡ��ָ����һ�����������������Tabu��Ľ���

    // ����·������
    int calcPathLen(const vector<int> &path, int mode);
    // ��������λ�õ�ֵ
    void swapVal(int &a, int &b);
    // ���ɺ�ѡ��
    vector<int> generateCandidate(const vector<int> &cur_path);
    // �ж�·���Ƿ��ڽ��ɱ���
    bool isInTabuList(const vector<int> &path);
    // ���½��ɱ�ͽ�����
    void updateTabuList(const vector<int> &cur_path, const vector<int> &new_path);
    // ���������㷨
    void tabuSearch(string src, vector<string> dst, int mode);

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

            int flag;
            // ����ֻ���Բ��еı�
            if (line.find("only") != string::npos)
                flag = 1;
            else
                flag = 0;

            // ��ȡsrc����
            string name1 = line.substr(0, loc1);
            // ��ȡdest����
            string name2 = line.substr(loc2 + 1, loc3 - loc2 - 1);

            // ���һ����
            addEdge(name1, name2, flag);
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

void Graph::addEdge(string src, string dest, int flag)
{
    // ����src��Ӧ���±�
    int index1 = getIndex(src);
    // ����dest��Ӧ���±�
    int index2 = getIndex(dest);

    //  ���һ��src��dest�ı�
    Node *newNode = new Node{index2, nullptr, flag};
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

void Graph::printPath(unsigned int s, unsigned int d, int mode)
{
    if (mode == 0)
    {
        cout << "��ͨ��ʽ������" << endl;
        for (int i = 0, j = 1; i < dis1[s][d].cnt - 1; i = j - 1)
        {
            int xd = 0, yd = 0;
            while (j <= dis1[s][d].cnt - 1)
            {
                // x�����ϵľ���
                xd += array[dis1[s][d].s[j]].loc.x - array[dis1[s][d].s[j - 1]].loc.x;
                // y�����ϵľ���
                yd += array[dis1[s][d].s[j]].loc.y - array[dis1[s][d].s[j - 1]].loc.y;
                if (xd != 0 && yd != 0)
                {
                    xd -= array[dis1[s][d].s[j]].loc.x - array[dis1[s][d].s[j - 1]].loc.x;
                    yd -= array[dis1[s][d].s[j]].loc.y - array[dis1[s][d].s[j - 1]].loc.y;
                    break;
                }
                j++;
            }
            if (xd != 0)
            {
                if (xd > 0)
                    cout << "�� " << array[dis1[s][d].s[i]].place_name << " ������ " << xd << "�� "
                         << "���� " << array[dis1[s][d].s[j - 1]].place_name << endl;
                else
                    cout << "�� " << array[dis1[s][d].s[i]].place_name << " ���� " << -xd << "�� "
                         << "���� " << array[dis1[s][d].s[j - 1]].place_name << endl;
            }
            else
            {
                if (yd > 0)
                    cout << "�� " << array[dis1[s][d].s[i]].place_name << " ���� " << yd << "�� "
                         << "���� " << array[dis1[s][d].s[j - 1]].place_name << endl;
                else
                    cout << "�� " << array[dis1[s][d].s[i]].place_name << " ������ " << -yd << " �� "
                         << "���� " << array[dis1[s][d].s[j - 1]].place_name << endl;
            }
        }
        cout << "����Ŀ�ĵ�" << array[dis1[s][d].s[dis1[s][d].cnt - 1]].place_name << endl;
    }
    else
    {
        cout << "��ͨ��ʽ������+���г�" << endl;
        for (int i = 0; i < dis2[s][d].cnt - 1; i++)
        {
            // x�����ϵľ���
            int xd = array[dis2[s][d].s[i + 1]].loc.x - array[dis2[s][d].s[i]].loc.x;
            // y�����ϵľ���
            int yd = array[dis2[s][d].s[i + 1]].loc.y - array[dis2[s][d].s[i]].loc.y;
            if (xd != 0)
            {
                if (dis2[s][dis2[s][d].s[i + 1]].mode == 0)
                {
                    if (xd > 0)
                    {
                        cout << "�� " << array[dis2[s][d].s[i]].place_name << " ������ " << xd << "�� "
                             << "���� " << array[dis2[s][d].s[i + 1]].place_name << endl;
                        cout << "ӵ���̶�Ϊ��" << dis2[s][dis2[s][d].s[i + 1]].congest << endl;
                    }
                    else
                    {
                        cout << "�� " << array[dis2[s][d].s[i]].place_name << " ���� " << -xd << "�� "
                             << "���� " << array[dis2[s][d].s[i + 1]].place_name << endl;
                        cout << "ӵ���̶�Ϊ��" << dis2[s][dis2[s][d].s[i + 1]].congest << endl;
                    }
                }
                else
                {
                    if (xd > 0)
                    {
                        cout << "�� " << array[dis2[s][d].s[i]].place_name << " ���������г� " << xd << "�� "
                             << "���� " << array[dis2[s][d].s[i + 1]].place_name << endl;
                        cout << "ӵ���̶�Ϊ��" << dis2[s][dis2[s][d].s[i + 1]].congest << endl;
                    }
                    else
                    {
                        cout << "�� " << array[dis2[s][d].s[i]].place_name << " �������г� " << -xd << "�� "
                             << "���� " << array[dis2[s][d].s[i + 1]].place_name << endl;
                        cout << "ӵ���̶�Ϊ��" << dis2[s][dis2[s][d].s[i + 1]].congest << endl;
                    }
                }
            }
            else
            {
                if (dis2[s][dis2[s][d].s[i + 1]].mode == 0)
                {
                    if (yd > 0)
                    {
                        cout << "�� " << array[dis2[s][d].s[i]].place_name << " ���� " << yd << "�� "
                             << "���� " << array[dis2[s][d].s[i + 1]].place_name << endl;
                        cout << "ӵ���̶�Ϊ��" << dis2[s][dis2[s][d].s[i + 1]].congest << endl;
                    }
                    else
                    {
                        cout << "�� " << array[dis2[s][d].s[i]].place_name << " ������ " << -yd << " �� "
                             << "���� " << array[dis2[s][d].s[i + 1]].place_name << endl;
                        cout << "ӵ���̶�Ϊ��" << dis2[s][dis2[s][d].s[i + 1]].congest << endl;
                    }
                }
                else
                {
                    if (yd > 0)
                    {
                        cout << "�� " << array[dis2[s][d].s[i]].place_name << " �������г� " << yd << "�� "
                             << "���� " << array[dis2[s][d].s[i + 1]].place_name << endl;
                        cout << "ӵ���̶�Ϊ��" << dis2[s][dis2[s][d].s[i + 1]].congest << endl;
                    }
                    else
                    {
                        cout << "�� " << array[dis2[s][d].s[i]].place_name << " ���������г� " << -yd << " �� "
                             << "���� " << array[dis2[s][d].s[i + 1]].place_name << endl;
                        cout << "ӵ���̶�Ϊ��" << dis2[s][dis2[s][d].s[i + 1]].congest << endl;
                    }
                }
            }
        }
        cout << "����Ŀ�ĵ�" << array[dis2[s][d].s[dis2[s][d].cnt - 1]].place_name << endl;
    }
}

void Graph::getPath(unsigned int s, int *prev, int *transportation, int *temp_dist, int mode, double *congestion)
{
    // �޸�:˫�������·��
    if (mode == 0)
    {
        for (int i = 0; i < N; i++)
        {
            if (dis1[s][i].cnt) // �Ѿ�������·����������
                continue;
            dis1[s][i].dist = temp_dist[i];
            int temp = i;
            dis1[s][i].mode = transportation[i];
            dis1[i][s].mode = transportation[i];
            while (temp != -1)
            {
                dis1[s][i].s[dis1[s][i].cnt++] = temp;
                temp = prev[temp];
            }

            // ˫�������·��
            memcpy(dis1[i][s].s, dis1[s][i].s, sizeof(dis1[s][i].s));
            dis1[i][s].dist = dis1[s][i].dist;
            dis1[i][s].cnt = dis1[s][i].cnt;

            // ��ת·��
            reverse(dis1[s][i].s, dis1[s][i].s + dis1[s][i].cnt);
        }
    }
    else
    {
        for (int i = 0; i < N; i++)
        {
            if (dis2[s][i].cnt) // �Ѿ�������·����������
                continue;
            dis2[s][i].dist = temp_dist[i];
            dis2[s][i].mode = transportation[i];
            dis2[i][s].mode = transportation[i];
            dis2[s][i].congest = congestion[i];
            dis2[i][s].congest = congestion[i];
            int temp = i;
            while (temp != -1)
            {
                dis2[s][i].s[dis2[s][i].cnt++] = temp;
                temp = prev[temp];
            }

            // ˫�������·��
            memcpy(dis2[i][s].s, dis2[s][i].s, sizeof(dis2[s][i].s));
            dis2[i][s].dist = dis2[s][i].dist;
            dis2[i][s].cnt = dis2[s][i].cnt;

            // ��ת·��
            reverse(dis2[s][i].s, dis2[s][i].s + dis2[s][i].cnt);
        }
    }
}

void Graph::dijkstra(string src, string dest, int mode)
{
    unsigned int s = getIndex(src);
    unsigned int d = getIndex(dest);
    if ((mode == 0 && dis1[s][d].cnt) || (mode == 1 && dis2[s][d].cnt)) // ���Ѿ�������·����ֱ�����
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

    int prev[N];                    // ��¼���·����ǰ���ڵ�,
    int transportation[N];          // ��ͨ��ʽ
    double congestion[N];           // ӵ���̶�
    memset(prev, -1, sizeof(prev)); // ÿ���ڵ��ǰ���ڵ��ʼ��Ϊ-1
    memset(transportation, 0, sizeof(transportation));
    memset(congestion, 0, sizeof(congestion));

    while (!pq.empty())
    {
        //   �Ѳ���
        int u = pq.top().y;
        visit[u] = 1;
        pq.pop();
        Node *temp = array[u].head;
        default_random_engine e;
        uniform_int_distribution<int> d(0, 500);
        e.seed(time(0));

        while (temp != nullptr)
        {
            //  ö�����г���
            int v = temp->dest;
            double w = abs(array[u].loc.x - array[v].loc.x) + abs(array[u].loc.y - array[v].loc.y);

            int is_take_bicycle = 0; // �Ƿ���������г��ķ�ʽ

            // �������һ��0-5.0�ĸ�����
            int cgs = d(e);
            double congst = cgs / 100.0; // ��������ĵ�·ӵ����

            if (mode == 1 && temp->flag == 0)
            {
                if (w / 3 * congst < w)
                {
                    w = w / 3 * congst;
                    is_take_bicycle = 1;
                }
            }

            if (!visit[v] && temp_dis[v] > temp_dis[u] + w)
            {
                // �ɳڲ���
                temp_dis[v] = temp_dis[u] + w;
                prev[v] = u;
                transportation[v] = is_take_bicycle;
                congestion[v] = congst;

                pq.push(make_pair(temp_dis[v], v));
            }
            temp = temp->next;
        }
    }

    // �ҳ�s���������е�����·��
    getPath(s, prev, transportation, temp_dis, mode, congestion);

    // printPath(s, d);
    return;
}

Graph::~Graph()
{
    delete[] array;
}

int Graph::calcPathLen(const vector<int> &path, int mode)
{
    int res = 0;
    if (mode == 0)
    {
        for (int i = 0; i < path.size() - 1; i++)
        {
            dijkstra(array[path[i]].place_name, array[path[i + 1]].place_name, mode);
            res += dis1[path[i]][path[i + 1]].dist;
        }

        dijkstra(array[path[path.size() - 1]].place_name, array[path[0]].place_name, mode);
        res += dis1[path[path.size() - 1]][path[0]].dist;
    }
    else
    {
        for (int i = 0; i < path.size() - 1; i++)
        {
            dijkstra(array[path[i]].place_name, array[path[i + 1]].place_name, mode);
            res += dis2[path[i]][path[i + 1]].dist;
        }

        dijkstra(array[path[path.size() - 1]].place_name, array[path[0]].place_name, mode);
        res += dis2[path[path.size() - 1]][path[0]].dist;
    }
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
    // srand(unsigned(time(0))); // �����������
    int x = rand() % cur_path.size();
    int y = rand() % cur_path.size();
    if (x == 0)
        x++;
    if (y == 0)
        y++;
    while (x == y)
    {
        y = rand() % cur_path.size();
        if (y == 0)
            y++;
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

void Graph::tabuSearch(string src, vector<string> dst, int mode)
{
    // ��ʼ����ǰ��
    vector<int> cur_path;
    cur_path.push_back(getIndex(src));
    for (int i = 0; i < dst.size(); i++)
    {
        cur_path.push_back(getIndex(dst[i]));
    }

    // �������һ��������
    // srand(unsigned(time(0))); // �����������
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

            // ���½��ɱ�ͽ�����
            updateTabuList(cur_path, new_path);
        }

        // ���½��ɱ�ͽ�����
        updateTabuList(cur_path, new_path);

        // ���µ�ǰ������Ž�
        cur_path = new_path;
        if (calcPathLen(cur_path, mode) < calcPathLen(best_path, mode))
        {
            best_path = cur_path;
        }
    }

    // �������·��
    for (int i = 0; i < best_path.size(); i++)
    {
        cout << array[best_path[i]].place_name << endl;
    }

    for (int i = 0; i < best_path.size() - 1; i++)
    {
        cout << array[best_path[i]].place_name << endl;
        // dijkstra(array[best_path[i]].place_name, array[best_path[i + 1]].place_name);
        printPath(best_path[i], best_path[i + 1], mode);
    }

    // cout << array[best_path[best_path.size() - 1]].place_name << " " << array[best_path[0]].place_name << endl;
    printPath(best_path[best_path.size() - 1], best_path[0], mode);
}