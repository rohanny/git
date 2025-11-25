#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include "./utils/sha1.h"
#include "utils/zstr.hpp"

using namespace std;

int git_init();
int git_hash_object(char* path);
int git_commit();
int git_cat_file(char* flag, char* sha);

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
    else if (command == "hash-object")
    {
        //this is not an actual command (or even if it is, it doesn't do that)
        //this thing only stores the files as git objects in the .git/objects folder
        if (command == "hash-object") {
            for (int i = 2; i < argc; i++) {
                git_hash_object(argv[i]);
            }
        }
    }
    else if (command == "cat-file")
    {
        return git_cat_file(argv[2], argv[3]);
    }
    else
    {
        cout << "Unknown command: " << command << endl;
        return EXIT_FAILURE;
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

int git_hash_object(char* path)
{
    if (!filesystem::exists(path))
    {
        cerr << "Error: File " << path << " does not exist." << endl;
        return EXIT_FAILURE;
    }

    ifstream f(path, ios::binary);
    vector<uint8_t> content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());

    string header = "blob" + to_string(content.size()) + '\0';
    vector<uint8_t> blob;
    blob.insert(blob.end(), header.begin(), header.end());
    blob.insert(blob.end(), content.begin(), content.end());

    SHA1_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, blob.data(), blob.size());

    uint8_t digest[SHA1_DIGEST_SIZE];
    SHA1_Final(&ctx, digest);

    string sha1hex; //this will have the 40char hex representation of the sha1 digest
    for (int i = 0; i < SHA1_DIGEST_SIZE; i++)
    {
        char buf[3];
        sprintf(buf, "%02x", digest[i]);
        sha1hex += buf;
    }

    string dir = "./.git/objects/" + sha1hex.substr(0, 2);
    string file = dir + "/" + sha1hex.substr(2);
    filesystem::create_directories(dir);

    zstr::ofstream out(file);
    out.write(reinterpret_cast<const char*>(blob.data()), blob.size());
    out.close();

    cout << "Added file " << path << " as blob " << sha1hex << endl;
    return EXIT_SUCCESS;
}

int git_commit()
{
    // Placeholder for future implementation
    return EXIT_SUCCESS;
}

int git_cat_file(char* flag, char* sha)
{
    if (flag != string("-p"))
    {
        cerr << "Error: Unsupported flag " << flag << endl;
        return EXIT_FAILURE;
    }
    string dir = "./.git/objects/" + string(sha).substr(0, 2);
    string obj = dir + "/" + string(sha).substr(2);
    if (!filesystem::exists(obj))
    {
        cerr << "Error: Object " << sha << " does not exist." << endl;
        return EXIT_FAILURE;
    }

    zstr::ifstream file(obj);
    std::string decompressed_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    size_t null_pos = decompressed_content.find('\0');
    if (null_pos == string::npos)
    {
        cerr << "Error: Invalid object format." << endl;
        return EXIT_FAILURE;
    }
    string content = decompressed_content.substr(null_pos + 1);
    cout << content << endl;

    return EXIT_SUCCESS;
}