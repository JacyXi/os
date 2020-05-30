#ifndef SFILESYSTEM_H
#define SFILESYSTEM_H
#include <string>
#include <iostream>
#include "sPath.h"
#include "sFile.h"
#include <error.h>
#include <set>
#include "map.h"
#include "BPlus_tree.h"


using namespace std;

class sFileSystem
{
public:
    sFileSystem();
    sFileSystem(string user);
    int touch(string filename, string content, int mod);
    int mkdir(string goalPath);
    int rm(string goalfile);
    int rm(string goalpath, string operants);
    string cat(string filename);
    string file_info(string filename);
    int cp(string from, string to, string operants);
    int mv(string from, string to, string operants);
    string pwd();
    int cd(string goalpath);
    string ls();
    int chmod(string file, int mod);
    string find(string file);
    int revoke(string file);
    void ch_user(string user);
    void changeContent(string filename, string content);
    bool has_path(string pathname);

private:
    int touch(string filename, string content, int mod, sPath* operating_path);
    int mkdir(string pathname, sPath * operating_path);
    void rmFile(string goalfile,sPath* operationPath);
    void rmDir(string goalpath, sPath* operationPath);
    string pwd(sPath * thislevel);
    void cpFile(string name, sPath* currentPath,sPath* targetPath);
    void cpDir(string name, sPath* currentPath, sPath* targetPath);
    int cd(string goal_path, sPath * operating_path);

    sPath * current_path;
    Vector<sPath*> allpath;
    string current_user;
    sPath * root;
    CBPlusTree tree;
    bool checkuser(string user);
    static Set<string> alluser;
    int path_amount;
    static const int STORAGE = 100;
    static const int TREESIZE = 10;
    string hashfunc(string filename, bool is_path);
    int get_location(string pathname);
    void copyDirHelper(sPath* current, sPath* operation_path, string operants);


};

#endif // SFILESYSTEM_H
