/*
 * File: spath.cpp
 * ---------------
 * This file implements the sPath class.
 */

#include "sPath.h"
#include "sFile.h"
#include "set.h"
#include <iostream>
using namespace std;

/*
 * Constructor: sPath
 * Usage: sPath cstk;
 * -------------------
 * Initializes a new empty sPath
 */
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

/*
 * Method: is_root
 * Usage: cstk.is_root();
 * ----------------------
 * Returns true if this path is the root path (has no parent)
 */
bool sPath::is_root(){
    return isRoot;
}

/*
 * Method: addFile
 * Usage: cstk.addFile(& file);
 * -----------------------------
 * Add the address of the new file to the end of the array (filegory)
 * Add the new file into the map (files), the key is the file name, and the value is its size (i.e. "50kb")
 */
void sPath::addFile(sFile & file) {
    size = size + 1;
    filegory[size - 1] = &file;
    files.add(file.get_name());
    if (check_almost_full()) expandCapacity();
}

/*
 * Method: check_almost_full
 * Usage: cstk.check_almost_full();
 * ----------------------------------
 * Returns true if the size is equal to the capacity, and false otherwise
 */
bool sPath::check_almost_full(){
    return (size) == capacity? true:false;
}

/*
 * Method: expandCapacity
 * Usage: cstk.expandCapacity();
 * -------------------------------
 * Doubles the size of the sPath
 */
void sPath::expandCapacity(){
    capacity = 2 * capacity;
    sFile ** array = new sFile*[capacity];
    for (int i = 0; i < size; i++) {
        array[i] = filegory[i];
    }
    delete [] filegory;
    filegory = array;
}

/*
 * Change the data type sPath to string
 */
std::string sPath::toString(){
    return pathname;
}

/*
 * Method: get_parent
 * Usage: cstk.get_parent();
 * ---------------------------
 * Returns the parent this file
 */
sPath * sPath::get_parent(){
    return parent;
}

/*
 * Method: get_subsets
 * Usage: cstk.get_subsets();
 * ---------------------------
 * Returns the subsets this file
 */
Set<string> sPath::get_subsets(){
    return subsets;
}

Set<string> sPath::get_files(){
    return files;
}

/*
 * Method: addPath
 * Usage: cstk.addPath(sub_path_name);
 * -----------------------------------
 * Add the sub_path_name into the set(subsets)
 */
void sPath::addPath(string sub_path_name){
    subsets.add(sub_path_name);
}

/*
 * Method: is_subset
 * Usage: cstk.is_subset(name);
 * ----------------------------
 * Returns true if this path is in the subset of another sPath (name)
 */
bool sPath::is_subset(string name){
    return subsets.contains(name);
}

/*
 * Method: findLocation
 * Usage: cstk.findLocation(filename);
 * -----------------------------------
 * return the location is the array(filegory) if the filename is in the array; else return -1
 */
int sPath::findLocation(string filename){
    for (int i=0;i<capacity;i++){
        if (filegory[i]->get_name().compare(filename) == 0) return i;
    }
    return -1;
}

/*
 * Method: removeFile
 * Usage: cstk.removeFile(filename)
 * -----------------------------------
 * If such file is in the array(filegory), this function first deallocate such class, then delete the pointer to such class.
 */
void sPath::removeFile(string filename){
    int location = findLocation(filename);
    if (location == -1) error("No such file.");
    filegory[location]->~sFile();
    filegory[location] = nullptr;
}

/*
 * Method: removePath
 * Usage: cstk.removePath(path);
 * ------------------------------
 * Delete the path in the subsets
 */
void sPath::removePath(string path){
    subsets.remove(path);
}

/*
 * Method:get_name
 * Usage: cstk.get_name();
 * -------------------------
 * Return the pathname
 */
string sPath::get_name(){
    return pathname;
}

void sPath::get_all_parent(sPath * thisLevel, Set<string> & parents_book){

/*
 * Method: get_all_parent
 * Usage: cstk.get_all_parent(* thisLevel, &parents_book);
 * -------------------------------------------------------
 * get the parents of this path if it is not the root;
 * if it has no parents(it is a root), then add it to parents_book as a root
 */
void sPath::get_all_parent(sPath * thisLevel, Set<string> &parents_book){
    if (!thisLevel->is_root()) {
        parents_book.add(get_name());
        get_all_parent(thisLevel->get_parent(), parents_book);
    } else {
        parents_book.add("root");
    }
}

/*
 * Method: read_file
 * Usage: cstk.read_file(filename);
 * --------------------------------
 * If there is such file in the filegory, print the content of thefile, and return the size of the file(i.e. "50kb")
 * if there is no such file, print "No such file."
 */
int sPath::read_file(string filename){
    int location = findLocation(filename);
    if (location != -1) {
        cout << filegory[location] -> get_content() << endl;
        return filegory[location]->get_size();
    } else {
        error("No such  file.");
    }
}


void sPath::get_pwd(sPath * thisLevel, Stack<string> & pwd_road) {
/*
 * Method: get_pwd
 * Usage: cstk.get_pwd(*thisLevel, & pwd_road);
 * ---------------------------------------------
 * 这个没看懂
 */
void sPath::get_pwd(sPath *thisLevel, Stack<string> & pwd_road) {
>>>>>>> dev1
    if (!thisLevel->is_root()) {
        pwd_road.push(get_name());
        get_pwd(thisLevel->get_parent(), pwd_road);
    } else {
        pwd_road.push("root");
    }
}

bool sPath::has_file(string filename) {
    return files.contains(filename);
}

void sPath::chmod(string user, string filename, int mod){
    int location = findLocation(filename);
    if (location != -1) {
        filegory[location] -> change_mod(user, mod);
    } else {
        error("No such  file.");
    }
}
