/*
 * File: sFileSystem.cpp
 * ---------------
 * This file implements the sFileSystem class.
 */
#include "sFileSystem.h"
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <error.h>
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "BPlus_tree.h"


/*
 * Constructor: sFileSystem
 * Usage: sFileSystem system;
 * ---------------------------------
 * Initialize a new empty sFileSystem.
 */
sFileSystem::sFileSystem() {
}


/*
 * Constructor: sFileSystem
 * Usage: sFileSystem system = sFileSystem(user);
 * ----------------------------------------------
 * Initialize a new empty sFileSystem, set a user.
 */
sFileSystem::sFileSystem(string user) {
    if (checkuser(user)) current_user = user; else error ("No such user.");
    root = new sPath("root", true);
    allpath.add(root);
    current_path = root;
    path_amount = 1;
}


/*
 * Method: checkuser
 * Usage: checkuser(user);
 * ----------------------
 * Check if the user is valid.
 */
bool sFileSystem::checkuser(string user) {
    return alluser.contains(user);
}

/*
 * Method: ch_user
 * Usage: ch_user(user);
 * ----------------------
 * Change current user if the user is valid.
 */
void sFileSystem::ch_user(string user) {
    if (checkuser(user)) {
        current_user = user;
    } else {
        error("No such user.");
    }
}

/*
 * Method: touch
 * Usage: touch(filename, content, mod);
 * -----------------------------------------------------
 * Create a file. Assign filename, content, mode, from the operation path.
 */
int sFileSystem::touch(string filename, string content, int mod){
    return touch(filename, content, mod, current_path);
};

/*
 * Method: touch
 * Usage: touch(filename, content, mod, operation_path);
 * -----------------------------------------------------
 * Helper function to create a file. Assign filename, content, mode, from the operation path.
 */
int sFileSystem::touch(string filename, string content,int mod, sPath * operation_path) {
    string key = hashfunc(filename, false);
    if (tree.search(key)) {
        Set<sPath*> target= tree.select(key,EQ).front();
        target.add(operation_path);
        tree.insert(key, target);
        sFile* nfile = new sFile(current_user, mod, filename, content, pwd(operation_path));
        operation_path -> addFile(nfile);
    } else {
        Set<sPath*> target;
        target.add(operation_path);
        tree.insert(key, target);
        sFile* nfile = new sFile(current_user, mod, filename, content, pwd(operation_path));
        operation_path -> addFile(nfile);
    }
    return 2;
}

/*
 * Method: file_info
 * Usage: file_info(filename);
 * ---------------------------
 * Get the file information if file exists.
 */
string sFileSystem::file_info(string filename) {
    if (current_path -> has_file(filename)) {
        sFile* thisfile = current_path->get_file(filename);
        string info = thisfile->get_info(current_user);
        return info;
    } else {
        error("No such file to delect.");
    }
}

/*
 * Method: mkdir
 * Usage: mkdir(pathname, operation_path);
 * -----------------------------------------------------
 * Create a path. Assign pathname from the operation path.
 */
int sFileSystem::mkdir(string pathname, sPath * operating_path) {
    if (operating_path -> is_subset(pathname)) error("The path already exists.");
    string key = hashfunc(pathname, true);
    string absolute_address = pathname.append("/").append(operating_path -> get_absolute());
    Set<sPath*> target;
    if (!tree.search(key)) {
        target.insert(operating_path);
        tree.insert(key, target);
        operating_path -> addPath(absolute_address);
        path_amount += 1;
        allpath.add(new sPath(absolute_address, operating_path));
    } else {
        Set<sPath*> target = tree.select(key,EQ).front();
        target.add(operating_path);
        tree.insert(key, target);
        operating_path -> addPath(absolute_address);
        path_amount += 1;
        allpath.add(new sPath(absolute_address, operating_path));
    }
    return 2;
}

/*
 * Method: mkdir
 * Usage: mkdir(pathname);
 * -----------------------
 * Create a path at the current directory.
 */
int sFileSystem::mkdir(string pathname) {
    return mkdir(pathname, current_path);
}

/*
 * Method: rm
 * Usage: rm(goalfile);
 * ----------------------------------
 * A helper function to remove a file.
 */
int sFileSystem::rm(string goalfile) {
    rmFile(goalfile, current_path);
    return 2;
}

/*
 * Method: rmFile
 * Usage: rmFile(goalfile, operationPath);
 * ----------------------------------
 * A helper function to remove a file.
 */
void sFileSystem::rmFile(string goalfile, sPath* operationPath) {
    if (operationPath -> has_file(goalfile)) {
        operationPath -> removeFile(goalfile);
        Set<sPath*> target;
        string key = hashfunc(goalfile, false);
        if(tree.search(key)) {
            target = tree.select(key, EQ).front();
            target.remove(operationPath);
            tree.insert(key,target);
        }
    } else {
        error("No such file to delect.");
    }

}


