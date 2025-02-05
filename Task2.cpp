#include <iostream>
#include <pthread.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

typedef map<string, int> FrequencyCount;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct ThreadData
{
    vector<string> Lines;
    FrequencyCount *WordCount;
    int ThreadID;
};

void *ThreadOperation(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    for (const string &Line : data->Lines)
    {
        string Word;
        istringstream Split(Line);

        while (Split >> Word)
        {
            pthread_mutex_lock(&lock);
            (*data->WordCount)[Word]++;
            pthread_mutex_unlock(&lock);
        }
    }

    return nullptr;
}

int main()
{
    int Segments;
    cout << "Enter amount of segments: ";
    cin >> Segments;
    cin.ignore();

    string FileName;
    cout << "Enter file name: ";
    getline(cin, FileName);

    FrequencyCount FreqCount;

    ifstream file(FileName);
    if (!file)
    {
        cout << "Failed to access file" << endl;
        return 1;
    }

    vector<string> DocContent;

    string Line;

    while (getline(file, Line))
    {
        DocContent.push_back(Line);
    }

    int DocSize = DocContent.size();
    int SegmentSize = DocSize / Segments;
    file.close();

    vector<pthread_t> Threads(Segments);
    vector<ThreadData> data(Segments);

    for (int i = 0; i < Segments; i++)
    {
        int start = i * SegmentSize;
        int End = (i == Segments - 1) ? DocSize : (i + 1) * SegmentSize;

        data[i].Lines = vector<string>(DocContent.begin() + start, DocContent.begin() + End);
        data[i].WordCount = &FreqCount;
        data[i].ThreadID = i + 1;

        pthread_create(&Threads[i], nullptr, ThreadOperation, &data[i]);
    }

    for (int i = 0; i < Segments; i++)
    {
        pthread_join(Threads[i], nullptr);
    }

    cout << "Final word frequency count:" << endl;
    for (const auto &pair : FreqCount)
    {
        cout << pair.first << " occurred " << pair.second << " times" << endl;
    }

    return 0;
}
