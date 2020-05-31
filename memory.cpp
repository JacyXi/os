#include "memory.h"
#include<iostream>
#include"strlib.h"
using namespace std;

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

ostream & operator<<(ostream & os, Process & process){
    string substr_1 =  "APP name:" + process.App_name;
    string substr_2 =  "Process index:" + integerToString(process.process);
    string substr_3 =  "Memory declared: " + integerToString(process.memory_declared);
    string substr_4 =  "Process name: " + process.process_name;

    os << substr_1 << endl << substr_4 << endl << substr_2 << endl << substr_3 << endl;
    return os;
}


bool operator==(Process A,Process B){
    return (A.App_name == B.App_name && A.process == B.process && A.memory_declared == B.memory_declared && A.process_name == B.process_name);
}

bool operator!=(Process A,Process B){
    return !(A==B);}

bool operator< (const Process& lhs, const Process& rhs){
    if (lhs.App_name < rhs.App_name) return 1;
    else if (lhs.App_name > rhs.App_name) return 0;
    else {
        if (lhs.process_name < rhs.process_name) return 1;
        else if (lhs.process_name > rhs.process_name) return 0;
        else {
            if (lhs.process < rhs.process) return 1;
            else if (lhs.process > rhs.process) return 0;
            else {
                if (lhs.memory_declared < rhs.memory_declared) return 1;
                else if (lhs.memory_declared > rhs.memory_declared) return 0;
                else return 0;
            }
        }
    }
}

bool operator==(page_index A,page_index B){
    return (A.inner_page_index == B.inner_page_index && A.outer_page_index == B.outer_page_index);
}

bool operator==(Block A,Block B){
    return (A.start == B.start && A.terminate == B.terminate);
}







// Below are the definition of Memory class.
const double Memory::TLB_PROPORTION = 0.25;

/*
 * Constructor: Memory
 * Usage: Memory(size)
 * -------------------
 * Initial with default size of the memory(4GB).
 */
Memory::Memory() {
    size = INITIAL_TOTAL_SIZE;
    free_size = size;
    occupied_size = 0;
    SPT_size = 0;
    TLB_size = 0;
    inner_page_total = size/OUT_PAGE_TOTAL/SINGLE_PAGE_SIZE;

    // Add the free block(the complete memory) into the vector of free_memory.
    page_index Free_start;
    page_index Free_terminate;
    Free_start.outer_page_index = 0;
    Free_start.inner_page_index = 0;
    Free_terminate.outer_page_index = OUT_PAGE_TOTAL - 1; // 1023
    Free_terminate.inner_page_index = inner_page_total - 1; //1023

    Block Free;
    Free.start = Free_start;
    Free.terminate = Free_terminate;
    Free.Block_size = free_size;
    free_memory.add(Free);

}

/*
 * Constructor: Memory
 * Usage: Memory(size)
 * -------------------
 * Initial with specified total size of the memory.
 */
Memory::Memory(int size) {
    // Remark: The size must be xxxGB, xxx must be an integer.
    this->size = size;
    free_size = size;
    occupied_size = 0;
    SPT_size = 0;
    TLB_size = 0;
    inner_page_total = (this->size)/OUT_PAGE_TOTAL/SINGLE_PAGE_SIZE;

    // Add the free block(the complete memory) into the vector of free_memory.
    page_index Free_start;
    page_index Free_terminate;
    Free_start.outer_page_index = 0;
    Free_start.inner_page_index = 0;
    Free_terminate.outer_page_index = OUT_PAGE_TOTAL - 1; // This time might not be 1023
    Free_terminate.inner_page_index = inner_page_total - 1; //This time might not be 1023
    Block Free;
    Free.start = Free_start;
    Free.terminate = Free_terminate;
    Free.Block_size = free_size;
    free_memory.add(Free);

}

/*
 * Method:Add_to_memory
 * Usage:Add_to_memory(process)
 * --------------------------------------
 * If there the process has already been added into the memory, print error message.
 * If the memory space is not enough for the process, print error message.
 * If nothing goes wrong, then the data field of free_size, occupied_size, occupied_memory,
 * free_memory, current_process, all_process_status will be modified.
 */
