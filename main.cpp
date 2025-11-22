#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;

int git_init();

int main(int argc, char **argv)
{
    cout << unitbuf;
    cerr << unitbuf;

    if (argc < 2 )
    {
        cout << "Not enough arguments!" << endl;
        return EXIT_FAILURE;
    }
    string command = argv[1];
    if (command == "init")
    {
        return git_init();
    }
}

int git_init()
{
    try
    {
        filesystem::create_directory("./.git");
        filesystem::create_directory("./.git/objects");
        filesystem::create_directory("./.git/refs");
        ofstream head_file("./.git/HEAD");
        cout << "Initialized empty git repository." << endl;
        return EXIT_SUCCESS;
    }
    catch (exception &e)
    {
        cerr << "Error initializing repository: " << e.what() << endl;
        return EXIT_FAILURE;
    }

}