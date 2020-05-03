#include "memory.h"
#include <iostream>
#include "strlib.h"
using namespace std;

// Below are the definition of Memory class.

/* Default constructor with 4GB memory*/
Memory::Memory()
{
    size = INITIAL_TOTAL_SIZE;
    free_size = size;
    occupied_size = 0;
    inner_page_total = size / OUT_PAGE_TOTAL / SINGLE_PAGE_SIZE;

    //Add the free block(the complete memory) into the vector of free_memory.
    page_index free_start;
    page_index Free_terminate;
    free_start.outer_page_index = 0;
    free_start.inner_page_index = 0;
    Free_terminate.outer_page_index = OUT_PAGE_TOTAL - 1; // 1023
    Free_terminate.inner_page_index = inner_page_total - 1; //1023

    Block Free;
    Free.start = free_start;
    Free.terminate = Free_terminate;
    Free.block_size = free_size;
    free_memory.add(Free);

}


/* Constructor with specified total size of the memory*/
Memory::Memory(int size)
{
    // Remark: The size must be xxxGB, xxx must be an integer.
    this -> size = size;
    free_size = size;
    occupied_size = 0;
    inner_page_total = (this -> size) / OUT_PAGE_TOTAL / SINGLE_PAGE_SIZE;

    // Add the free block(the complete memory) into the vector of free_memory.
    page_index free_start;
    page_index Free_terminate;
    free_start.outer_page_index = 0;
    free_start.inner_page_index = 0;
    Free_terminate.outer_page_index = OUT_PAGE_TOTAL - 1; // This time might not be 1023
    Free_terminate.inner_page_index = inner_page_total - 1; //This time might not be 1023
    Block Free;
    Free.start = free_start;
    Free.terminate = Free_terminate;
    Free.block_size = free_size;
    free_memory.add(Free);

}

/*
 * Method: add_to_memory
 * Usage: add_to_memory(process)
 * --------------------------------------
 * If there the process has already been added into the memory, print error message.
 * If the memory space is not enough for the process, print error message.
 * If nothing goes wrong, then the data field of free_size, occupied_size, occupied_memory,
 * free_memory, current_process, all_process_status will be modified.
 */
