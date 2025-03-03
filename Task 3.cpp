#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct PageRegister
{
    int PageNumber;
    int age;

    PageRegister(int Page)
    {
        PageNumber = Page;
        age = 0;
    }
};

void FaultCounter(int Frames, string FileName)
{
    fstream file(FileName);
    if (!file)
    {
        cout << "Failed to read File" << endl;
        return;
    }

    vector<int> PageRefs;
    int num;
    while (file >> num)
    {
        PageRefs.push_back(num);
    }
    file.close();

    cout << "Displaying File content: ";
    for (int i = 0; i < PageRefs.size(); i++)
    {
        cout << PageRefs[i] << " ";
    }
    cout << endl;

    vector<PageRegister> Memory;
    int PageFaults = 0;
    int ReferenceCount = 0;

    for (int Page : PageRefs)
    {
        bool Found = false;

        for (auto &Ref : Memory)
        {
            Ref.age++;
            if (Ref.PageNumber == Page)
            {
                Ref.age = 0;
                Found = true;
                break;
            }
        }

        if (!Found)
        {
            if (Memory.size() < Frames)
            {
                Memory.push_back(PageRegister(Page));
            }
            else
            {
                int Oldest = 0;
                for (int i = 1; i < Memory.size(); i++)
                {
                    if (Memory[i].age > Memory[Oldest].age)
                    {
                        Oldest = i;
                    }
                }
                Memory[Oldest] = PageRegister(Page);
            }

            PageFaults++;
        }
    }

    cout << "Total Page Faults: " << PageFaults << endl;
}

int main()
{
    int Frames;
    cout << "Enter amount of Frames: ";
    cin >> Frames;

    cin.ignore();
    string FileName;
    cout << "Enter a file to read page references: ";
    getline(cin, FileName);

    FaultCounter(Frames, FileName);
}
