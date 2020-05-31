#ifndef SFILE_H
#define SFILE_H
#include <string>
#include <cstring>
#include <strlib.h>
#include <stdio.h>
#include "map.h"
#include <time.h>
#include <lexicon.h>
#include <ctime>

using namespace std;

/*
 * Class: sFile
 * -------------
 * This interface defines a class that
 * models the file in a filesystem.
 */
class sFile
{
public:
    /* Default constructor of sFile.*/
    sFile();

    /*
     * Destructor: ~sFile
     * Usage: (Usually implicit)
     * ----------------------------------
     * Deallocates storage associated with this cstk. This method is called
     * whenever a sFile instance variable is deallocated.
     */
    ~sFile();

    /*
     * Constructor: sFile
     * Usage: sFile(string user, int mod);
     * ---------------------------------------------------------------
     * Initializes a new file that has specific mode (the qualification for the user),
     * filename, and content.
     */
    sFile(string user, int mod);

    /*
     * Constructor: sFile
     * Usage: sFile(user, mod, name, content);
     * ---------------------------------------------------------------
     * Initializes a new file that has specific mode (the qualification for the user),
     * filename, and content.
     */
    sFile(string user, int mod, string name, string content,string location);

    /*
     * Method: get_content
     * Usage: get_content();
     * ----------------------------------
     * Returns the string of the content in the file.
     */
    string get_content();

    /*
     * Method: get_name
     * Usage: get_name();
     * ----------------------------------
     * Returns a string of the filename.
     */
    string get_name();

    /*
     * Method: get_info
     * Usage: get_info();
     * ----------------------------------
     * Returns the information of certain file.
     * The information includes file name, file type, file size,
     * file location, file create time, file modified time, file
     * user rights.
     */
    string get_info(string user);

    /*
     * Method: get_type
     * Usage: get_type();
     * ----------------------------------
     * Returns a string of the type of this file.
     */
    string get_type();

    /*
     * Method: get_size
     * Usage: get_size();
     * ----------------------------------
     * Returns a string of the size of this file (i.e. "200kb").
     */
    int get_size();

    /*
     * Method: get_location
     * Usage: get_location();
     * ----------------------------------
     * Returns a string of the location of this file.
     */
    string get_location();

    /*
     * Method: get_create_time
     * Usage: get_create_time();
     * -------------------------------------
     * Returns the string of the time when the file is created.
     */
    string get_create_time();

    /*
     * Method: get_modified_time
     * Usage: get_modified_time();
     * -----------------------------------------
     * Returns the string of time when the file was modified
     * by the last user.
     */
    string get_modified_time();

    /*
     * Method: get_mod
     * Usage: user_Mod = cstk.get_mod(user);
     * ----------------------------------
     * Returns the integer represents the mod (rights) for the user
     * read:4; write:2; execute:1
     * Find a key in a map and return the value if the key could be found
     * The return is the sum of all rights for the user
     */
    int get_mod(string user);

    /*
     * Method: get_mod_info
     * Usage: get_mod_info();
     * ---------------------------------------
     * Returns the string describing the rights of the user.
     */
    string get_mod_info();

    /*
     * Method: add_content
     * Usage: add_content(content);
     * ---------------------------------
     * Add a string (representing the new content) to the and of
     * the previous content.
     */
    void add_content(string content);

    /*
     * Method: change_mod
     * Usage: change_mod(user, right);
     * -------------------------------------
     * Change the user's right if the input right is different from
     * the old right. Raise error "No right has to change if the new
     * right is the same as the old one.
     */
    void change_mod(string user, int right);

    /*
     * Method: revoke
     * Usage: revoke();
     * -------------------------------------
     * Change the user's right if the input right is different from
     * the old right. Raise error "No right has to change if the new
     * right is the same as the old one.
     */
    void revoke();

    /*
     * Method: assign_location
     * Usage: assign_location();
     * ----------------------------------
     * Give file the location information.
     */
    void assign_location(string loca);

    /*
     * Method: change_content
     * Usage: change_content(user, content);
     * -------------------------------------
     * Change the user's file concent if the input right is different from
     * the old content. Raise error "No right to write. Please change your mod."
     * if the right not 7.
     */
    void change_content(string user, string content);

    /*string filename*/
    string filename;
    /*string contents*/
    string contents;
    /*Map mod_category*/
    Map<string,int> mod_category;
    /* string file type*/
    string type;
    /*string file size*/
    int size;
    /*string file location*/
    string location;
    /*time_t create time*/
    time_t create_time;
    /*time_t modified time*/
    time_t modified_time;
    /*bool visible*/
    bool visible;
    /*string previous version*/
    string previous_version;

};

#endif // SFILE_H
