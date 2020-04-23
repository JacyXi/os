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
sFile::~sFile(){

}
int sFile::get_mod(string user){
    if (mod_category.containsKey(user)) return mod_category[user]; else error("No such user.");
}
string sFile::get_content(){
    string output;
    output = contents;
    return output;
}
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
string sFile::get_name() {
    return filename;
}
string sFile::get_type() {
    return type;
}
int sFile::get_size() {
    return size;
}

string sFile::get_location(){
    string output;
    foreach (string i in location) {
        output.append(i);
        output.append("/");
    }
    return output;
}
string sFile::get_create_time(){
    return "";
}
string sFile::get_modified_time(){
    return "";
}
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
void sFile::add_content(string content){
    contents.append(content);
}
void sFile::change_mod(string user, int right){
    if (get_mod(user) != right) mod_category[user] = right; else error("No right has to change.");
}