void Memory::Add_to_memory(Process process) {

    int memory_declared = process.memory_declared;
    int TLB_declared = memory_declared * TLB_PROPORTION;
    int SPT_declared = memory_declared - TLB_declared;

    int Actual_TLB_declared = correction_on_size(TLB_declared);
    int Actual_SPT_declared = correction_on_size(SPT_declared);
    int Actual_memory_declared = Actual_SPT_declared + Actual_TLB_declared;

    // Check whether the memory is full. If it's full, then invoke virtual memory.
    if (Actual_memory_declared > free_size) {
        virtual_memory(process, TLB_declared);
    }
    // Check whether you are adding the same process for the second times.
    else if (all_process_status.containsKey(process)) {
        error("There is already such a process in the memory.");
    }
    // This is the case when we don't have to invoke virtual memory.
    else {
        // Remark: TLB_size is the first element, SPT_size is the second!
        Vector<int> Process_size;
        Process_size.add(Actual_TLB_declared);
        Process_size.add(Actual_SPT_declared);
        all_process_size.put(process,Process_size);

        free_size -= Actual_memory_declared;
        occupied_size += Actual_memory_declared;
        TLB_size += Actual_TLB_declared;
        SPT_size += Actual_SPT_declared;

        current_process.add(process);

        Vector<Block> TLB_table;
        Vector<Block> SPT_table;

        // In the next two line, the function has already modified the free_memory and occupied_memory.
        // Note: here the input can't use Atualized size data, as the function itself will make the correction.
        TLB_table = partition_add_to_memory(Actual_TLB_declared);
        SPT_table = partition_add_to_memory(Actual_SPT_declared);

        process_status page_table;
        page_table.SPT = SPT_table;
        page_table.TLB = TLB_table;

        all_process_status.put(process,page_table);
   }

}


/*
 * Method:Remove_from_memory
 * Usage:Remove_from_memory(process)
 * --------------------------------------
 * If there are no such a process inside the memory, print error message.
 *
 * If nothing goes wrong, then the data field of free_size, occupied_size, occupied_memory,
 * free_memory, current_process, all_process_status will be modified.
 */
void Memory::Remove_from_memory(Process process) {
    //Check whether there is such a process in the memory.
    if (! all_process_status.containsKey(process)) error("There is no such a process in the memory.");
    // If there is such a process, then we can start to remove.
    else {
        process_status page_table = all_process_status[process];
        Vector<int> Process_size = all_process_size[process];
        int Actual_TLB_declared = Process_size[0];
        int Actual_SPT_declared = Process_size[1];
        int Actual_memory_decalred = Actual_SPT_declared + Actual_TLB_declared;

        // Clear the (process,page table) pair and (process,Process_size) pair.
        all_process_status.remove(process);
        all_process_size.remove(process);
        // Modify Free , Occupied and TLB SPT size value
        free_size += Actual_memory_decalred;
        occupied_size -= Actual_memory_decalred;
        TLB_size -= Actual_TLB_declared;
        SPT_size -= Actual_SPT_declared;

        // Clear the process from the current_process vector.
        current_process.removeValue(process);
        // Adjust the two vectors that records the memory usage.
        for (Block page_table_blocks : page_table.TLB) {
            occupied_memory.removeValue(page_table_blocks);
            free_memory.add(page_table_blocks);
        }
        for (Block page_table_blocks : page_table.SPT) {
            occupied_memory.removeValue(page_table_blocks);
            free_memory.add(page_table_blocks);
        }

    }

}

/*
 * Method:Get_address
 * Usage:Get_address(process)
 * --------------------------------------
 * If there are no such a process inside the memory, print error message.
 * If nothing goes wrong, then the function will return the memory location of the process
 * in the form of memory blocks.
 */