/*
 * Method: rm
 * Usage: rm(goalfile, operants);
 * ----------------------------------
 * A helper function to remove a file.
 */
int sFileSystem::rm(string goal, string operants){
    if (!operants.compare("-r")) {
        if (current_path -> get_name().compare(goal) == 0) {
            error("Could not delect your current path.");
        } else if (current_path -> is_subset(goal)) {
            rmDir(goal, current_path);
        }
    } else {
        rmFile(goal, current_path);
    }
    return 2;
}

/*
 * Method: rmDir
 * Usage: rmDir(goal, operationPath);
 * ----------------------------------
 * A helper function to remove a path.
 */
void sFileSystem::rmDir(string goal, sPath* operationPath) {
    if (!goal.compare("*")) {
        for (string files : current_path->get_files()) rmFile(files, current_path);
        for (string subsets : current_path->get_subsets()) rmDir(subsets, current_path);

    } else if (operationPath -> is_subset(goal)) {
        operationPath -> removePath(goal);
        string absolute_goal_raw = goal;
        string absolute_goal = absolute_goal_raw.append("/").append(operationPath->get_absolute());
        int location = get_location(absolute_goal);
        sPath * path_to_remove = allpath[location];
        for (string files : path_to_remove->get_files()) rmFile(files, path_to_remove);
        for (string subsets : path_to_remove->get_subsets_absolute()) rmDir(subsets,path_to_remove);

        Set<sPath*> target;
        target = tree.select(hashfunc(goal,true), EQ).front();

        if (target.contains(allpath[location])) {
            target.remove(allpath[location]);
        } else {
            error("Could not remove to such path.");
        }
        tree.insert(hashfunc(goal,true), target);
        allpath[location] -> ~sPath();
        allpath.remove(location);
    } else {
        error("Could not find the target directory.");
    }

}

/*
 * Method: cat
 * Usage: cat(filename);
 * ----------------------------
 * Show the content of the file.
 */
string sFileSystem::cat(string filename) {
    return current_path -> read_file(filename);
}

/*
 * Method: cpDir
 * Usage: cpDir(name, currentPath, targetPath);
 * --------------------------------------------
 * A helper function to copy a path.
 */
void sFileSystem::cpDir(string name, sPath *currentPath, sPath *targetPath) {
    string raw1 = name;
    string raw2 = name;
    string absolute_name = raw1.append("/").append(targetPath -> get_absolute());
    mkdir(absolute_name, targetPath);
    string origin_absolute = raw2.append("/").append(currentPath -> get_absolute());
    sPath * origin_path = allpath[get_location(origin_absolute)];
    for (string file : origin_path -> get_files()) cpFile(file, origin_path, allpath[get_location(absolute_name)]);
    for (string path : origin_path -> get_subsets()) cpDir(path, origin_path, allpath[get_location(absolute_name)]);
}

/*
 * Method: cpFile
 * Usage: cpFile(name, currentPath, targetPath);
 * --------------------------------------------
 * A helper function to copy a file.
 */
void sFileSystem::cpFile(string name, sPath *currentPath, sPath *targetPath) {
    sFile* origin_file = currentPath -> get_file(name);
    touch(name, origin_file -> get_content(), origin_file -> get_mod(current_user), targetPath);

}


/*
 * Method: cp
 * Usage: cp(from, to, operants);
 * --------------------------------------------
 * Copy a file or directory to another location.
 */
int sFileSystem::cp(string from, string to, string operants) {
    if (!operants.compare("-r")) {
        if (current_path -> is_subset(from) & (get_location(to) >= 0)){
            cpDir(from, current_path, allpath[get_location(to)]);
        }

    } else if (!operants.compare("-p")) {
        if (current_path -> has_file(from) & (get_location(to) >= 0)) {
            cpFile(from, current_path, allpath[get_location(to)]);
        }
    } else {
        error("Wrong operants.");
    }

    return 2;
}

/*
 * Method: get_location
 * Usage: get_location(pathname);
 * ------------------------------
 * Return the path location in a pathgory.
 */
int sFileSystem::get_location(string pathname) {
    int location = -1;
    for (int i = 0; i < allpath.size(); i++) {
        if ((allpath[i] -> get_absolute().compare(pathname)) == 0) {
            location = i;
            break;
        }
    }
    return location;
}

/*
 * Method: mv
 * Usage: mv(from, to, operants);
 * ------------------------------
 * Move a file or path to another place.
 */
int sFileSystem::mv(string from, string to, string operants){
    cp(from, to, operants);
    rm(from, operants);
    return 2;
}


/*
 * Method: pwd
 * Usage: pwd();
 * ------------------------------
 * Get current directory absolute address.
 */
string sFileSystem::pwd() {
    return pwd(current_path);
}



/*
 * Method: pwd
 * Usage: pwd(thislevel);
 * ------------------------------
 * Get target directory absolute address.
 */
