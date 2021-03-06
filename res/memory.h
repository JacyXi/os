
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
    string App_name;
    string process_name;
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
    int Block_size;
};

/* Struct process_status: Here, the process_status has 2 data fields.
   For each process, it contains two partitions of memory to store.
   The first part is TLB, which is the usually-accessed data in the memory.
   The second part is SPT, which denotes the Slow Page Table. It is the rare-accessed data in the memory.
   The TLB data will not be mutated unless the process is terminated.
   The SPT data might be swapped if virtual memory is invoked.
*/
struct process_status{
    Vector<Block> TLB;
    Vector<Block> SPT;
};



ostream & operator<<(ostream & os, Block block);
ostream & operator<<(ostream & os, Process & process);


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

    Vector<Block> Get_address(const Process & process);

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
<<<<<<< HEAD:res/memory.h

    int Get_process_memory(const Process & process);
=======
    int Get_process_memory(Process & process);
>>>>>>> ChenYuheng:memory.h


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


    /* Method:Get_TLB_Size
     * Usage:Get_TLB_Size()
     * --------------------------------------
     * This function will return the space of TLB memory, which is one of the partition of the occupied memory.(The unit is kb)*/
    int Get_TLB_Size();

    /* Method:Get_SPT_Size
     * Usage:Get_SPT_Size()
     * --------------------------------------
     * This function will return the space of SPT memory, which is another partition of the occupied memory.(The unit is kb)*/
    int Get_SPT_Size();

    /* Method:Get_Current_Process
     * Usage:Get_Current_Process()
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

    /* Method:Block_Position_Transfer
     * Usage:Block_Position_Transfer(Block block)
     * --------------------------------------
     * This function will return the specified block's start position into a double which
     * tells the proportion of the start position in the physical memory.*
     * This function only for GUI usage.*/
    Vector<double> Block_Position_Transfer(Block block);

    /* Method:Create_Process
     * Usage:Create_Process(string APP_Name,string Process_Name, int Process_Index, int Memory_Declared)
     * --------------------------------------
     * This function is used to create a Process type variable in one statement.*/
    Process Create_Process(string APP_Name,string Process_Name, int Process_Index, int Memory_Declared);

    /* Method:Remove_APP_from_memory
     * Usage:Remove_from_memory(process)
     * --------------------------------------
     * If there are no such a process inside the memory, print error message.
     * If nothing goes wrong, then the data field of Free_size, Occupied_size, Occupied_memory,
     * Free_memory, Current_Process, All_Process_Status will be modified.*/
    void Remove_APP_from_memory(string APP_Name);


private:

    static const int INITIAL_TOTAL_SIZE = 4194304;// The unit is 'kb'; 4*1024*1024 = 4GB
    static const int SINGLE_PAGE_SIZE = 4;
    static const int OUT_PAGE_TOTAL = 1024;
    static const double TLB_PROPORTION;
    int inner_page_total;
    int size;
    int Free_size;
    int Occupied_size;
    int TLB_size;
    int SPT_size;
    /* These are the vectors that are used to record the occupied space of physical memory
       TLB denotes 快表, SPT denotes 慢表。TLB union SPT = Occupied_memory.
    */
    Vector<Block> Occupied_memory;
    /*This map is used to record the processes' TLB and SPT memory size.*/
    Map<Process,Vector<int>> All_Process_Size;
    /* This is the vector that is used to record the free space of physical memory*/
    Vector<Block> Free_memory;
    /*This vector is used to record the process in the memory*/
    Vector<Process> current_process;
    /*This map is used to record the process in the memory and its according page_table in the form of vec<Block>*/
    Map<Process,process_status> All_Process_Status;
    /*This function is used to add the TLB and SPT part of the process into the memory seperately*/
    Vector<Block> Partition_add_to_memory(int memory_declared);
    /* This function is used to judge whether two blocks are next to each other*/
    bool Is_neighbor(const Block & b1,const Block & b2);
    /* This function is used to invoke virtual memory*/
    void Virtual_memory(Process process,int TLB_declared);
    /* This function is used to make a correction on the true size occupied or freed*/
    int Correction_on_size(int input_size);


};

#endif // MEMORY_H
