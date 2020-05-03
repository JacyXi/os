/*
 * File: sPath.h
 * ---------------
 * This interface defines the sPath class, which implements the
 * sPath abstraction for the path in the filesystem.
 */

#ifndef SPATH_H
#define SPATH_H

#include "sFile.h"
#include <string>
#include "set.h"
#include <map>
#include "stack.h"

/*
 * Class: sPath
 * -------------
 * This interface defines a class that models the path in a filesystem
 *
 */

class sPath
{
public:
    sPath();
    ~sPath();
    sPath(string name, sPath * previous_path);
    sPath(string name, bool is_root);
    void addFile(sFile & file);
    void addPath(string sub_path_name);
    void removePath(string path);
    void removeFile(string filename);
    sPath * get_parent();
    void expandCapacity();
    bool check_almost_full();
    std::string toString();
    Set<string> get_subsets();
    Set<string> get_subsets_absolute();
    Set<string> get_files();
    bool is_subset(string name);
    bool is_root();
    string get_name();
    void get_all_parent(sPath * thisLevel, Set<string> & parents_book);
    int read_file(string filename);
    void get_pwd(sPath * thisLevel, Stack<string> & pwd_road);
    bool has_file(string filename);
    sFile * get_file(string filename);
    void chmod(string user, string filename, int mod);
    string get_absolute();
private:
    int findLocation(string filename);
    sFile ** filegory;
    sPath * parent;
    int size;
    bool isRoot;
    int capacity;
    string pathname;
    const static int INIT_SIZE = 10;
    Set<string> subsets;
    Set<string> files;
    Set<string> subsets_absolute;
};

#endif
