/*
 * File: spath.cpp
 * ---------------
 * This file implements the sPath class.
 */

#include "sPath.h"
#include "sFile.h"
#include "set.h"
#include <vector.h>
#include <iostream>
using namespace std;

/*
 * Constructor: sPath
 * Usage: sPath path;
 * ---------------------------------
 * Initialize a new empty sPath.
 */
sPath::sPath()
{
    capacity = INIT_SIZE;
}

/*
 * Destructor: ~sPath
 * Usage: (Usually implicit)
 * ----------------------------------
 * Deallocate storage associated with this path. This method
 * is called whenever a sPath instance variable is deallocated.
 */
sPath::~sPath() {
    filegory.clear();
    parent = nullptr;
    size = 0;
    pathname = "";

}

/*
 * Constructor: sPath
 * Usage: sPath path = sPath(name, previous_path)
 * ----------------------------------
 * Initialize a new sPath with name and parent path pointer.
 */
sPath::sPath(string name, sPath * previous_path) {
    parent = previous_path;
    size = 0;
    capacity = INIT_SIZE;
    pathname = name;
    isRoot = false;
}

/*
 * Constructor: sPath
 * Usage: sPath path = sPath(name, is_root)
 * ----------------------------------
 * Initialize root sPath with name.
 */
sPath::sPath(string name, bool is_root) {
    isRoot = is_root;
    size = 0;
    capacity = INIT_SIZE;
    pathname = name;

}

/*
 * Method: is_root
 * Usage: is_root();
 * ----------------------
 * Return true if this path is the root path (has no parent).
 */
bool sPath::is_root() {
    return isRoot;
}

/*
 * Method: addFile
 * Usage: addFile(& file);
 * -----------------------------
 * Add the address of the new file to the end of the array (filegory)
 * Add the new file into the map (files), the key is the file name,
 * and the value is its size (i.e. "50kb").
 */
void sPath::addFile(sFile* file) {
    size = size + 1;
    filegory.add(file);
    files.add(file->get_name());
}


/*
 * Method: toString
 * Usage: toString();
 * ---------------------------
 * Get string name of sPath.
 */
std::string sPath::toString() {
    return pathname;
}

/*
 * Method: get_parent
 * Usage: get_parent();
 * ---------------------------
 * Return the parent path pointer.
 */
sPath * sPath::get_parent() {
    return parent;
}

/*
 * Method: get_absolute
 * Usage: get_absolute();
 * ---------------------------
 * Return the absolute path address.
 */
string sPath::get_absolute() {
    return pathname;
}

/*
 * Method: get_subsets
 * Usage: get_subsets();
 * ---------------------------
 * Return name set of subsets in this path.
 */
Set<string> sPath::get_subsets() {
    return subsets;
}

/*
 * Method: get_subsets_absolute
 * Usage: get_subsets_absolute();
 * ------------------------------
 * Return absolute address set of subsets in this path.
 */
Set<string> sPath::get_subsets_absolute() {
    return subsets_absolute;
}

/*
 * Method: get_files
 * Usage: get_files();
 * ------------------------------
 * Return filename set of files in this path.
 */
Set<string> sPath::get_files() {
    return files;
}

/*
 * Method: get_file
 * Usage: get_file(filename);
 * ------------------------------
 * Return filename set of files in this path.
 */
sFile * sPath::get_file(string filename) {
    if (has_file(filename)) {
        return filegory[findLocation(filename)];
    } else {
        error("No such file.");
    }
}

/*
 * Method: addPath
 * Usage: addPath(sub_path_name);
 * -----------------------------------
 * Add a subset into the path, by absolute address.
 */
void sPath::addPath(string sub_path_name){
    int position = sub_path_name.find("/");
    subsets.add(sub_path_name.substr(0, position));
    subsets_absolute.add(sub_path_name);
}

/*
 * Method: is_subset
 * Usage: is_subset(name);
 * ----------------------------
 * Return if is a subset in this path.
 */
bool sPath::is_subset(string name){
    return subsets.contains(name);
}

/*
 * Method: findLocation
 * Usage: findLocation(filename);
 * -----------------------------------
 * Find the location of the file in filegory. Otherwise return -1.
 */
int sPath::findLocation(string filename) {
    for (int i = 0; i < filegory.size(); i++) {
        try {
            if (filegory[i] -> get_name().compare(filename) == 0){
                return i;
            }
        } catch (ErrorException) {
            continue;
        }
    }
    return -1;
}

/*
 * Method: removeFile
 * Usage: removeFile(filename)
 * -----------------------------------
 * Delect the file. Otherwise raise error.
 */
void sPath::removeFile(string filename) {
    int location = findLocation(filename);
    if (location == -1) error("No such file.");
    filegory[location] -> ~sFile();
    filegory.remove(location);
    files.remove(filename);
}

/*
 * Method: removePath
 * Usage: removePath(path);
 * ------------------------------
 * Delete the subset of path.
 */
void sPath::removePath(string path) {
    subsets.remove(path);
}

/*
 * Method: get_name
 * Usage: get_name();
 * -------------------------
 * Return the path short name.
 */
string sPath::get_name() {
    int position = pathname.find("/");
    return pathname.substr(0, position);
}


/*
 * Method: get_all_parent
 * Usage: get_all_parent(* thisLevel, &parents_book);
 * -------------------------------------------------------
 * Get parents of this path stored in a set.
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
 * Usage: read_file(filename);
 * --------------------------------
 * If there is such file in the filegory, print the content of the file,
 * and return memory used in this process(i.e. "50kb").
 * Raise error "No such file." if there is no such file.
 */
int sPath::read_file(string filename) {
    int location = findLocation(filename);
    if (location != -1) {
        cout << filegory[location] -> get_content() << endl;
        return filegory[location] -> get_size();
    } else {
        error("No such  file.");
    }
}


/*
 * Method: get_pwd
 * Usage: cstk.get_pwd(*thisLevel, &pwd_road);
 * ---------------------------------------------
 * Return the absolute address of this path as a set.
 */
void sPath::get_pwd(sPath * thisLevel, Stack<string> & pwd_road) {
    if (!thisLevel->is_root()) {
        string name = get_name();
        pwd_road.push(name);
        get_pwd(thisLevel->get_parent(), pwd_road);
    } else {
        pwd_road.push("root");
    }
}

/*
 * Method: has_file
 * Usage: has_file(filename);
 * ---------------------------------------------
 * Check if file exists.
 */
bool sPath::has_file(string filename) {
    return files.contains(filename);
}

/*
 * Method: chmod
 * Usage: chmod(user, filename, mod);
 * ---------------------------------------------
 * Change the mode of file.
 */
void sPath::chmod(string user, string filename, int mod) {
    int location = findLocation(filename);
    if (location != -1) {
        filegory[location] -> change_mod(user, mod);
    } else {
        error("No such  file.");
    }
}