Vector<Block> Memory::Get_address(const Process & process) {
    // Check whether the input process is still in the memory or not.
    if (!all_process_status.containsKey(process)) error("There is no such a process");

    // If there is such a process, then we can get the address(blocks) of the process.
    process_status process_status = all_process_status[process];
    Vector<Block> TLB_Blocks = process_status.TLB;
    Vector<Block> SPT_Blocks = process_status.SPT;
    if (SPT_Blocks.size() == 0) return TLB_Blocks;
    else {
        Block TLB_tail = TLB_Blocks[TLB_Blocks.size() - 1];
        Block SPT_head = SPT_Blocks[0];
        if (is_neighbor(TLB_tail, SPT_head)) {
            Block merged;
            merged.start = TLB_tail.start;
            merged.terminate = SPT_head.terminate;
            merged.Block_size = TLB_tail.Block_size + SPT_head.Block_size;
            SPT_Blocks[0] = merged;
            TLB_Blocks.remove(TLB_Blocks.size() - 1);
            Vector<Block> process_block = TLB_Blocks + SPT_Blocks;
            return process_block;
        }
        else {
            Vector<Block> process_block = TLB_Blocks + SPT_Blocks;
            return process_block;
        }
    }
}

/*
 * Method:Get_App_memory
 * Usage:Get_App_memory(App_name)
 * --------------------------------------
 * This function will return the total memory occupied by the App.
 * The input is the string App_name.
 */
int Memory::Get_App_memory(string App_name) {
    Vector<int> App_memory_vector;
    for (Process processes : current_process) {
        if (processes.App_name == App_name) {
            int App_memory = all_process_size[processes][0] + all_process_size[processes][1];
            App_memory_vector.add(App_memory);
        }
    }
    if (App_memory_vector.size() == 0) {
        error("There is no such APP in the memory.");
    } else {
        // Sum up the memory for the APP_Name given.
        int Total_App_Memory = 0;
        for (int i = 0; i < App_memory_vector.size(); i++) {
            Total_App_Memory += App_memory_vector[i];
        }
        return Total_App_Memory;
    }
}

/*
 * Method:Get_process_memory
 * Usage:Get_process_memory(process)
 * --------------------------------------
 * This function will return the memory occupied by the process specified.
 */
int Memory::Get_process_memory(Process & process) {
    if (!all_process_status.containsKey(process)) {
        error("There is no such process in the memory.");
    } else {
        int process_memory_declared = all_process_size[process][0] + all_process_size[process][1];
        return process_memory_declared;
    }
}

/*
 * Method:Get_free_memory_Size
 * Usage:Get_free_memory_Size()
 * --------------------------------------
 * This function will return the total space of free memory.(The unit is kb)
 */
