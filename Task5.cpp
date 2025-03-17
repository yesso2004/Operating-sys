#include <iostream>
#include <map>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
using namespace std;

void FileSizes(const string &directory, map<int, int> &Histogram, int Binwidth)
{
    DIR *dir = opendir(directory.c_str());

    if (!dir)
    {
        cout << "Failed to access Directory" << endl;
        return;
    }

    struct dirent *input;
    struct stat FileSize;

    while ((input = readdir(dir)) != nullptr)
    {
        string FilePath = directory + "/" + input->d_name;

        if (string(input->d_name) == "." || string(input->d_name) == "..")
            continue;

        if (stat(FilePath.c_str(), &FileSize) == 0)
        {
            if (S_ISREG(FileSize.st_mode))
            {
                int BinIndex = FileSize.st_size / Binwidth;
                Histogram[BinIndex]++;
            }
            else if (S_ISDIR(FileSize.st_mode))
            {
                FileSizes(FilePath, Histogram, Binwidth);
            }
        }
    }
    closedir(dir);
}

void PrintHisto(map<int, int> Histogram, int BinWidth)
{
    cout << "BinWidth: " << BinWidth << " bytes" << endl;
    cout << "------------------------------------" << endl;

    for (const auto &bin : Histogram)
    {
        cout << "[" << bin.first * BinWidth << " - " << (bin.first + 1) * BinWidth - 1 << "]: ";
        cout << "(" << bin.second << " files)" << endl;
    }
}

int main()
{
    int BinWidth;
    string Directory;
    cout << "Enter a directory: ";
    getline(cin, Directory);

    cout << "Enter a binwidth (bytes): ";
    cin >> BinWidth;

    if (BinWidth <= 0)
    {
        cout << "BinWidth cannot be less than 1" << endl;
        return 1;
    }

    map<int, int> Histogram;

    FileSizes(Directory, Histogram, BinWidth);
    PrintHisto(Histogram, BinWidth);
}