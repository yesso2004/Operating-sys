#include <iostream>
#include <dirent.h>
#include <string>
using namespace std;

void ListFiles(string Directory)
{
    DIR *dir = opendir(Directory.c_str());

    if (!dir)
    {
        cout << "Failed to access Directory" << endl;
        return;
    }

    struct dirent *Input;
    cout << "Directory: " << Directory << endl;

    while ((Input = readdir(dir)) != nullptr)
    {
        if (string(Input->d_name) == "." || string(Input->d_name) == "..")
            continue;

        cout << Input->d_name << endl;
    }

    cout << endl;
    closedir(dir);
}

int main(int argc, char *argv[])
{

    for (int i = 0; i < argc; i++)
    {
        ListFiles(argv[i]);
    }
}