void Memory::add_to_memory(Process process) {
    // string app_name = process.app_name;
    // int process_index = process.process;
    int memory_declared = process.memory_declared;

    // Check whether the memory is full.
    if (memory_declared > free_size) {
        error("There is no enough memory space for this process!");
    }
    // Check whether you are adding the same process for the second times.
    else if (all_process_status.containsKey(process)) {
        error("There is already such a process in the memory.");
    }

    else {
        free_size -= memory_declared;
        occupied_size += memory_declared;
        current_process.add(process);
        Vector<Block> page_table;

        int free_accumulated_memory = 0; // This is to record the accumulated memory during the process of iterating the blocks in free_memory.

        // Do a deep copy of free_memory for iteration
        Vector<Block> free_memory_copy = free_memory;
        // Start the for loop iteration
        for (Block Free_Blocks : free_memory_copy) {
            free_accumulated_memory += Free_Blocks.block_size; // Use this variable to denote the accumulated memory added.

            if (free_accumulated_memory < memory_declared) {
                // Get the address for the free block at this iteration
                page_index Free_Block_Start = Free_Blocks.start;
                page_index Free_Block_Terminate = Free_Blocks.terminate;


                // Remove the block in free memory
                free_memory.remove(0);


                // Add occupation on the occupied_memory
                Block new_occupied;
                new_occupied.start = Free_Block_Start;
                new_occupied.terminate = Free_Block_Terminate;
                new_occupied.block_size = Free_Blocks.block_size;
                occupied_memory.add(new_occupied);

                // Update the page table for this process at this step.
                page_table.add(new_occupied);
                continue;
            } else {

                int remaining_memory = free_accumulated_memory - memory_declared;

                // This is to check whether the remaining memory is 0 modulo 4.
                // Eg. The remaining memory = 5kb, then 5kb/4 = 1; then 1*4<5, we should return 1 page.
                // However, the reamaining memory should be 1*4  = 4kb as in this situation there will be fragments.

                // This variable counts the number of pages needed to free from the buttom.
                int pages_from_bottom = remaining_memory / SINGLE_PAGE_SIZE;

                if (pages_from_bottom * SINGLE_PAGE_SIZE < remaining_memory) {
                    remaining_memory = pages_from_bottom * SINGLE_PAGE_SIZE;
                }

                // Take 4*1024*1024 as an example, then if the remaining memory takes 1026 pages, then
                // inner_page_index_pre = 2, out_page_number_from_bottom = 1
                // Hence, we can use this to calculate the index needed.

                int inner_page_index_pre = pages_from_bottom % inner_page_total;
                int out_page_number_from_bottom = (pages_from_bottom - inner_page_index_pre) / inner_page_total;

                page_index Free_Block_Start = Free_Blocks.start;
                page_index Free_Block_Terminate = Free_Blocks.terminate;

                page_index divide_free;
                divide_free.inner_page_index = Free_Block_Terminate.inner_page_index - inner_page_index_pre + 1;
                divide_free.outer_page_index = Free_Block_Terminate.outer_page_index - out_page_number_from_bottom;
                if (divide_free.inner_page_index < 0) {
                    divide_free.inner_page_index += inner_page_total;
                    divide_free.outer_page_index -= 1;
                }

                if (divide_free.inner_page_index > 1023) {
                    divide_free.inner_page_index -= inner_page_total;
                    divide_free.outer_page_index += 1;
                }

                page_index divide_occupied;
                divide_occupied.inner_page_index = divide_free.inner_page_index - 1;
                divide_occupied.outer_page_index = divide_free.outer_page_index;
                if (divide_occupied.inner_page_index < 0) {
                    divide_occupied.inner_page_index += inner_page_total;
                    divide_occupied.outer_page_index -= 1;
                }

                if (divide_occupied.inner_page_index > 1023) {
                    divide_occupied.inner_page_index -= inner_page_total;
                    divide_occupied.outer_page_index += 1;
                }

                Block Occupied;
                Block Free;

                Occupied.start = Free_Block_Start;
                Occupied.terminate = divide_occupied;
                Occupied.block_size = Free_Blocks.block_size - remaining_memory;


                Free.start = divide_free;
                Free.terminate = Free_Block_Terminate;
                Free.block_size = remaining_memory;

                free_memory[0] = Free;
                occupied_memory.add(Occupied);

                page_table.add(Occupied);

                // Update the map. Construct linkage between process and its coressponding page table.
                all_process_status.put(process,page_table);
                // This makes sure that if going into this else statement, the for loop will terminate.
                break;
            }

        }

    }

}

/*
 * Method:remove_from_memory
 * Usage:remove_from_memory(process)
 * --------------------------------------
 * If there are no such a process inside the memory, print error message.
 * If nothing goes wrong, then the data field of free_size, occupied_size, occupied_memory,
 * free_memory, current_process, all_process_status will be modified.
 */
void Memory::remove_from_memory(Process process) {
        string app_name = process.app_name;
        // int process_index = process.process;
        int memory_declared = process.memory_declared;

        // Check whether there is such a process in the memory.
        if (!all_process_status.containsKey(process)) {
            error("There is no such a process in the memory.");
        // If there is such a process, then we can start to remove.
        } else {
            Vector<Block> page_table = all_process_status[process];

            // Clear the (process,page table) pair.
            all_process_status.remove(process);
            // Modify Free and Occupied size value
            free_size += memory_declared;
            occupied_size -= memory_declared;
            // Clear the process from the current_process vector.
            current_process.removeValue(process);
            // Adjust the two vectors that records the memory usage.
            for (Block page_table_blocks : page_table) {
                occupied_memory.removeValue(page_table_blocks);
                free_memory.add(page_table_blocks);
            }
        }

}

/*
 * Method:get_address
 * Usage:get_address(process)
 * --------------------------------------
 * If there are no such a process inside the memory, print error message.
 * If nothing goes wrong, then the function will return the memory location of the process
 * in the form of memory blocks.
 */
Vector<Block> Memory::get_address(Process process) {
    // Check whether the input process is still in the memory or not.
    if (!all_process_status.containsKey(process)) error("There is no such a process");

    // If there is such a process, then we can get the address(blocks) of the process.
    Vector<Block> process_block = all_process_status[process];
    return process_block;
}

