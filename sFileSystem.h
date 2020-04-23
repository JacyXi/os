#ifndef SFILESYSTEM_H
#define SFILESYSTEM_H
#include <string>
#include <iostream>
#include "sPath.h"
#include "sFile.h"
#include <error.h>
#include <set>
#include "map.h"


using namespace std;

class sFileSystem
{
public:
    sFileSystem();
    sFileSystem(string user);
    int touch(string goalPath, string filename, string content);
    int touch(string filename, string content);
    int mkdir(string goalPath, string pathname);
    int mkdir(string pathname);
    int rm(string goalfile);
    int rm(string goalpath, string operants);
    int cat(string filename);
    int cp(string from, string to, string operants);
    int mv(string from, string to, string operants);
    int pwd();
    int cd(string goalpath);
    int ls();
    int chmod(string file, int mod);
    int find(string file);
    int revoke(string file);
    void ch_user(string user);
    int pwd(sPath * thislevel);

private:
    sPath * current_path;
    sPath ** allpath;
    string current_user;
    sPath * root;
    bool checkuser(string user);
    static set<string> alluser;
    int path_amount;
    static const int STORAGE = 100;

};

#endif // SFILESYSTEM_H
