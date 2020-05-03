#ifndef SFILESYSTEM_H
#define SFILESYSTEM_H
#include <string>
#include <iostream>
#include "sPath.h"
#include "sFile.h"
#include <error.h>
#include <set>
#include "map.h"
#include "bpt.h"


using namespace std;

class sFileSystem
{
public:
    sFileSystem();
    sFileSystem(string user);
    int touch(string goalPath, string filename, string content);
    int touch(string filename, string content, int mod, sPath* operating_path);
    int mkdir(string goalPath, string pathname);
    int mkdir(string pathname, sPath * operating_path);
    void rmFile(string goalfile,sPath* operationPath);
    void rmDir(string goalpath, sPath* operationPath);
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
    void cpFile(string name, sPath* currentPath,sPath* targetPath);
    void cpDir(string name, sPath* currentPath, sPath* targetPath);
    int cd(string goal_path, sPath * operating_path);


private:
    sPath * current_path;
    sPath ** allpath;
    string current_user;
    sPath * root;
    bool checkuser(string user);
    static set<string> alluser;
    int path_amount;
    static const int STORAGE = 100;
    static const int TREESIZE = 10;
    int hashfunc(string filename, bool is_path);
    int get_location(string pathname);
    void copyDirHelper(sPath* current, sPath* operation_path, string operants);


};

#endif // SFILESYSTEM_H