int Memory::Get_free_memory_Size() {
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
 * Method:Get_TLB_Size
 * Usage:Get_TLB_Size()
 * --------------------------------------
 * This function will return the space of TLB memory,
 * which is one of the partition of the occupied memory.(The unit is kb)
 */
int Memory::Get_TLB_Size() {
    return TLB_size;
}

/*
 * Method:Get_SPT_Size
 * Usage:Get_SPT_Size()
 * --------------------------------------
 * This function will return the space of SPT memory,
 * which is another partition of the occupied memory.(The unit is kb)
 */
int Memory::Get_SPT_Size() {
    return SPT_size;
}

/*
 * Method:get_current_process
 * Usage:get_current_process()
 * --------------------------------------
 * This function will return all the processes in the memory right now. Information include
 * App_name, process index and memory declared inside each process.
 */
Vector<Process> Memory::get_current_process() {
    return current_process;
}

/*
 * Method:Get_free_memory
 * Usage:Get_free_memory()
 * --------------------------------------
 * This function will return all the free memory location in the memory right now.
 * The location information will be represented as blocks in the memory.
 */
Vector<Block> Memory::Get_free_memory() {
    return free_memory;
}

/*
 * Method:Get_occupied_memory
 * Usage:Get_occupied_memory()
 * --------------------------------------
 * This function will return all the occupied memory location in the memory right now.
 * The location information will be represented as blocks in the memory.
 */
Vector<Block> Memory::Get_occupied_memory() {
    return occupied_memory;
}


/*
 * Method: partition_add_to_memory
 * Usage: partition_add_to_memory(memory_declared);
 * ------------------------------------------------
 * This function is used to add the TLB and SPT part
 * of the process into the memory seperately.
 */
Vector<Block> Memory::partition_add_to_memory(int memory_declared) {
    int free_accumulated_memory = 0; // This is to record the accumulated memory during the process of iterating the blocks in free_memory.
        // Do a deep copy of free_memory for iteration
        Vector<Block> free_memory_copy = free_memory;
        Vector<Block> table;
        // Start the for loop iteration
        for (Block Free_Blocks : free_memory_copy){
            free_accumulated_memory += Free_Blocks.Block_size; // Use this variable to denote the accumulated memory added.
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
                new_occupied.Block_size = Free_Blocks.Block_size;
                occupied_memory.add(new_occupied);

                // Update the partition page table (could be TLB or SPT) for this process at this step.
                table.add(new_occupied);
                continue;
            } else {
                int remaining_memory = free_accumulated_memory - memory_declared;

                /*This is to check whether the remaining memory is 0 modulo 4.
                  Eg. The remaining memory = 5kb, then 5kb/4 = 1; then 1*4<5, we should return 1 page.
                      However, the reamaining memory should be 1*4  = 4kb as in this situation there will be fragments.
                 */

                // This variable counts the number of pages needed to free from the buttom.
                int pages_from_bottom = remaining_memory / SINGLE_PAGE_SIZE;

                if (pages_from_bottom * SINGLE_PAGE_SIZE < remaining_memory) {
                    remaining_memory = pages_from_bottom * SINGLE_PAGE_SIZE;
                }

                /*Take 4*1024*1024 as an example, then if the remaining memory takes 1026 pages, then
                 * inner_page_index_pre = 2, out_page_number_from_bottom = 1
                 * Hence, we can use this to calculate the index needed.
                .*/
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
                if (divide_occupied.inner_page_index > 1023){
                    divide_occupied.inner_page_index -= inner_page_total;
                    divide_occupied.outer_page_index += 1;
                }

                Block Occupied;
                Block Free;

                Occupied.start = Free_Block_Start;
                Occupied.terminate = divide_occupied;
                Occupied.Block_size = Free_Blocks.Block_size - remaining_memory;

                Free.start = divide_free;
                Free.terminate = Free_Block_Terminate;
                Free.Block_size = remaining_memory;

                free_memory[0] = Free;
                occupied_memory.add(Occupied);

                table.add(Occupied);
                // This makes sure that if going into this else statement, the for loop will terminate.
                break;
            }
        }
    return table;
}

/*
 * Method: is_neighbor
 * Usage: is_neighbor(b1, b2);
 * ---------------------------
 * This function is used to judge whether
 * two blocks are next to each other.
 */
bool Memory::is_neighbor(const Block & b1, const Block & b2) {
    page_index b1_start = b1.start;
    page_index b1_terminate = b1.terminate;
    page_index b2_start = b2.start;
    page_index b2_terminate = b2.terminate;
    if (b1_terminate.outer_page_index == b2_start.outer_page_index
            && b1_terminate.inner_page_index + 1 == b2_start.inner_page_index) {
        return true;
    } else if (b1_terminate.outer_page_index + 1  == b2_start.outer_page_index
             && b1_terminate.inner_page_index == 1023 && b2_start.inner_page_index == 0) {
        return true;
    } else if (b1_start.outer_page_index == b2_terminate.outer_page_index
             && b2_terminate.inner_page_index + 1 == b1_start.inner_page_index) {
        return true;
    } else if (b2_terminate.outer_page_index + 1 == b1_start.outer_page_index
             && b2_terminate.inner_page_index == 1023 && b1_start.inner_page_index == 0) {
        return true;
    } else {
        return false;
    }
}

/*
 * Method: virtual_memory
 * Usage: virtual_memory(process, TLB_decalred);
 * ---------------------------------------------
 * This function is used to invoke virtual memory.
 */
