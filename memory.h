#ifndef MEMORY_H
#define MEMORY_H
#include<string>
#include"vector.h"
#include"map.h"
#include"strlib.h"
using namespace std;

struct Process{
    string App_name;
    int process;
    int memory_declared;
};


struct page_index{
    int outer_page_index;
    int inner_page_index;
};

struct Block{
    page_index start;
    page_index terminate;
    int Block_size;
};



ostream & operator<<(ostream & os, Block block);
ostream & operator<<(ostream & os, Process process);


bool operator==(Process A,Process B);
bool operator!=(Process A,Process B);
bool operator< (const Process& lhs, const Process& rhs);
bool operator==(page_index A,page_index B);
bool operator==(Block A,Block B);





class Memory
{
public:
    Memory();
    Memory(int size);
    void Add_to_memory(Process process);
    void Remove_from_memory(Process process);
    Vector<Block> Get_address(Process process);
    int Get_App_memory(string App_Name);
    int Get_process_memory(Process process);
    int Get_Free_Memory_Size();
    int Get_Occupied_Memory_Size();
    Vector<Process> Get_Current_Process();


private:
    const int Initial_total_size = 4194304;// The unit is 'kb'; 4*1024*1024 = 4GB
    const int single_page_size = 4;
    const int out_page_total = 1024;
    int inner_page_total;
    int size;
    int Free_size;
    int Occupied_size;
    /* These two vectors are used to record the use of physical memory*/
    Vector<Block> Occupied_memory;
    Vector<Block> Free_memory;
    /*This vector is used to record the process in the memory*/
    Vector<Process> Current_Process;
    /*This map is used to record the process in the memory and its according page_table in the form of vec<Block>*/
    Map<Process,Vector<Block>> All_Process_Status;


};

#endif // MEMORY_H
