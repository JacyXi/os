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
    bpt::bplus_tree database("storage.db",true);
}

bool sFileSystem::checkuser(string user) {
    return (alluser.find(user) != alluser.end()) ? true : false;
}

int sFileSystem::touch(string filename, string content){
    sFile nfile = sFile(current_user, 7, filename, content);
    current_path -> addFile(nfile);
    int key = hashfunc(filename);
    bpt::bplus_tree database("storage.db");
    sPath* target[10];
    database.search(key, &target);
    bool is_empty = true;
    for (int i = 0; i < 10; i++) {
        if (target[i] != nullptr) is_empty = true;
    }
    if (is_empty) {
        target[0] = current_path;
        database.insert(key, target);
    } else {
        bool has_inseart = false;
        for (int i = 0; i < 10; i++) {
            if (target[i] == nullptr) {
                target[i] = current_path;
                database.update(key, target);
                has_inseart = true;
                break;
            }
        }
        if (!has_inseart) error("The duplicated file are too much.");
    }
    return 2;
}

int sFileSystem::mkdir(string pathname) {
    if (current_path -> is_subset(pathname)) error("The path already exists.");
    current_path -> addPath(pathname.append(current_path -> get_absolute()));
    path_amount += 1;
    allpath[path_amount - 1] = new sPath(pathname.append(current_path->get_absolute()), current_path);
    return 2;
}
int sFileSystem::rm(string goalfile) {
    rm(goalfile, current_path);
    return 2;
}

void sFileSystem::rm(string goalfile,sPath* operationPath){
    operationPath -> removeFile(goalfile);
    bpt::bplus_tree database("storage.db");
    database.remove(hashfunc(goalfile));
}

int sFileSystem::rm(string goal, string operants){
    rm(goal,operants,current_path);
    return 2;
}

void sFileSystem::rm(string goal, string operants,sPath* operationPath) {
    if (operants.compare("-r") == 0) {
        if (operationPath -> get_name().compare(goal) == 0) error("Could not delect your current path.");
        if (operationPath -> is_subset(goal)) {
            operationPath -> removePath(goal);
            int location = get_location(goal);
            sPath * path_to_remove = allpath[location];
            for (string files : path_to_remove->get_files()) rm(files, path_to_remove);
            for (string subsets : path_to_remove->get_subsets()) rm(subsets, "-r",path_to_remove);
            allpath[location] -> ~sPath();
            allpath[location] = nullptr;
        } else {
            Set<string> parent_book;
            operationPath->get_all_parent(current_path->get_parent(), parent_book);
            if (parent_book.contains(goal)) {
                error("Could not delete super path.");
            } else {
                int location = get_location(goal);
                if (location > 1) {
                    sPath* path_to_remove = allpath[location];
                    sPath* its_parent = path_to_remove -> get_parent();
                    its_parent -> removePath(goal);
                    for (string files : path_to_remove->get_files()) rm(files, path_to_remove);
                    for (string subsets : path_to_remove->get_subsets()) rm(subsets, "-r", path_to_remove);
                    allpath[location] -> ~sPath();
                    allpath[location] = nullptr;
                }
            }
        }
    } else {
        rm(goal);
    }
}

int sFileSystem::cat(string filename) {
    return current_path -> read_file(filename);
}

int sFileSystem::cp(string from, string to, string operants) {
    //判断是否存在目标文件夹，判断是否是当前文件夹
    if (!current_path->get_name().compare(to)) error("Could not copy to the current directory.");
    int location = get_location(to);
    if (location < 0) error("Could not find the target directory.");
    sPath* operation_path = allpath[location];
    //复制文件夹
    if (!operants.compare("-r")) {
        if (current_path->is_subset(from)) {


        } else {
            error("No such directory exists.");
        }


    } else if (!operants.compare("-p")) {
        //复制文件
    } else {
        error("Wrong operants.");
    }




    return 0;
}

int sFileSystem::get_location(string pathname){
    int location = -1;
    for (int i = 0; i < path_amount; i++) {
        if (!allpath[i]->get_name().compare(pathname)) location = i; break;
    }
    return location;
}

int sFileSystem::mv(string from, string to, string operants){
    cp(from, to, operants);
    rm(from,operants);
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
int sFileSystem::cd(string goalpath){
    for (int i = 0; i< path_amount; i++) {
        if  (!allpath[i]->get_name().compare(goalpath)) {
            current_path = allpath[i];
            return 2;
        }
    }
    error("No such exist path.");
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
    sPath* target[10];
    if (database.search(hashfunc(file), &target) != 0){
        cout << "No such file."<<endl;
    } else {
        for (int i = 0; i < 10; i++) {
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

int sFileSystem::hashfunc(string filename){
    int length = filename.length();
    const char *y = filename.c_str();
    string code = to_string(length);
    for (int i = 0; i < length; i++) {
        code.append(to_string((int)y[i]));
    }
    code.append("0000000000000000");
    return atoi(code.substr(0,16).c_str());
}


set<string> sFileSystem::alluser = {"Jacy","Yanzhang","Xiaojie","Yuhao","Yuheng"};
