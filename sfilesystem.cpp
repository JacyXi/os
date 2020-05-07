#include "sFileSystem.h"
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <error.h>
#include "set.h"
#include "stack.h"
#include "foreach.h"
#include "bpt.h"
#include "vector.h"

sFileSystem::sFileSystem()
{
}



sFileSystem::sFileSystem(string user){
    if (checkuser(user)) current_user = user; else error ("No such user.");
    allpath = new sPath * [STORAGE];
    root = new sPath("root", true);
    allpath[0] = root;
    current_path = root;
    path_amount = 1;
    bpt::bplus_tree database("storage.db", true);
}



bool sFileSystem::checkuser(string user) {
    return (alluser.find(user) != alluser.end()) ? true : false;
}







int sFileSystem::touch(string filename, string content,int mod, sPath* operation_path) {
    int key = hashfunc(filename,false);
    bpt::bplus_tree database("storage.db");
    vector<sPath*> target;
    database.search(key, &target);
    bool is_empty = true;
    for (int i = 0; i < TREESIZE; i++) {
        if (target[i] != nullptr) is_empty = true;
    }
    if (is_empty) {
        target[0] = operation_path;
        database.insert(key, target);
        sFile nfile = sFile(current_user, mod, filename, content);
        operation_path -> addFile(nfile);
    } else {
        bool has_insert = false;
        for (int i = 0; i < TREESIZE; i++) {
            if (target[i] == nullptr) {
                target[i] = operation_path;
                database.update(key, target);
                has_insert = true;
                sFile nfile = sFile(current_user, mod, filename, content);
                operation_path -> addFile(nfile);
                break;
            }
        }
        if (!has_insert) error("The duplicated files are too much. Please change the name.");
    }
    return 2;
}









int sFileSystem::mkdir(string pathname, sPath * operating_path) {
    if (operating_path -> is_subset(pathname)) error("The path already exists.");

    int key = hashfunc(pathname, true);
    string absolute_address = pathname.append("/").append(operating_path -> get_absolute());
    bpt::bplus_tree database("storage.db");
    vector<sPath*> target;
    database.search(key, &target);
    bool is_empty = true;
    for (int i = 0; i < TREESIZE; i++) {
        if (target[i] != nullptr) is_empty = true;
    }
    if (is_empty) {
        target[0] = operating_path;
        database.insert(key, target);
        operating_path -> addPath(absolute_address);
        path_amount += 1;
        allpath[path_amount - 1] = new sPath(absolute_address, operating_path);
    } else {
        bool has_insert = false;
        for (int i = 0; i < TREESIZE; i++) {
            if (target[i] == nullptr) {
                target[i] = operating_path;
                database.update(key, target);
                has_insert = true;
                operating_path -> addPath(absolute_address);
                path_amount += 1;
                allpath[path_amount - 1] = new sPath(absolute_address, operating_path);
                break;
            }
        }
        if (!has_insert) error("The duplicated file are too much.");
    }
    return 2;
}









int sFileSystem::rm(string goalfile) {
    rmFile(goalfile, current_path);
    return 2;
}








void sFileSystem::rmFile(string goalfile, sPath* operationPath) {
    if (operationPath -> has_file(goalfile)) {
        operationPath -> removeFile(goalfile);
        bpt::bplus_tree database("storage.db");
        vector<sPath*> target;
        database.search(hashfunc(goalfile,false), &target);
        for (int i = 0; i < TREESIZE; i++) {
            if (target[i] == operationPath) {
                target[i] = nullptr;
                database.update(hashfunc(goalfile,false),target);
            }
        }
    } else {
        error("No such file to delect.");
    }

}







int sFileSystem::rm(string goal, string operants){
    if (!operants.compare("-r")) {
        if (current_path -> get_name().compare(goal) == 0) {
            error("Could not delect your current path.");
        } else if (current_path->is_subset(goal)) {
            rmDir(goal, current_path);
        }
    } else {
        rmFile(goal, current_path);
    }
    return 2;
}








void sFileSystem::rmDir(string goal, sPath* operationPath) {
    if (!goal.compare("*")) {
        for (string files : current_path->get_files()) rmFile(files, current_path);
        for (string subsets : current_path->get_subsets()) rmDir(subsets, current_path);

    } else if (operationPath -> is_subset(goal)) {
        operationPath -> removePath(goal);
        string absolute_goal = goal.append("/").append(operationPath->get_absolute());
        int location = get_location(absolute_goal);
        sPath * path_to_remove = allpath[location];
        for (string files : path_to_remove->get_files()) rmFile(files, path_to_remove);
        for (string subsets : path_to_remove->get_subsets_absolute()) rmDir(subsets,path_to_remove);

        bpt::bplus_tree database("storage.db");
        vector<sPath*> target;
        database.search(hashfunc(goal,true),&target);
        for (int i = 0; i < TREESIZE; i++) {
            if (target[i] == allpath[location]) target[i] = nullptr;
        }
        database.update(hashfunc(goal,true),target);
        allpath[location] -> ~sPath();
        allpath[location] = nullptr;
    } else {
        error("Could not find the target directory.");
    }

}