/*
 * Method:get_app_memory
 * Usage:get_app_memory(app_name)
 * --------------------------------------
 * This function will return the total memory occupied by the App.
 * The input is the string app_name.
 */
int Memory::get_app_memory(string app_name) {
    Vector<int> App_memory_vector;
    for (Process processes : current_process) {
        if (processes.app_name == app_name) {
            int App_memory = processes.memory_declared;
            App_memory_vector.add(App_memory);
        }
    }

    if (App_memory_vector.size() == 0) {
        error("There is no such APP in the memory.");
    } else {
        // Sum up the memory for the app_name given.
        int Total_App_Memory = 0;
        for (int i = 0; i < App_memory_vector.size(); i++) {
            Total_App_Memory += App_memory_vector[i];
        }
        return Total_App_Memory;
    }
}

/*
 * Method:get_process_memory
 * Usage:get_process_memory(process)
 * --------------------------------------
 * This function will return the memory occupied by the process specified.
 */
int Memory::get_process_memory(Process process) {
    if (!all_process_status.containsKey(process)) {
        error("There is no such process in the memory.");
    } else {
        int process_memory_declared = process.memory_declared;
        return process_memory_declared;
    }
}

/*
 * Method:get_free_memory_size
 * Usage:get_free_memory_size()
 * --------------------------------------
 * This function will return the total space of free memory.(The unit is kb)
 */
int Memory::get_free_memory_size() {
    return free_size;
}

/*
 * Method:get_occupied_memory_size
 * Usage:get_occupied_memory_size()
 * --------------------------------------
 * This function will return the total space of occupied memory.(The unit is kb)
 */
int Memory::get_occupied_memory_size() {
    return occupied_size;
}

/*
 * Method:get_current_process
 * Usage:get_current_process()
 * --------------------------------------
 * This function will return all the processes in the memory right now. Information include
 * app_name, process index and memory declared inside each process.
 */
Vector<Process> Memory::get_current_process() {
    return current_process;
}

/*
 * Method:get_free_memory
 * Usage:get_free_memory()
 * --------------------------------------
 * This function will return all the free memory location in the memory right now.
 * The location information will be represented as blocks in the memory.
 */
Vector<Block> Memory::get_free_memory() {
    return free_memory;
}

/*
 * Method:get_occupied_memory
 * Usage:get_occupied_memory()
 * --------------------------------------
 * This function will return all the occupied memory location in the memory right now.
 * The location information will be represented as blocks in the memory.
 */
Vector<Block> Memory::get_occupied_memory() {
    return occupied_memory;
}


ostream & operator<<(ostream & os, Block block){
    page_index start = block.start;
    page_index terminate = block.terminate;
    int start_out = start.outer_page_index;
    int start_in = start.inner_page_index;
    int terminate_out = terminate.outer_page_index;
    int terminate_in = terminate.inner_page_index;

    string substr_1 =  "Start:(Out:" + integerToString(start_out) + ",In:" + integerToString(start_in) +")";
    string substr_2 =  "Terminate:(Out:" + integerToString(terminate_out) + ",In:" + integerToString(terminate_in) +")";

    os << "{" << substr_1 << "," << substr_2 << "}";
    return os;
}

ostream & operator<<(ostream & os, Process process) {
    string substr_1 =  "APP name:" + process.app_name;
    string substr_2 =  "Process index:" + integerToString(process.process);
    string substr_3 =  "Memory declared: " + integerToString(process.memory_declared);

    os << substr_1 << endl << substr_2 << endl << substr_3 << endl;
    return os;
}


bool operator==(Process A, Process B) {
    return (A.app_name == B.app_name && A.process == B.process && A.memory_declared == B.memory_declared);
}

bool operator!=(Process A, Process B) {
    return !(A == B);
}

bool operator< (const Process& lhs, const Process& rhs) {
     return ((lhs.app_name < rhs.app_name ) || (lhs.process < rhs.process)
             || (lhs.memory_declared < rhs.memory_declared) );
}

bool operator==(page_index A, page_index B) {
    return (A.inner_page_index == B.inner_page_index && A.outer_page_index == B.outer_page_index);
}

bool operator==(Block A, Block B) {
    return (A.start == B.start && A.terminate == B.terminate);
}
