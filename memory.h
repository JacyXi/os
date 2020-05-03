#ifndef MEMORY_H
#define MEMORY_H
#include <string>
#include "vector.h"
#include "map.h"
#include "strlib.h"
using namespace std;

/*
 * Struct Process
 * -----------------------------------
 * Here, the process has 3 data fields.
 * For each App, we assume it has multiple possible processes.
 * And for each process of a specific App, it has its own declared memory.
 */
struct Process {
    string app_name;
    int process;
    int memory_declared;
};

/* Struct page index
 * -----------------------------------
 * Here, the page index has 2 data fields.
 * For each page index, it is located by its outer page index and inner page index.
 * For detailed principle please refer to OS design textbook.
 */
struct page_index {
    int outer_page_index;
    int inner_page_index;
};

/* Struct Block
 * -----------------------------------
 * Here, the Block has 3 data fields.
 * For each Block, it is refered to a continuous memory block in the memory.
 * We describe it by its start and terminate location using page index as well as its size.
 */
struct Block {
    page_index start;
    page_index terminate;
    int block_size;
};



ostream & operator<<(ostream & os, Block block);
ostream & operator<<(ostream & os, Process process);


bool operator==(Process A, Process B);
bool operator!=(Process A, Process B);
bool operator< (const Process& lhs, const Process& rhs);
bool operator==(page_index A, page_index B);
bool operator==(Block A, Block B);





class Memory
{
public:

    /* Default constructor with 4GB memory*/
    Memory();

    /* Constructor with specified total size of the memory*/
    Memory(int size);


    /* Method:add_to_memory
     * Usage:add_to_memory(process)
     * --------------------------------------
     * If there the process has already been added into the memory, print error message.
     * If the memory space is not enough for the process, print error message.
     * If nothing goes wrong, then the data field of free_size, occupied_size, Occupied_memory,
     * free_memory, current_process, all_process_status will be modified.*/
    void add_to_memory(Process process);

    /* Method:remove_from_memory
     * Usage:remove_from_memory(process)
     * --------------------------------------
     * If there are no such a process inside the memory, print error message.
     * If nothing goes wrong, then the data field of free_size, occupied_size, Occupied_memory,
     * free_memory, current_process, all_process_status will be modified.*/
    void remove_from_memory(Process process);

    /* Method:get_address
     * Usage:get_address(process)
     * --------------------------------------
     * If there are no such a process inside the memory, print error message.
     * If nothing goes wrong, then the function will return the memory location of the process
     * in the form of memory blocks.*/
    Vector<Block> get_address(Process process);

    /* Method:get_app_memory
     * Usage:get_app_memory(app_name)
     * --------------------------------------
     * This function will return the total memory occupied by the App.
     * The input is the string app_name.*/
    int get_app_memory(string app_name);

    /* Method:get_process_memory
     * Usage:get_process_memory(process)
     * --------------------------------------
     * This function will return the memory occupied by the process specified.*/
    int get_process_memory(Process process);

    /* Method:get_free_memory_size
     * Usage:get_free_memory_size()
     * --------------------------------------
     * This function will return the total space of free memory.(The unit is kb)*/
    int get_free_memory_size();

    /* Method:get_occupied_memory_size
     * Usage:get_occupied_memory_size()
     * --------------------------------------
     * This function will return the total space of occupied memory.(The unit is kb)*/
    int get_occupied_memory_size();

    /* Method:get_current_process
     * Usage:get_current_process()
     * --------------------------------------
     * This function will return all the processes in the memory right now. Information include
     * app_name, process index and memory declared inside each process.*/
    Vector<Process> get_current_process();

    /* Method:get_free_memory
     * Usage:get_free_memory()
     * --------------------------------------
     * This function will return all the free memory location in the memory right now.
     * The location information will be represented as blocks in the memory.*/
    Vector<Block> get_free_memory();

    /* Method:get_occupied_memory
     * Usage:get_occupied_memory()
     * --------------------------------------
     * This function will return all the occupied memory location in the memory right now.
     * The location information will be represented as blocks in the memory.*/
    Vector<Block> get_occupied_memory();



private:

    static const int INITIAL_TOTAL_SIZE = 4194304;// The unit is 'kb'; 4*1024*1024 = 4GB
    static const int SINGLE_PAGE_SIZE = 4;
    static const int OUT_PAGE_TOTAL = 1024;
    int inner_page_total;
    int size;
    int free_size;
    int occupied_size;

    /* These two vectors are used to record the use of physical memory*/
    Vector<Block> occupied_memory;

    Vector<Block> free_memory;
    /*This vector is used to record the process in the memory*/
    Vector<Process> current_process;
    /*This map is used to record the process in the memory and its according page_table in the form of vec<Block>*/
    Map<Process, Vector<Block>> all_process_status;

};

#endif // MEMORY_H
