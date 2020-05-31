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
    /*
     * Constructor: sPath
     * Usage: sPath path;
     * ---------------------------------
     * Initialize a new empty sPath.
     */
    sPath();

    /*
     * Destructor: ~sPath
     * Usage: (Usually implicit)
     * ----------------------------------
     * Deallocate storage associated with this path. This method
     * is called whenever a sPath instance variable is deallocated.
     */
    ~sPath();

    /*
     * Constructor: sPath
     * Usage: sPath path = sPath(name, previous_path)
     * ----------------------------------
     * Initialize a new sPath with name and parent path pointer.
     */
    sPath(string name, sPath * previous_path);

    /*
     * Constructor: sPath
     * Usage: sPath path = sPath(name, is_root)
     * ----------------------------------
     * Initialize root sPath with name.
     */
    sPath(string name, bool is_root);

    /*
     * Method: addFile
     * Usage: addFile(& file);
     * -----------------------------
     * Add the address of the new file to the end of the array (filegory)
     * Add the new file into the map (files), the key is the file name,
     * and the value is its size (i.e. "50kb").
     */
    void addFile(sFile * file);

    /*
     * Method: addPath
     * Usage: addPath(sub_path_name);
     * -----------------------------------
     * Add a subset into the path, by absolute address.
     */
    void addPath(string sub_path_name);

    /*
     * Method: removePath
     * Usage: removePath(path);
     * ------------------------------
     * Delete the subset of path.
     */
    void removePath(string path);

    /*
     * Method: removeFile
     * Usage: removeFile(filename)
     * -----------------------------------
     * Delect the file. Otherwise raise error.
     */
    void removeFile(string filename);

    /*
     * Method: get_parent
     * Usage: get_parent();
     * --------------------
     * Return the parent path pointer.
     */
    sPath * get_parent();


    /*
     * Method: toString
     * Usage: toString();
     * ---------------------------
     * Get string name of sPath.
     */
    std::string toString();

    /*
     * Method: get_subsets
     * Usage: get_subsets();
     * ---------------------------
     * Return name set of subsets in this path.
     */
    Set<string> get_subsets();

    /*
     * Method: get_subsets_absolute
     * Usage: get_subsets_absolute();
     * ------------------------------
     * Return absolute address set of subsets in this path.
     */
    Set<string> get_subsets_absolute();

    /*
     * Method: get_files
     * Usage: get_files();
     * ------------------------------
     * Return filename set of files in this path.
     */
    Set<string> get_files();

    /*
     * Method: is_subset
     * Usage: is_subset(name);
     * ----------------------
     * Return if is a subset in this path.
     */
    bool is_subset(string name);

    /*
     * Method: is_root
     * Usage: is_root();
     * ----------------------
     * Return true if this path is the root path (has no parent).
     */
    bool is_root();

    /*
     * Method: get_name
     * Usage: get_name();
     * -------------------------
     * Return the path short name.
     */
    string get_name();

    /*
     * Method: get_all_parent
     * Usage: get_all_parent(* thisLevel, &parents_book);
     * --------------------------------------------------
     * Get parents of this path stored in a set.
     */
    void get_all_parent(sPath * thisLevel, Set<string> & parents_book);

    /*
     * Method: read_file
     * Usage: read_file(filename);
     * --------------------------------
     * If there is such file in the filegory, print the content of the file,
     * and return memory used in this process(i.e. "50kb").
     * Raise error "No such file." if there is no such file.
     */
    string read_file(string filename);

    /*
     * Method: get_pwd
     * Usage: cstk.get_pwd(*thisLevel, &pwd_road);
     * ---------------------------------------------
     * Return the absolute address of this path as a set.
     */
    void get_pwd(sPath * thisLevel, Stack<string> & pwd_road);

    /*
     * Method: has_file
     * Usage: has_file(filename);
     * ---------------------------------------------
     * Check if file exists.
     */
    bool has_file(string filename);

    /*
     * Method: get_file
     * Usage: get_file(filename);
     * ------------------------------
     * Return filename set of files in this path.
     */
    sFile * get_file(string filename);

    /*
     * Method: chmod
     * Usage: chmod(user, filename, mod);
     * ---------------------------------------------
     * Change the mode of file.
     */
    void chmod(string user, string filename, int mod);

    /*
     * Method: chcontent
     * Usage: chcontent(user, filename, content);
     * -----------------------------------------
     * Change the content of file.
     */
    void chcontent(string user, string filename, string content);

    /*
     * Method: get_absolute
     * Usage: get_absolute();
     * ---------------------------
     * Return the absolute path address.
     */
    string get_absolute();
private:

    /*
     * Method: findLocation
     * Usage: findLocation(filename);
     * -----------------------------------
     * Find the location of the file in filegory. Otherwise return -1.
     */
    int findLocation(string filename);
    /* Vector file book*/
    Vector<sFile*> filegory;
    /*sPath parent*/
    sPath * parent;
    /*int size*/
    int size;
    /*bool isRoot*/
    bool isRoot;
    /*int capacity*/
    int capacity;
    /*string path name*/
    string pathname;
    /*int INIT_SIZE*/
    const static int INIT_SIZE = 10;
    /*Set subsets*/
    Set<string> subsets;
    /*Set files*/
    Set<string> files;
    /*Set subsets contain absolute address*/
    Set<string> subsets_absolute;
};

#endif
