#include "sFile.h"
#include <string>
#include <iostream>
#include <cstring>
#include <strlib.h>
#include "map.h"
#include "stdio.h"
#include <time.h>
#include <error.h>
#include <ctime>

/*
 * Constructor: sFile
 * Usage: sFile(string user, int mod);
 * ---------------------------------------------------------------
 * Initializes a new file that has specific mode (the qualification for the user),
 * filename, and content.
 */
sFile::sFile(string user, int mod)
{
    mod_category[user] = mod;

}

/*
 * Constructor: sFile
 * Usage: sFile(user, mod, name, content);
 * ---------------------------------------------------------------
 * Initializes a new file that has specific mode (the qualification for the user),
 * filename, and content.
 */
sFile::sFile(string user, int mod, string name, string content,string loca) {
    filename = name;
    contents = content;
    mod_category[user] = mod;
    create_time = time(0);
    modified_time = time(0);
    type = filename.substr(filename.rfind("."));
    location = loca;
}

/*
 * Destructor: ~sFile
 * Usage: (Usually implicit)
 * ----------------------------------
 * Deallocates storage associated with this cstk. This method is called
 * whenever a sFile instance variable is deallocated.
 */
sFile::~sFile() {
    filename = "";
    contents = "";
    mod_category.clear();
}

/*
 * Method: get_mod
 * Usage: user_Mod = cstk.get_mod(user);
 * ----------------------------------
 * Returns the integer represents the mod (rights) for the user
 * read:4; write:2; execute:1
 * Find a key in a map and return the value if the key could be found
 * The return is the sum of all rights for the user
 */
int sFile::get_mod(string user) {
    if (!mod_category.containsKey(user)) {
        mod_category.add(user, 7);
    }
    return mod_category[user];
}

/*
 * Method: assign_location
 * Usage: assign_location();
 * ----------------------------------
 * Give file the location information.
 */
void sFile::assign_location(string loca) {
    location = loca;
}

/*
 * Method: get_content
 * Usage: get_content();
 * ----------------------------------
 * Returns the string of the content in the file.
 */
string sFile::get_content() {
    string output;
    output = contents;
    return output;
}

/*
 * Method: get_info
 * Usage: get_info();
 * ----------------------------------
 * Returns the information of certain file.
 * The information includes file name, file type, file size,
 * file location, file create time, file modified time, file
 * user rights.
 */
string sFile::get_info(string user) {
    if (!mod_category.containsKey(user)) {
        mod_category.add(user, 7);
    }
    if (mod_category.get(user) >= 4) {
        string output;
        output.append("File name: ");
        output.append(filename);
        output.append("\n");
        output.append("File type: ");
        output.append(get_type());
        output.append("\n");
        output.append("File size: ");
        output.append(to_string(get_size()).append(" kb"));
        output.append("\n");
        output.append("File location: ");
        output.append(get_location());
        output.append("\n");
        output.append("File create time: ");
        output.append(get_create_time());
        output.append("File modified time: ");
        output.append(get_modified_time());
        output.append("File user rights: \n");
        output.append(get_mod_info());
        output.append("\n");
        return output;
    } else {
        return "Forbid for inspection.";
    }
}

/*
 * Method: get_name
 * Usage: get_name();
 * ----------------------------------
 * Returns a string of the filename.
 */
string sFile::get_name() {
    return filename;
}

/*
 * Method: get_type
 * Usage: get_type();
 * ----------------------------------
 * Returns a string of the type of this file.
 */
string sFile::get_type() {
    return type;
}

/*
 * Method: get_size
 * Usage: get_size();
 * ----------------------------------
 * Returns a string of the size of this file (i.e. "200kb").
 */
int sFile::get_size() {
    return contents.length();
}

/*
 * Method: get_location
 * Usage: get_location();
 * ----------------------------------
 * Returns a string of the location of this file.
 */
string sFile::get_location() {
    return location;
}

/*
 * Method: get_create_time
 * Usage: get_create_time();
 * -------------------------------------
 * Returns the string of the time when the file is created.
 */
string sFile::get_create_time() {
    return ctime(&create_time);
}

/*
 * Method: get_modified_time
 * Usage: get_modified_time();
 * -----------------------------------------
 * Returns the string of time when the file was modified
 * by the last user.
 */
string sFile::get_modified_time() {
    return ctime(&modified_time);
}

/*
 * Method: get_mod_info
 * Usage: get_mod_info();
 * ---------------------------------------
 * Returns the string describing the rights of the user.
 */
string sFile::get_mod_info() {
    string output;
    for (string user : mod_category.keys()) {
        output.append("User ");
        output.append(user);
        output.append(": ");
        output.append(to_string(mod_category[user]));
        output.append("\n");
    }
    return output;
}

/*
 * Method: add_content
 * Usage: add_content(content);
 * ---------------------------------
 * Add a string (representing the new content) to the and of
 * the previous content.
 */
void sFile::add_content(string content) {
    modified_time = time(0);
    contents.append(content);
}

/*
 * Method: change_mod
 * Usage: change_mod(user, right);
 * -------------------------------------
 * Change the user's right if the input right is different from
 * the old right. Raise error "No right has to change if the new
 * right is the same as the old one.
 */
void sFile::change_mod(string user, int right) {
    if (get_mod(user) != right) {
        modified_time = time(0);
        mod_category[user] = right;
    } else {
        modified_time = time(0);
    }
}

/*
 * Method: change_content
 * Usage: change_content(user, content);
 * -------------------------------------
 * Change the user's file concent if the input right is different from
 * the old content. Raise error "No right to write. Please change your mod."
 * if the right not 7.
 */
void sFile::change_content(string user, string content) {
    if (!mod_category.containsKey(user)){
        mod_category.add(user, 7);
    }
    if (mod_category.get(user) != 7) {
        error("No right to write. Please change your mod.");
    } else {
        modified_time = time(0);
        previous_version = contents;
        contents = content;
    }
}

/*
 * Method: revoke
 * Usage: revoke();
 * -------------------------------------
 * Change the user's right if the input right is different from
 * the old right. Raise error "No right has to change if the new
 * right is the same as the old one.
 */
void sFile::revoke() {
    if (previous_version.length() > 0) {
        contents = previous_version;
    } else {
        error("Could only revoke once");
    }
}

