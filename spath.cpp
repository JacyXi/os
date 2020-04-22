#include "sPath.h"
#include "sFile.h"
#include "set.h"
#include <iostream>
using namespace std;

sPath::sPath()
{
    filegory = new sFile*[INIT_SIZE];
    capacity = INIT_SIZE;
}

sPath::~sPath(){
    delete[] filegory;
}

sPath::sPath(string name, sPath * previous_path) {
    parent = previous_path;
    filegory = new sFile*[INIT_SIZE];
    size = 0;
    capacity = INIT_SIZE;
    pathname = name;
    isRoot = false;
}
sPath::sPath(string name, bool is_root) {
    isRoot = is_root;
    filegory = new sFile * [INIT_SIZE];
    size = 0;
    capacity = INIT_SIZE;
    pathname = name;

}
bool sPath::is_root(){
    return isRoot;
}

void sPath::addFile(sFile & file) {
    size = size + 1;
    filegory[size - 1] = &file;
    files.add(file.get_name(),size);
    if (check_almost_full()) expandCapacity();
}

bool sPath::check_almost_full(){
    return (size) == capacity? true:false;
}

void sPath::expandCapacity(){
    capacity = 2 * capacity;
    sFile ** array = new sFile*[capacity];
    for (int i = 0; i < size; i++) {
        array[i] = filegory[i];
    }
    delete [] filegory;
    filegory = array;
}

std::string sPath::toString(){
    return pathname;
}
sPath * sPath::get_parent(){
    return parent;
}

Set<string> sPath::get_subsets(){
    return subsets;
}

void sPath::addPath(string sub_path_name){
    subsets.add(sub_path_name);
}

bool sPath::is_subset(string name){
    return subsets.contains(name);
}
int sPath::findLocation(string filename){
    for (int i=0;i<capacity;i++){
        if (filegory[i]->get_name().compare(filename) == 0) return i;
    }
    return -1;
}
void sPath::removeFile(string filename){
    int location = findLocation(filename);
    if (location == -1) error("No such file.");
    filegory[location]->~sFile();
    filegory[location] = nullptr;
}
void sPath::removePath(string path){
    subsets.remove(path);
}
string sPath::get_name(){
    return pathname;
}
void sPath::get_all_parent(sPath * thisLevel, Set<string> &parents_book){
    if (!thisLevel->is_root()) {
        parents_book.add(get_name());
        get_all_parent(thisLevel->get_parent(), parents_book);
    } else {
        parents_book.add("root");
    }
}
int sPath::read_file(string filename){
    int location = findLocation(filename);
    if (location != -1) {
        cout << filegory[location] -> get_content() << endl;
        return filegory[location]->get_size();
    } else {
        error("No such  file.");
    }
}
void sPath::get_pwd(sPath *thisLevel, Stack<string> & pwd_road) {
    if (!thisLevel->is_root()) {
        pwd_road.push(get_name());
        get_pwd(thisLevel->get_parent(), pwd_road);
    } else {
        pwd_road.push("root");
    }
}
