#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

void PrintMatrix(vector<vector<int>> M, int row, int column)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            cout << M[i][j] << " ";
        }
    }
}

bool DeadLockDetection(vector<int> &E, vector<vector<int>> &C, vector<vector<int>> &R, int Processes, int ResourceTypes)
{
    vector<int> Resources = E;
    vector<bool> Done(Processes, false);

    for (int i = 0; i < Processes; i++)
    {
        for (int j = 0; j < ResourceTypes; j++)
        {
            Resources[j] -= C[i][j];
        }
    }

    cout << "Resources available: ";
    for (int i = 0; i < ResourceTypes; i++)
    {
        cout << Resources[i] << " ";
    }

    cout << endl;

    bool Progression = true;

    while (Progression)
    {
        Progression = false;
        for (int i = 0; i < Processes; i++)
        {
            if (!Done[i])
            {
                bool Runnable = true;
                for (int j = 0; j < ResourceTypes; j++)
                {
                    if (R[i][j] > Resources[j])
                    {
                        Runnable = false;
                        break;
                    }
                }
                if (Runnable)
                {
                    for (int j = 0; j < ResourceTypes; j++)
                    {
                        Resources[j] += C[i][j];
                    }
                    Done[i] = true;
                    Progression = true;
                }
            }
        }
    }

    vector<int> Deadlocks;
    for (int i = 0; i < Processes; i++)
    {
        if (!Done[i])
        {
            Deadlocks.push_back(i);
        }
    }

    if (!Deadlocks.empty())
    {
        cout << "Deadlocks Detected: ";
        for (int i = 0; i < Deadlocks.size(); i++)
        {
            cout << "P" << i << " ";
        }
        cout << endl;
        return true;
    }
    cout << "No deadlocks detected" << endl;
    return false;
}

void ProcessFile(string FileName)
{
    ifstream File(FileName);

    if (!File)
    {
        cout << "Failed to open file " << endl;
        return;
    }

    int Processes, ResourceTypes;
    File >> Processes >> ResourceTypes;
    cout << "Number of Proccesses: " << Processes << endl;
    cout << "Number of Resource types: " << ResourceTypes << endl;

    vector<int> E(ResourceTypes);

    cout << "Resources of Vector E: ";
    for (int i = 0; i < ResourceTypes; i++)
    {
        File >> E[i];
        cout << E[i] << " ";
    }

    cout << endl;

    vector<vector<int>> C(Processes, vector<int>(ResourceTypes));

    cout << "Allocation Matrix C: ";
    for (int i = 0; i < Processes; i++)
    {
        for (int j = 0; j < ResourceTypes; j++)
        {
            File >> C[i][j];
        }
    }

    PrintMatrix(C, Processes, ResourceTypes);

    cout << endl;
    vector<vector<int>> Requests(Processes, vector<int>(ResourceTypes));

    cout << "Request Matrix: ";
    for (int i = 0; i < Processes; i++)
    {
        for (int j = 0; j < ResourceTypes; j++)
        {
            File >> Requests[i][j];
        }
    }

    PrintMatrix(Requests, Processes, ResourceTypes);
    cout << endl;

    File.close();

    DeadLockDetection(E, C, Requests, Processes, ResourceTypes);
}

int main()
{
    string FileName;
    cout << "Enter a file name: ";
    getline(cin, FileName);

    ProcessFile(FileName);
}