int sFileSystem::cat(string filename) {
    return current_path -> read_file(filename);
}





void sFileSystem::cpDir(string name, sPath *currentPath, sPath *targetPath) {
    string absolute_name = name.append("/").append(targetPath->get_absolute());
    mkdir(absolute_name, targetPath);
    string origin_absolute = name.append("/").append(currentPath->get_absolute());
    sPath * origin_path = allpath[get_location(origin_absolute)];
    for (string file : origin_path->get_files()) cpFile(file, origin_path, allpath[get_location(absolute_name)]);
    for (string path : origin_path->get_subsets()) cpDir(path, origin_path, allpath[get_location(absolute_name)]);
}

void sFileSystem::cpFile(string name, sPath *currentPath, sPath *targetPath) {
    sFile* origin_file = currentPath -> get_file(name);
    touch(name, origin_file -> get_content(),origin_file -> get_mod(current_user), targetPath);

}



int sFileSystem::cp(string from, string to, string operants) {
    if (operants.compare("-r")) {
        if (current_path->is_subset(from) & (get_location(to) >= 0)){
            cpDir(from, current_path, allpath[get_location(to)]);
        }

    } else if (operants.compare("-p")) {
        if (current_path -> has_file(from) & (get_location(to) >= 0)) {
            cpFile(from, current_path, allpath[get_location(to)]);
        }
    } else {
        error("Wrong operants.");
    }

    return 2;
}







int sFileSystem::get_location(string pathname){
    int location = -1;
    for (int i = 0; i < path_amount; i++) {
        if (!allpath[i]->get_absolute().compare(pathname)) location = i; break;
    }
    return location;
}










int sFileSystem::mv(string from, string to, string operants){
    cp(from, to, operants);
    rm(from, operants);
    return 2;
}











int sFileSystem::pwd(){
    return pwd(current_path);
}











int  sFileSystem::pwd(sPath * thislevel){
    Stack<string> parents_book;
    thislevel -> get_pwd(thislevel, parents_book);
    string output;
    for (string i; !parents_book.isEmpty(); i = parents_book.pop()) {
        output.append(i);
        output.append("/");
    }
    cout << output << endl;
    return 2;
}








int sFileSystem::cd(string goalpath, sPath* operating_path){
    string absolute_address = goalpath.append("/").append(operating_path->get_absolute());
    int i = get_location(absolute_address);
    if (i >= 0) {
        current_path = allpath[i];
        return 2;
    } else {
        error("No such exist path.");
    }
}

int sFileSystem::cd(string goalpath){
    return cd(goalpath, current_path);
}











int sFileSystem::ls(){
    Set<string> subsets = current_path->get_subsets();
    cout << "subfolders:" << endl;
    cout << "------------" << endl;
    for (string p : subsets) {
        cout << p << endl;
    }
    Set<string> files = current_path->get_files();
    cout << "files:"<< endl;
    cout << "------------" << endl;
    for (string f : files) {
        cout << f << endl;
    }
    cout << "------------" << endl;
    return 2;
}










int sFileSystem::chmod(string file, int mod) {
    current_path -> chmod(current_user, file, mod);
    return 2;
}










int sFileSystem::find(string file) {
    bpt::bplus_tree database("storage.db");
    vector<sPath*> target;
    if (database.search(hashfunc(file, false), &target) != 0){
        cout << "No such file."<<endl;
    } else {
        for (int i = 0; i < TREESIZE; i++) {
            if (target[i]!=nullptr) pwd(target[i]);
        }
    }
    return 2;
}











int sFileSystem::revoke(string file) {
    if (current_path->has_file(file)) {
        sFile * thisfile = current_path->get_file(file);
        thisfile->revoke();
        return 2;
    } else {
        error("No such file exists.");
    }
}










int sFileSystem::hashfunc(string filename, bool is_path){
    if (!is_path){
        int length = filename.length();
        const char *y = filename.c_str();
        string code = to_string(length);
        for (int i = 0; i < length; i++) {
            code.append(to_string((int)y[i]));
        }
        code.append("0000000000000000");
        return atoi(code.substr(0,16).c_str());
    } else {
        int length = filename.length();
        const char *y = filename.c_str();
        string code = "9";
        code.append(to_string(length));
        for (int i = 0; i < length; i++) {
            code.append(to_string((int)y[i]));
        }
        code.append("000000000000000");
        return atoi(code.substr(0,16).c_str());
    }
}








set<string> sFileSystem::alluser = {"Jacy","Yanzhang","Xiaojie","Yuhao","Yuheng"};
