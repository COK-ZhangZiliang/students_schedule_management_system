#include <bits/stdc++.h>
using namespace std;
const int N = 127;          // �ڵ������ֵ
typedef pair<int, int> PII; // �洢����
#define x first
#define y second
bool collison[N] = {0};

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

private:
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

Graph::~Graph()
{
    delete[] array;
}