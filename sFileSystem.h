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
    /* Default constructor for filesystem.*/
    sFileSystem();

    /* Constructor with specified user.*/
    sFileSystem(string user);

    /*
     * Method: touch
     * Usage: touch(filename, content, mod);
     * -----------------------------------------------------
     * Create a file. Assign filename, content, mode, from the operation path.
     */
    int touch(string filename, string content, int mod);

    /*
     * Method: mkdir
     * Usage: mkdir(pathname);
     * -----------------------
     * Create a path at the current directory.
     */
    int mkdir(string goalPath);

    /*
     * Method: rm
     * Usage: rm(goalfile);
     * ----------------------------------
     * A helper function to remove a file.
     */
    int rm(string goalfile);

    /*
     * Method: rm
     * Usage: rm(goalfile, operants);
     * ----------------------------------
     * A helper function to remove a file.
     */
    int rm(string goalpath, string operants);

    /*
     * Method: cat
     * Usage: cat(filename);
     * ----------------------------
     * Show the content of the file.
     */
    string cat(string filename);

    /*
     * Method: file_info
     * Usage: file_info(filename);
     * ---------------------------
     * Get the file information if file exists.
     */
    string file_info(string filename);

    /*
     * Method: cp
     * Usage: cp(from, to, operants);
     * --------------------------------------------
     * Copy a file or directory to another location.
     */
    int cp(string from, string to, string operants);

    /*
     * Method: mv
     * Usage: mv(from, to, operants);
     * ------------------------------
     * Move a file or path to another place.
     */
    int mv(string from, string to, string operants);

    /*
     * Method: pwd
     * Usage: pwd();
     * ------------------------------
     * Get current directory absolute address.
     */
    string pwd();

    /*
     * Method: cd
     * Usage: cd(goalpath);
     * -------------------------------------
     * Change current path to target path.
     */
    int cd(string goalpath);

    /*
     * Method: ls
     * Usage: ls();
     * -------------------------------------
     * List all files and paths in the current path.
     */
    string ls();

    /*
     * Method: chmod
     * Usage: chmod();
     * ----------------------------
     * Change the mode for the file.
     */
    int chmod(string file, int mod);

    /*
     * Method: find
     * Usage: find(file);
     * -----------------------------------
     * Find the file's absolute address. There maybe some duplicated name files.
     * Also duplicated name paths together.
     */
    string find(string file);
    /*
     * Method: revoke
     * Usage: revoke(file);
     * -----------------------------------
     * Revoke the file to the previous version.
     */
    int revoke(string file);

    /*
     * Method: ch_user
     * Usage: ch_user(user);
     * ----------------------
     * Change current user if the user is valid.
     */
    void ch_user(string user);

    /*
     * Method: changeContent
     * Usage: changeContent(filename, content);
     * ----------------------------------------
     * Change file content if file exists.
     */
    void changeContent(string filename, string content);

    /*
     * Method: has_path
     * Usage: has_path(filename);
     * --------------------------
     * Check whether the path exists.
     */
    bool has_path(string pathname);

private:
    /*
     * Method: touch
     * Usage: touch(filename, content, mod, operation_path);
     * -----------------------------------------------------
     * Helper function to create a file. Assign filename, content, mode, from the operation path.
     */
    int touch(string filename, string content, int mod, sPath* operating_path);

    /*
     * Method: mkdir
     * Usage: mkdir(pathname, operation_path);
     * -----------------------------------------------------
     * Create a path. Assign pathname from the operation path.
     */
    int mkdir(string pathname, sPath * operating_path);

    /*
     * Method: rmFile
     * Usage: rmFile(goalfile, operationPath);
     * ----------------------------------
     * A helper function to remove a file.
     */
    void rmFile(string goalfile,sPath* operationPath);

    /*
     * Method: rmDir
     * Usage: rmDir(goal, operationPath);
     * ----------------------------------
     * A helper function to remove a path.
     */
    void rmDir(string goalpath, sPath* operationPath);

    /*
     * Method: pwd
     * Usage: pwd(thislevel);
     * ------------------------------
     * Get target directory absolute address.
     */
    string pwd(sPath * thislevel);

    /*
     * Method: cpFile
     * Usage: cpFile(name, currentPath, targetPath);
     * --------------------------------------------
     * A helper function to copy a file.
     */
    void cpFile(string name, sPath* currentPath,sPath* targetPath);

    /*
     * Method: cpDir
     * Usage: cpDir(name, currentPath, targetPath);
     * --------------------------------------------
     * A helper function to copy a path.
     */
    void cpDir(string name, sPath* currentPath, sPath* targetPath);

    /*
     * Method: cd
     * Usage: cd(goalpath, operating_path);
     * -------------------------------------
     * A helper function to change current path.
     */
    int cd(string goal_path, sPath * operating_path);

    /*
     * Method: hashfunc
     * Usage: hashfunc(filename, is_path);
     * -----------------------------------
     * Hash function to convert file or path into a 16-digits hashcode.
     */
    string hashfunc(string filename, bool is_path);

    /*
     * Method: get_location
     * Usage: get_location(pathname);
     * ------------------------------
     * Return the path location in a pathgory.
     */
    int get_location(string pathname);

    /*
     * Method: checkuser
     * Usage: checkuser(user);
     * ----------------------
     * Check if the user is valid.
     */
    bool checkuser(string user);

    /*sPath current_path*/
    sPath * current_path;
    /*Vector current_path*/
    Vector<sPath*> allpath;
    /*Set alluser*/
    static Set<string> alluser;
    /*string current_user*/
    string current_user;
    /*sPath root*/
    sPath * root;
    /*CBPlusTree tree*/
    CBPlusTree tree;
    /*int path_amount*/
    int path_amount;
    /*int STORAGE*/
    static const int STORAGE = 100;
    /*int TREESIZE*/
    static const int TREESIZE = 10;
};

#endif // SFILESYSTEM_H