void Memory::virtual_memory(Process process, int TLB_decalred) {
    int memory_declared = process.memory_declared;
    int SPT_declared = memory_declared - TLB_decalred;

    int Actual_TLB_declared = correction_on_size(TLB_decalred);
    int Actual_SPT_declared = correction_on_size(SPT_declared);

    // The case when there is no enough TLB space for the new process.
    if ((TLB_size + Actual_TLB_declared) > size) error("There is no enough space for the process!");

    // The case when virtual memory is able to be invoked.
    else {
        Vector<Block> Temporary_free_memory;
        occupied_size = size;
        TLB_size += Actual_TLB_declared;
        int Previous_SPT_Size = SPT_size;
        SPT_size = occupied_size - TLB_size;

        // Do a deep copy of the current_process for the iteration usage. The iteration will follow FIFO rule( Just like a queue).
        Vector<Process> Process_Queue = current_process;
        current_process.add(process);

        // Empty free_memory and put its empty space(record by Vector of block) into TFM.
        if (free_size != 0) {
            free_size = 0;
            Temporary_free_memory = free_memory;
            for (int i = 0; i < free_memory.size(); i++) {
                free_memory.remove(i);
            }
        }

        // Handle Block situation for virtual memory
        int SPT_need_remove_size = Previous_SPT_Size + Actual_SPT_declared - SPT_size;

        // Situation when the new process requires all the SPT to be removed.
        if (SPT_need_remove_size > Previous_SPT_Size) {
            for (Process process : Process_Queue) {
                // Modify all_process_size
                Vector<int> process_size = all_process_size[process];
                // Here is to modify the SPT size of the exist processes.
                process_size[1] = 0;
                all_process_size[process] = process_size;

                // Modify occupied_memory and all_process_status
                // Step1: Remove the original processes' SPT blocks in occupied_memory and Assign empty SPT block vector to them in all_process_status.
                process_status ps = all_process_status[process];
                Vector<Block> SPT = ps.SPT;
                // Remove the corresponding blocks in the occupied_memory.
                for (Block block : SPT) {
                    occupied_memory.removeValue(block);
                    Temporary_free_memory.add(block);
                }
                // Assign an empty vector of block to SPT and then assign the new process status back to the map.
                Vector<Block> Empty;
                ps.SPT = Empty;
                all_process_status[process] = ps;
            }

            // Step2: Assign New space for the new process in the occupied_memory and A_P_St.
            int free_accumulated_memory = 0; // This is to record the accumulated memory during the process of iterating the blocks in free_memory.

            // Do a deep copy of Temporary_free_memory for iteration
                Vector<Block> free_memory_copy = Temporary_free_memory;
                Vector<Block> TLB_blocks;

            // Start the for loop iteration to arrange space for TLB of the new process.
              for (Block Free_Blocks : free_memory_copy){
                    free_accumulated_memory += Free_Blocks.Block_size; // Use this variable to denote the accumulated memory added.
                    if (free_accumulated_memory < Actual_TLB_declared) {
                        // Get the address for the free block at this iteration
                        page_index Free_Block_Start = Free_Blocks.start;
                        page_index Free_Block_Terminate = Free_Blocks.terminate;

                        // Remove the block in temporary free memory
                        Temporary_free_memory.remove(0);

                        // Add occupation on the occupied_memory
                        Block new_occupied;
                        new_occupied.start = Free_Block_Start;
                        new_occupied.terminate = Free_Block_Terminate;
                        new_occupied.Block_size = Free_Blocks.Block_size;
                        occupied_memory.add(new_occupied);
                        TLB_blocks.add(new_occupied);
                        continue;
                    } else {
                        int remaining_memory = free_accumulated_memory - Actual_TLB_declared;
                        /* This is to check whether the remaining memory is 0 modulo 4.
                         * Eg. The remaining memory = 5kb, then 5kb/4 = 1; then 1*4<5, we should return 1 page.
                         * However, the reamaining memory should be 1*4  = 4kb as in this situation there will be fragments.
                         */

                        // This variable counts the number of pages needed to free from the buttom.
                        int pages_from_bottom = remaining_memory / SINGLE_PAGE_SIZE;

                        if (pages_from_bottom * SINGLE_PAGE_SIZE < remaining_memory) {
                            remaining_memory = pages_from_bottom*SINGLE_PAGE_SIZE;
                        }

                        /*Take 4*1024*1024 as an example, then if the remaining memory takes 1026 pages, then
                         * inner_page_index_pre = 2, out_page_number_from_bottom = 1
                         * Hence, we can use this to calculate the index needed.
                        .*/
                        int inner_page_index_pre = pages_from_bottom % inner_page_total;
                        int out_page_number_from_bottom = (pages_from_bottom-inner_page_index_pre) / inner_page_total;

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
                        if (divide_occupied.inner_page_index < 0){
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
                        Occupied.Block_size = Free_Blocks.Block_size - remaining_memory;

                        Free.start = divide_free;
                        Free.terminate = Free_Block_Terminate;
                        Free.Block_size = remaining_memory;

                        Temporary_free_memory[0] = Free;
                        occupied_memory.add(Occupied);

                        TLB_blocks.add(Occupied);
                        // This makes sure that if going into this else statement, the for loop will terminate.
                        break;
                    }

                }
              Vector<Block> SPT_blocks;
              // Put all the remaining free space as occupied SPT_blocks for the new process.
              for (Block spt : Temporary_free_memory) {
                  SPT_blocks.add(spt);
                  occupied_memory.add(spt);
              }
              process_status new_process_status;
              new_process_status.SPT = SPT_blocks;
              new_process_status.TLB = TLB_blocks;
              all_process_status[process] = new_process_status;
        }
        // Situation when we have to move partial SPT from exist processes.
        else{
            int Accumulated_Exist_Process_SPT_Size = 0;
            int Process_Queue_index = 0;
            for (Process process : Process_Queue) {

                Vector<int> Process_size = all_process_size[process];
                int Process_SPT_Size = Process_size[1];
                Accumulated_Exist_Process_SPT_Size += Process_SPT_Size;

                if (Accumulated_Exist_Process_SPT_Size > SPT_need_remove_size) break;

                Process_Queue_index += 1;

                // Below are the situation when we have to remove the complete SPT part of one of the exist process.
                // Here is to modify the SPT size of the exist processes.
                Process_size[1] = 0;
                all_process_size[process] = Process_size;

                // Modify occupied_memory and all_process_status
                // Step1: Remove the original processes' SPT blocks in occupied_memory and Assign empty SPT block vector to them in all_process_status.
                process_status ps = all_process_status[process];
                Vector<Block> SPT = ps.SPT;
                // Remove the corresponding blocks in the occupied_memory.
                for (Block block : SPT) {
                occupied_memory.removeValue(block);
                Temporary_free_memory.add(block);
                }
                // Assign an empty vector of block to SPT and then assign the new process status back to the map.
                Vector<Block> Empty;
                ps.SPT = Empty;
                all_process_status[process] = ps;
            }

            // Jump out of the for loop and deal with the specific SPT block we need.
            int remaining_SPT_size = Accumulated_Exist_Process_SPT_Size - SPT_need_remove_size;

            // Retrieve the specific process that we need to modify.
            Process Specific_Process = Process_Queue[Process_Queue_index];

            // Deal with the process' blocks that is not specific
            process_status ps = all_process_status[Specific_Process];
            int Specific_Process_SPT_size = all_process_size[Specific_Process][1];
            int Single_SPT_need_remove_size = Specific_Process_SPT_size - remaining_SPT_size;
            Vector<Block> SPT = ps.SPT;
            int accumulated_block_size = 0;
            int block_index = 0;
            for (Block block : (ps.SPT)) {
                int block_size = block.Block_size;
                accumulated_block_size += block_size;
                if (accumulated_block_size > Single_SPT_need_remove_size) break;
                block_index += 1;
                occupied_memory.removeValue(block);
                SPT.removeValue(block);
                Temporary_free_memory.add(block);
            }

            // The specific block and start to cut
            Block Specific_Block = ps.SPT[block_index];

            // ************ copy from previous ************
            int remaining_memory = accumulated_block_size - Single_SPT_need_remove_size;


            // This variable counts the number of pages needed to free from the buttom.
            int pages_from_bottom = remaining_memory/SINGLE_PAGE_SIZE;

            if (pages_from_bottom * SINGLE_PAGE_SIZE < remaining_memory) {
                remaining_memory = pages_from_bottom * SINGLE_PAGE_SIZE;
            }

            /*Take 4*1024*1024 as an example, then if the remaining memory takes 1026 pages, then
             * inner_page_index_pre = 2, out_page_number_from_bottom = 1
             * Hence, we can use this to calculate the index needed.
            .*/
            int inner_page_index_pre = pages_from_bottom % inner_page_total;
            int out_page_number_from_bottom = (pages_from_bottom-inner_page_index_pre) / inner_page_total;

            page_index Free_Block_Start = Specific_Block.start;
            page_index Free_Block_Terminate = Specific_Block.terminate;

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


            Block Removed;
            Block Reserved;

            Removed.start = Free_Block_Start;
            Removed.terminate = divide_occupied;
            Removed.Block_size = Specific_Block.Block_size - remaining_memory;

            Reserved.start = divide_free;
            Reserved.terminate = Free_Block_Terminate;
            Reserved.Block_size = remaining_memory;

            //************** End copy ***************

            occupied_memory.removeValue(Specific_Block);
            occupied_memory.add(Reserved);
            SPT.removeValue(Specific_Block);
            SPT.add(Reserved);
            Temporary_free_memory.add(Removed);

            // Renew the SPT table for the process.
            ps.SPT = SPT;
            all_process_status[Specific_Process] = ps;

            // ALL PROCESS SIZE modification towards this specific process.
            Vector<int> Specific_Process_Size = all_process_size[Specific_Process];
            Specific_Process_Size[1] = remaining_SPT_size;
            all_process_size[Specific_Process] = Specific_Process_Size;

            // Arrange new space for the new input process.
            // STEP2
            int free_accumulated_memory = 0; // This is to record the accumulated memory during the process of iterating the blocks in free_memory.

            // Do a deep copy of Temporary_free_memory for iteration
            Vector<Block> free_memory_copy = Temporary_free_memory;
            Vector<Block> TLB_blocks;


            // Start the for loop iteration to arrange space for TLB of the new process.
            for (Block Free_Blocks : free_memory_copy) {
                free_accumulated_memory += Free_Blocks.Block_size; // Use this variable to denote the accumulated memory added.

                if (free_accumulated_memory < Actual_TLB_declared) {
                    // Get the address for the free block at this iteration
                    page_index Free_Block_Start = Free_Blocks.start;
                    page_index Free_Block_Terminate = Free_Blocks.terminate;

                    // Remove the block in temporary free memory
                    Temporary_free_memory.remove(0);

                    // Add occupation on the occupied_memory
                    Block new_occupied;
                    new_occupied.start = Free_Block_Start;
                    new_occupied.terminate = Free_Block_Terminate;
                    new_occupied.Block_size = Free_Blocks.Block_size;
                    occupied_memory.add(new_occupied);

                    TLB_blocks.add(new_occupied);
                    continue;
                } else {
                    int remaining_memory = free_accumulated_memory - Actual_TLB_declared;

                    /* This is to check whether the remaining memory is 0 modulo 4.
                     * Eg. The remaining memory = 5kb, then 5kb/4 = 1; then 1*4<5, we should return 1 page.
                     * However, the reamaining memory should be 1*4  = 4kb as in this situation there will be fragments.
                     */

                    // This variable counts the number of pages needed to free from the buttom.
                    int pages_from_bottom = remaining_memory / SINGLE_PAGE_SIZE;

                    if (pages_from_bottom * SINGLE_PAGE_SIZE < remaining_memory) {
                        remaining_memory = pages_from_bottom * SINGLE_PAGE_SIZE;
                    }

                    /*Take 4*1024*1024 as an example, then if the remaining memory takes 1026 pages, then
                     * inner_page_index_pre = 2, out_page_number_from_bottom = 1
                     * Hence, we can use this to calculate the index needed.
                    .*/
                    int inner_page_index_pre = pages_from_bottom % inner_page_total;
                    int out_page_number_from_bottom = (pages_from_bottom-inner_page_index_pre)/inner_page_total;

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
                    if (divide_occupied.inner_page_index <0){
                        divide_occupied.inner_page_index += inner_page_total;
                        divide_occupied.outer_page_index -= 1;
                    }

                    if (divide_occupied.inner_page_index >1023){
                        divide_occupied.inner_page_index -= inner_page_total;
                        divide_occupied.outer_page_index += 1;
                    }

                    Block Occupied;
                    Block Free;

                    Occupied.start = Free_Block_Start;
                    Occupied.terminate = divide_occupied;
                    Occupied.Block_size = Free_Blocks.Block_size - remaining_memory;

                    Free.start = divide_free;
                    Free.terminate = Free_Block_Terminate;
                    Free.Block_size = remaining_memory;

                    Temporary_free_memory[0] = Free;
                    occupied_memory.add(Occupied);

                    TLB_blocks.add(Occupied);
                    // This makes sure that if going into this else statement, the for loop will terminate.
                    break;
                }
          }
          Vector<Block> SPT_blocks;
          // Put all the remaining free space as occupied SPT_blocks for the new process.
          for (Block spt : Temporary_free_memory) {
              SPT_blocks.add(spt);
              occupied_memory.add(spt);
          }
          process_status new_process_status;
          new_process_status.SPT = SPT_blocks;
          new_process_status.TLB = TLB_blocks;
          all_process_status[process] = new_process_status;
        }
    }

    // Handle all_process_size
    Vector<int> Process_size;
    Process_size.add(Actual_TLB_declared);

    // This is the case when the input new process is extremely large.
    if (Actual_SPT_declared > SPT_size) {
        Actual_SPT_declared = SPT_size;
    }
    Process_size.add(Actual_SPT_declared);
    all_process_size.add(process,Process_size);

}

/*
 * Method: correction_on_size
 * Usage: correction_on_size(input_size);
 * ------------------------------------------
 * This function is used to make a correction on the
 * true size occupied or freed.
 */
int Memory::correction_on_size(int input_size){
    if (input_size % SINGLE_PAGE_SIZE == 0) return input_size;
    else{
        int page_needed = input_size/SINGLE_PAGE_SIZE + 1;
        int actual_memory = page_needed * SINGLE_PAGE_SIZE;
        return actual_memory;
    }
}

/*
 * Method:block_position_transfer
 * Usage:block_position_transfer(Block block)
 * --------------------------------------
 * This function will return the specified block's start position into a double which
 * tells the proportion of the start position in the physical memory.
 * This function only for GUI usage.
 */
Vector<double> Memory::block_position_transfer(Block block) {
    Vector<double> result;
    double pagenum = 1024;
    int start_out = block.start.outer_page_index;
    int start_in = block.start.inner_page_index;
    int stotalPage = start_out * 1024 + start_in;
    double dstp = stotalPage;
    double start_proportion = dstp / pagenum / pagenum;

    int terminate_out = block.terminate.outer_page_index;
    int terminate_in = block.terminate.inner_page_index;
    int ttotalPage = terminate_out * 1024 + terminate_in;
    double dttp = ttotalPage;
    double terminate_proportion = dttp / pagenum / pagenum;

    result.add(start_proportion);
    result.add(terminate_proportion);
    return result;
}

/*
 * Method:create_process
 * Usage:create_process(APP_Name, Process_Name, Process_Index, Memory_Declared);
 * --------------------------------------
 * This function is used to create a Process type variable in one statement.
 */
Process Memory::create_process(string APP_Name, string Process_Name, int Process_Index, int Memory_Declared) {
    Process process;
    process.App_name = APP_Name;
    process.process_name = Process_Name;
    process.process = Process_Index;
    process.memory_declared = Memory_Declared;
    return process;
}

/*
 * Method:remove_app_from_memory
 * Usage:Remove_from_memory(process)
 * --------------------------------------
 * If there are no such a process inside the memory, print error message.
 * If nothing goes wrong, then the data field of free_size, occupied_size, occupied_memory,
 * free_memory, current_process, all_process_status will be modified.
 */
void Memory::remove_app_from_memory(string APP_Name) {
    Vector<Process> CP = current_process;
    for (Process process : CP) {
        if (process.App_name != APP_Name) {
            continue;
        } else {
            Remove_from_memory(process);
        }
    }
}

/*
 * Method: In_Memory
 * Usage: In_Memory(process)
 * --------------------------------------
 * Judge whether the process is already in the memory or not.
 * If yes, then return true. Otherwise return false.
 */
bool Memory::In_Memory(Process process) {
    return all_process_status.containsKey(process) ? 1 : 0;
}