string sFileSystem::pwd(sPath * thislevel){
    Stack<string> parents_book;
    thislevel -> get_pwd(thislevel, parents_book);
    string output = "/";
    while (!parents_book.isEmpty()) {
        output.append(parents_book.pop());
        output.append("/");
    }
    return output;
}


/*
 * Method: cd
 * Usage: cd(goalpath, operating_path);
 * -------------------------------------
 * A helper function to change current path.
 */
int sFileSystem::cd(string goalpath, sPath* operating_path){
    if (goalpath.find("/") != goalpath.npos) {
        int i = get_location(goalpath);
        if (i >= 0) {
            current_path = allpath[i];
            return 2;
        } else {
            error("No such exist path.");
        }
    }
    if (operating_path->is_root()) {
        string absolute_address = goalpath.append("/").append(operating_path->get_absolute());
        int i = get_location(absolute_address);
        if (i >= 0) {
            current_path = allpath[i];
            return 2;
        } else {
            error("No such exist path.");
        }
    } else if (!goalpath.compare(operating_path->get_parent()->get_name())) {
        current_path = operating_path->get_parent();
        return 2;
    } else {
        string absolute_address = goalpath.append("/").append(operating_path->get_absolute());
        int i = get_location(absolute_address);
        if (i >= 0) {
            current_path = allpath[i];
            return 2;
        } else {
            error("No such exist path.");
        }
    }
}

/*
 * Method: cd
 * Usage: cd(goalpath);
 * -------------------------------------
 * Change current path to target path.
 */
int sFileSystem::cd(string goalpath) {
    return cd(goalpath, current_path);
}


/*
 * Method: ls
 * Usage: ls();
 * -------------------------------------
 * List all files and paths in the current path.
 */
string sFileSystem::ls() {
    Set<string> subsets = current_path->get_subsets();
    string result =  "subfolders:\n------------\n";
    for (string p : subsets) {
        result.append(p);
        result.append("\n");
    }
    result.append("------------\n");
    Set<string> files = current_path->get_files();
    result.append("files:\n------------\n");
    for (string f : files) {
        result.append(f);
        result.append("\n");
    }
    return result;
}


/*
 * Method: chmod
 * Usage: chmod();
 * ----------------------------
 * Change the mode for the file.
 */
int sFileSystem::chmod(string file, int mod) {
    current_path -> chmod(current_user, file, mod);
    return 2;
}

/*
 * Method: find
 * Usage: find(file);
 * -----------------------------------
 * Find the file's absolute address. There maybe some duplicated name files.
 * Also duplicated name paths together.
 */
string sFileSystem::find(string file) {
    string result;
    Set<sPath*> target;
    if (!tree.search(hashfunc(file, false))){
        return "No such file.";
    } else {
        target = tree.select(hashfunc(file, false),EQ).front();
        try {
            result = "Try to find the file with name ";
            result.append(file);
            result.append(" by using B+ tree search.\n");
            for (sPath * path : target) {
                result.append(pwd(path));
                result.append("\n");
            }
        } catch (ErrorException) {
            return "Failed in find";
        }
    }
    return result;
}

/*
 * Method: revoke
 * Usage: revoke(file);
 * -----------------------------------
 * Revoke the file to the previous version.
 */
int sFileSystem::revoke(string file) {
    if (current_path->has_file(file)) {
        sFile * thisfile = current_path->get_file(file);
        thisfile->revoke();
        return 2;
    } else {
        error("No such file exists.");
    }
}

/*
 * Method: hashfunc
 * Usage: hashfunc(filename, is_path);
 * -----------------------------------
 * Hash function to convert file or path into a 16-digits hashcode.
 */
string sFileSystem::hashfunc(string filename, bool is_path){
    if (!is_path){
        int length = filename.length();
        const char *y = filename.c_str();
        string code = to_string(length);
        for (int i = 0; i < length; i++) {
            code.append(to_string((int)y[i]));
        }
        code.append("0000000000000000");
        return code.substr(0,16);
    } else {
        int length = filename.length();
        const char *y = filename.c_str();
        string code = "9";
        code.append(to_string(length));
        for (int i = 0; i < length; i++) {
            code.append(to_string((int)y[i]));
        }
        code.append("000000000000000");
        return code.substr(0,16);
    }
}

/*
 * Method: changeContent
 * Usage: changeContent(filename, content);
 * ----------------------------------------
 * Change file content if file exists.
 */
void sFileSystem::changeContent(string filename, string content){
    current_path -> chcontent(current_user, filename, content);
}

/*
 * Method: has_path
 * Usage: has_path(filename);
 * --------------------------
 * Check whether the path exists.
 */
bool sFileSystem::has_path(string pathname) {
    return current_path->is_subset(pathname);
}

Set<string> sFileSystem::alluser = {"Jacy", "Eric", "Blaine", "Yuheng"};


