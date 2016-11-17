#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>
using namespace std;

bool verbose = false;

string RemoveQuotes( std::string str )
{
    size_t n;
    while ( ( n = str.find('\"') ) != std::string::npos || ( n = str.find('\<') ) != std::string::npos || ( n = str.find('\>') ) != std::string::npos ) str.erase(n,1);

    return str;
}

string ret_newfile(string filestr)
{
    fstream file;
    file.open(RemoveQuotes(filestr).c_str());

    string str;

    while(!file.eof())
    {
        string tmpString;
        getline(file, tmpString);
        if(verbose)
            cout << tmpString << endl;
        str.insert(str.size(), (tmpString + "\n").c_str());
    }

    return str;
}

int main(int argc, char* argv[])
{
    string filename = string(argv[1]);

    cout << filename << endl;

    for(int i = 2 ; i < argc ; ++i)
    {
        if(string(argv[i]) == "-v")
            verbose = true;
        else
        cout << "Unknown command" << endl;

    }

    fstream file;
    file.open((filename).c_str());

    assert(file.is_open() == true);

    string finalString;

    cout << "Commencing file parsing..." << endl;

    if(verbose)
        cout << "===========================================" << endl;

    while(!file.eof())
    {
        string str_file;

        getline(file, str_file);

        size_t found = str_file.find("#include");

        if(found != string::npos)
        {
            cout << "External file found: " << str_file.substr(9) << endl;
            string tmpstring = str_file.substr(9);
            str_file = ret_newfile(tmpstring);
        }

        if(verbose)
            cout << str_file << endl;

        finalString.insert(finalString.size(), str_file + "\n");
    }

    if(verbose)
        cout << "===========================================" << endl;

    cout << "File parsing complete..." << endl;

    file.close();

    string newFilename = "proc_" + filename;

    ofstream outFile;

    cout << "Writing file to disk..." << endl;

    outFile.open(newFilename.c_str());
    outFile << finalString;

    outFile.close();

    cout << "File writing complete!" << endl;

    return 0;
}
