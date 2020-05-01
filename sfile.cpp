#include "sFile.h"
#include <string>
#include <iostream>
#include <cstring>
#include <strlib.h>
#include "map.h"
#include "stdio.h"
#include <time.h>
#include <lexicon.h>
#include <error.h>
#include <foreach.h>
#include <ctime>

/*
 * Constructor: sFile
 * Usage: sFile cstk;
 * Initializes a new file that has specific mode (the qualification for the user), filename, and content
 */
sFile::sFile(string user, int mod)
{
    mod_category[user] = mod;

}
sFile::sFile(string user, int mod, string name, string content) {
    filename = name;
    contents = content;
    mod_category[user] = mod;
    struct tm *timeinfo;
}

/*
 * Destructor: ~sFile
 * Usage: (Usually implicit)
 * -------------------------
 * Deallocates storage associated with this cstk. This method is called
 * whenever a sFile instance variable is deallocated
 */
sFile::~sFile(){

}

/*
 * Method: get_mod
 * Usage: user_Mod = cstk.get_mod(user);
 * ------------------------------------
 * Returns the integer represents the mod (rights) for the user
 * read:4; write:2; execute:1
 * Find a key in a map and return the value if the key could be found
 * The return is the sum of all rights for the user
 */
int sFile::get_mod(string user){
    if (mod_category.containsKey(user)) return mod_category[user]; else error("No such user.");
}

/*
 * Method: get_content
 * Usage: contents = cstk.get_content();
 * --------------------------------------
 * Returns the string of the content in the file
 */
string sFile::get_content(){
    string output;
    output = contents;
    return output;
}

/*
 * Method: get_info
 * Usage: info = cstk.get_info();
 * ------------------------------
 * Returns the information of certain file
 * The information includes file name, file type, file size, file location, file create time, file modified time, file user rights
 */
string sFile::get_info(){
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
    output.append("\n");
    output.append("File modified time: ");
    output.append(get_modified_time());
    output.append("\n");
    output.append("File user rights: ");
    output.append(get_mod_info());
    output.append("\n");
    return output;
}

/*
 * Method: get_name
 * Usage: filename = cstk.get_name();
 * ----------------------------------
 * Returns a string of the filename
 */
string sFile::get_name(){
    return filename;
}

/*
 * Method: get_type
 * Usage: type = cstk.get_type();
 * -------------------------------
 * Returns a string of the type of this file
 */
string sFile::get_type(){
    return type;
}

/*
 * Method: get_size
 * Usage: size = cstk.get_size();
 * -------------------------------
 * Returns a string of the size of this file (i.e. "200kb")
 */
int sFile::get_size(){
    return size;
}

/*
 * Method: get_location
 * Usage: location = cstk.get_location();
 * --------------------------------------
 * Returns a string of the location of this file
 */
string sFile::get_location(){
    string output;
    foreach (string i in location) {
        output.append(i);
        output.append("/");  //   这个斜杠是不是反了？
    }
    return output;
}

/*
 * Method: get_create_time
 * Usage: c_time = cstk.get_create_time();
 * -------------------------------------
 * Returns the string of the time when the file is created
 */
string sFile::get_create_time(){
    return "";
}

/*
 * Method: get_modified_time
 * Usage: m_time = cstk.get_modified_time();
 * -----------------------------------------
 * Returns the string of time when the file was modified by the last user
 */
string sFile::get_modified_time(){
    return "";
}

/*
 * Method: get_mod_info
 * Usage: mod_info = cstk.get_mod_info();
 * ---------------------------------------
 * Returns the string describing the rights of the user
 */
string sFile::get_mod_info(){
    string output;
    foreach(string user in mod_category.keys()) {
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
 * Usage: cstk.add_content(content);
 * ---------------------------------
 * Add a string (representing the new content) to the and of the previous content
 */
void sFile::add_content(string content){
    contents.append(content);
}
/*
 * Method: change_mod
 * Usage: cstk.change_mod(user, right);
 * -------------------------------------
 * Change the user's right if the input right is different from the old right
 * Print "No right has to change if the new right is the same as the old one
 */
void sFile::change_mod(string user, int right){
    if (get_mod(user) != right) mod_category[user] = right; else error("No right has to change.");
}

