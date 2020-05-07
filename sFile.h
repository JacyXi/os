#ifndef SFILE_H
#define SFILE_H
#include <string>
#include <iostream>
#include <cstring>
#include <strlib.h>
#include "map.h"
#include <time.h>
#include <lexicon.h>
#include <ctime>
#include <stdio.h>
using namespace std;


class sFile
{
public:
    sFile();
    ~sFile();
    sFile(string user, int mod);
    sFile(string user, int mod, string name, string content);
    string get_content();
    string get_name();
    string get_info();
    string get_type();
    int get_size();
    string get_location();
    string get_create_time();
    string get_modified_time();
    int get_mod(string user);
    string get_mod_info();
    void add_content(string content);
    void change_mod(string user, int right);
    void revoke();

private:
    string filename;
    string contents;
    Map<string,int> mod_category;
    //文件种类
    string type;
    int size;
    vector<string> location;
    time_t create_time;
    time_t modified_time;
    bool visible;
    string previous_version;

};

#endif // SFILE_H
