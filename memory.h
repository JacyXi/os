#ifndef MEMORY_H
#define MEMORY_H
#include<string>
#include"vector.h"
#include"map.h"
#include"strlib.h"
using namespace std;

/* Struct Process: Here, the process has 3 data fields.
   For each App, we assume it has multiple possible processes.
   And for each process of a specific App, it has its own declared memory.
*/
struct Process{
    string App_name;
    int process;
    int memory_declared;
};

/* Struct page index: Here, the page index has 2 data fields.
   For each page index, it is located by its outer page index and inner page index.
   For detailed principle please refer to OS design textbook.
*/
struct page_index{
    int outer_page_index;
    int inner_page_index;
};

/* Struct Block: Here, the Block has 3 data fields.
   For each Block, it is refered to a continuous memory block in the memory.
   We describe it by its start and terminate location using page index as well as its size.
*/
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
    /* Default constructor with 4GB memory*/
    Memory();

    /* Constructor with specified total size of the memory*/
    Memory(int size);

    /* Method to add a specific process into the memory, the allocate algorithm will allocate the memory to this process*/
    void Add_to_memory(Process process);

    /* Method to remove a specific process out of the memory, also will free the memory*/
    void Remove_from_memory(Process process);

    /* Method to get a specific process's address location in the memory*/
    Vector<Block> Get_address(Process process);

    /* Method to get a specific App's total memory size inside the memory currently*/
    int Get_App_memory(string App_Name);

    /* Method to get a specific process's memory size inside the memory currently*/
    int Get_process_memory(Process process);

    /* Method to get the size of total free space inside the memory currently*/
    int Get_Free_Memory_Size();

    /* Method to get the size of total occupied space inside the memory currently*/
    int Get_Occupied_Memory_Size();

    /* Method to get all the current process inside the mememory right currently*/
    Vector<Process> Get_Current_Process();

    /* Method to get the free space fragments(blocks) inside the memory right now*/
    Vector<Block> Get_Free_memory();

    /* Method to get the occupied space fragments(blocks) inside the memory right now*/
    Vector<Block> Get_Occupied_memory();



private:
    static const int INITIAL_TOTAL_SIZE = 4194304;// The unit is 'kb'; 4*1024*1024 = 4GB
    static const int SINGLE_PAGE_SIZE = 4;
    static const int OUT_PAGE_TOTAL = 1024;
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
