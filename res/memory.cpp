#include "memory.h"
#include <iostream>
#include "strlib.h"
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

    os << substr_1 << endl << substr_2 << endl << substr_3 << endl;
    return os;
}


bool operator==(Process A,Process B){
    return (A.App_name == B.App_name && A.process == B.process && A.memory_declared == B.memory_declared);
}

bool operator!=(Process A,Process B){
    return !(A==B);}

bool operator< (const Process& lhs, const Process& rhs) {
             return ( (lhs.App_name < rhs.App_name ) || (lhs.process < rhs.process) || (lhs.memory_declared < rhs.memory_declared) );
        }

bool operator==(page_index A,page_index B){
    return (A.inner_page_index == B.inner_page_index && A.outer_page_index == B.outer_page_index);
}

bool operator==(Block A,Block B){
    return (A.start == B.start && A.terminate == B.terminate);
}








// Below are the definition of Memory class.
const double Memory::TLB_PROPORTION = 0.25;

/* Default constructor with 4GB memory*/
Memory::Memory()
{

    size = INITIAL_TOTAL_SIZE;
    Free_size = size;
    Occupied_size = 0;
    SPT_size = 0;
    TLB_size = 0;
    inner_page_total = size/OUT_PAGE_TOTAL/SINGLE_PAGE_SIZE;

    // Add the free block(the complete memory) into the vector of Free_memory.
    page_index Free_start;
    page_index Free_terminate;
    Free_start.outer_page_index = 0;
    Free_start.inner_page_index = 0;
    Free_terminate.outer_page_index = OUT_PAGE_TOTAL-1; // 1023
    Free_terminate.inner_page_index = inner_page_total-1; //1023

    Block Free;
    Free.start = Free_start;
    Free.terminate = Free_terminate;
    Free.Block_size = Free_size;
    Free_memory.add(Free);

}


/* Constructor with specified total size of the memory*/
Memory::Memory(int size)
{
    // Remark: The size must be xxxGB, xxx must be an integer.
    this->size = size;
    Free_size = size;
    Occupied_size = 0;
    SPT_size = 0;
    TLB_size = 0;
    inner_page_total = (this->size)/OUT_PAGE_TOTAL/SINGLE_PAGE_SIZE;

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
    Free.Block_size = Free_size;
    Free_memory.add(Free);

}

/*
 * Method: add_to_memory
 * Usage: add_to_memory(process)
 * --------------------------------------
 * If there the process has already been added into the memory, print error message.
 * If the memory space is not enough for the process, print error message.
<<<<<<< HEAD
 * If nothing goes wrong, then the data field of free_size, occupied_size, occupied_memory,
 * free_memory, current_process, all_process_status will be modified.
 */
void Memory::add_to_memory(Process process) {
    // string app_name = process.app_name;
    // int process_index = process.process;

    int memory_declared = process.memory_declared;
    int TLB_declared = memory_declared * TLB_PROPORTION;
    int SPT_declared = memory_declared - TLB_declared;

    int Actual_TLB_declared = Correction_on_size(TLB_declared);
    int Actual_SPT_declared = Correction_on_size(SPT_declared);
    int Actual_memory_declared = Actual_SPT_declared + Actual_TLB_declared;


    // Check whether the memory is full. If it's full, then invoke virtual memory.
    if (Actual_memory_declared>Free_size){
        Virtual_memory(process,TLB_declared);
    }
    // Check whether you are adding the same process for the second times.
    else if (All_Process_Status.containsKey(process)) {
        error("There is already such a process in the memory.");
    }

    // This is the case when we don't have to invoke virtual memory.
    else{
        // Remark: TLB_size is the first element, SPT_size is the second!
        Vector<int> Process_size;
        Process_size.add(Actual_TLB_declared);
        Process_size.add(Actual_SPT_declared);
        All_Process_Size.put(process,Process_size);

        Free_size -= Actual_memory_declared;
        Occupied_size += Actual_memory_declared;
        TLB_size += Actual_TLB_declared;
        SPT_size += Actual_SPT_declared;

        current_process.add(process);

        Vector<Block> TLB_table;
        Vector<Block> SPT_table;

        // In the next two line, the function has already modified the Free_memory and Occupied_memory.
        // Note: here the input can't use Atualized size data, as the function itself will make the correction.
        TLB_table = Partition_add_to_memory(Actual_TLB_declared);
        SPT_table = Partition_add_to_memory(Actual_SPT_declared);


        process_status page_table;
        page_table.SPT = SPT_table;
        page_table.TLB = TLB_table;

        All_Process_Status.put(process,page_table);
            }

}


/* Method:Remove_from_memory
 * Usage:Remove_from_memory(process)
 * --------------------------------------
 * If there are no such a process inside the memory, print error message.
 *
 * If nothing goes wrong, then the data field of Free_size, Occupied_size, Occupied_memory,
 * Free_memory, current_process, All_Process_Status will be modified.*/
void Memory::remove_from_memory(Process process){


        //Check whether there is such a process in the memory.
        if (! All_Process_Status.containsKey(process)) error("There is no such a process in the memory.");
        // If there is such a process, then we can start to remove.
        else {
            process_status page_table = All_Process_Status[process];
            Vector<int> Process_size = All_Process_Size[process];
            int Actual_TLB_declared = Process_size[0];
            int Actual_SPT_declared = Process_size[1];
            int Actual_memory_decalred = Actual_SPT_declared + Actual_TLB_declared;

            // Clear the (process,page table) pair and (process,Process_size) pair.
            All_Process_Status.remove(process);
            All_Process_Size.remove(process);
            // Modify Free , Occupied and TLB SPT size value
            Free_size += Actual_memory_decalred;
            Occupied_size -= Actual_memory_decalred;
            TLB_size -= Actual_TLB_declared;
            SPT_size -= Actual_SPT_declared;

            // Clear the process from the current_process vector.
            current_process.removeValue(process);
            // Adjust the two vectors that records the memory usage.
            for (Block page_table_blocks : page_table.TLB){
                Occupied_memory.removeValue(page_table_blocks);
                Free_memory.add(page_table_blocks);
            }
            for (Block page_table_blocks : page_table.SPT){
                Occupied_memory.removeValue(page_table_blocks);
                Free_memory.add(page_table_blocks);
            }

        }

}

/* Method:Get_address
 * Usage:Get_address(process)
 * --------------------------------------
 * If there are no such a process inside the memory, print error message.
 * If nothing goes wrong, then the function will return the memory location of the process
 * in the form of memory blocks.*/
Vector<Block> Memory::Get_address(const Process & process){
    // Check whether the input process is still in the memory or not.
    if (!All_Process_Status.containsKey(process)) error("There is no such a process");

    // If there is such a process, then we can get the address(blocks) of the process.
    process_status process_status = All_Process_Status[process];
    Vector<Block> TLB_Blocks = process_status.TLB;
    Vector<Block> SPT_Blocks = process_status.SPT;
    if (SPT_Blocks.size() == 0) return TLB_Blocks;
    else{
        Block TLB_tail = TLB_Blocks[TLB_Blocks.size()-1];
        Block SPT_head = SPT_Blocks[0];
        if (Is_neighbor(TLB_tail,SPT_head)){
            Block merged;
            merged.start = TLB_tail.start;
            merged.terminate = SPT_head.terminate;
            SPT_Blocks[0] = merged;
            TLB_Blocks.remove(TLB_Blocks.size()-1);
            Vector<Block> process_block = TLB_Blocks + SPT_Blocks;
            return process_block;
        }
        else {
            Vector<Block> process_block = TLB_Blocks + SPT_Blocks;
            return process_block;
        }
    }
}

/* Method:Get_App_memory
 * Usage:Get_App_memory(App_name)
 * --------------------------------------
 * This function will return the total memory occupied by the App.
 * The input is the string App_name.*/
int Memory::get_app_memory(string App_name){
    Vector<int> App_memory_vector;
    for (Process processes : current_process){
        if (processes.App_name == App_name) {
            int App_memory = All_Process_Size[processes][0] + All_Process_Size[processes][1];
            App_memory_vector.add(App_memory);
    }
    }
        if (App_memory_vector.size() == 0) error("There is no such APP in the memory.");
        else{
            // Sum up the memory for the APP_Name given.
            int Total_App_Memory = 0;
            for (int i = 0; i < App_memory_vector.size();i++){
                Total_App_Memory += App_memory_vector[i];
            }
            return Total_App_Memory;
        }
    }

/* Method:Get_process_memory
 * Usage:Get_process_memory(process)
 * --------------------------------------
 * This function will return the memory occupied by the process specified.*/
int Memory::Get_process_memory(const Process & process){
    if (! All_Process_Status.containsKey(process)) error("There is no such process in the memory.");
    else{
        int process_memory_declared = All_Process_Size[process][0] + All_Process_Size[process][1];
        return process_memory_declared;
    }
}

/* Method:get_free_memory_Size
 * Usage:get_free_memory_Size()
 * --------------------------------------
 * This function will return the total space of free memory.(The unit is kb)*/
int Memory::get_free_memory_size(){
    return Free_size;
}

/* Method:get_occupied_memory_Size
 * Usage:get_occupied_memory_Size()
 * --------------------------------------
 * This function will return the total space of occupied memory.(The unit is kb)*/
int Memory::get_occupied_memory_size(){
    return Occupied_size;
}

/* Method:Get_TLB_Size
 * Usage:Get_TLB_Size()
 * --------------------------------------
 * This function will return the space of TLB memory, which is one of the partition of the occupied memory.(The unit is kb)*/
int Memory::Get_TLB_Size(){
    return TLB_size;
}

/* Method:Get_SPT_Size
 * Usage:Get_SPT_Size()
 * --------------------------------------
 * This function will return the space of SPT memory, which is another partition of the occupied memory.(The unit is kb)*/
int Memory::Get_SPT_Size(){
    return SPT_size;
}

/* Method:get_current_process
 * Usage:get_current_process()
 * --------------------------------------
 * This function will return all the processes in the memory right now. Information include
 * App_name, process index and memory declared inside each process.*/
Vector<Process> Memory::get_current_process(){
    return current_process;
}

/* Method:get_free_memory
 * Usage:get_free_memory()
 * --------------------------------------
 * This function will return all the free memory location in the memory right now.
 * The location information will be represented as blocks in the memory.*/
Vector<Block> Memory::get_free_memory(){
    return Free_memory;
}

/* Method:get_occupied_memory
 * Usage:get_occupied_memory()
 * --------------------------------------
 * This function will return all the occupied memory location in the memory right now.
 * The location information will be represented as blocks in the memory.*/
Vector<Block> Memory::get_occupied_memory(){
    return Occupied_memory;
}


/* Below are private functions*/
/*This function is used to add the TLB and SPT part of the process into the memory seperately*/
Vector<Block> Memory::Partition_add_to_memory(int memory_declared){
    int free_accumulated_memory = 0; // This is to record the accumulated memory during the process of iterating the blocks in Free_memory.
        // Do a deep copy of Free_memory for iteration
        Vector<Block> Free_memory_copy = Free_memory;
        Vector<Block> table;
        // Start the for loop iteration
        for (Block Free_Blocks : Free_memory_copy) {
            free_accumulated_memory += Free_Blocks.Block_size; // Use this variable to denote the accumulated memory added.

            if (free_accumulated_memory < memory_declared) {
                // Get the address for the free block at this iteration
                page_index Free_Block_Start = Free_Blocks.start;
                page_index Free_Block_Terminate = Free_Blocks.terminate;


                // Remove the block in free memory
                Free_memory.remove(0);


                // Add occupation on the occupied_memory
                Block new_occupied;
                new_occupied.start = Free_Block_Start;
                new_occupied.terminate = Free_Block_Terminate;
                new_occupied.Block_size = Free_Blocks.Block_size;
                Occupied_memory.add(new_occupied);

                // Update the partition page table (could be TLB or SPT) for this process at this step.
                table.add(new_occupied);
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

                Occupied.Block_size = Free_Blocks.Block_size - remaining_memory;


                Free.start = divide_free;
                Free.terminate = Free_Block_Terminate;
                Free.Block_size = remaining_memory;

                Free_memory[0] = Free;
                Occupied_memory.add(Occupied);


                table.add(Occupied);

                // This makes sure that if going into this else statement, the for loop will terminate.
                break;
            }

        }
    return table;
    }

/* This function is used to judge whether two blocks are next to each other*/
bool Memory::Is_neighbor(const Block & b1, const Block & b2){
    page_index b1_start = b1.start;
    page_index b1_terminate = b1.terminate;
    page_index b2_start = b2.start;
    page_index b2_terminate = b2.terminate;
    if (b1_terminate.outer_page_index == b2_start.outer_page_index && b1_terminate.inner_page_index + 1 == b2_start.inner_page_index){
        return true;
    }
    else if (b1_terminate.outer_page_index + 1  == b2_start.outer_page_index && b1_terminate.inner_page_index == 1023 && b2_start.inner_page_index == 0){
        return true;
    }
    else if (b1_start.outer_page_index == b2_terminate.outer_page_index && b2_terminate.inner_page_index + 1 == b1_start.inner_page_index){
        return true;
    }
    else if (b2_terminate.outer_page_index + 1  == b1_start.outer_page_index && b2_terminate.inner_page_index == 1023 && b1_start.inner_page_index == 0){
        return true;
    }
    else return false;
}



/* This function is used to invoke virtual memory*/
void Memory::Virtual_memory(Process process,int TLB_decalred){
    int memory_declared = process.memory_declared;
    int SPT_declared = memory_declared - TLB_decalred;

    int Actual_TLB_declared = Correction_on_size(TLB_decalred);
    int Actual_SPT_declared = Correction_on_size(SPT_declared);
//    int Actual_memory_declared = Actual_TLB_declared + Actual_SPT_declared;

    // The case when there is no enough TLB space for the new process.
    if ((TLB_size + Actual_TLB_declared) > size) error("There is no enough space for the process!");

    // The case when virtual memory is able to be invoked.
    else{
        Vector<Block> Temporary_Free_Memory;
        Occupied_size = size;
        TLB_size += Actual_TLB_declared;
        int Previous_SPT_Size = SPT_size;
        SPT_size = Occupied_size - TLB_size;

        // Do a deep copy of the current_process for the iteration usage. The iteration will follow FIFO rule( Just like a queue).
        Vector<Process> Process_Queue = current_process;
        current_process.add(process);

        // Empty Free_memory and put its empty space(record by Vector of block) into TFM.
        if (Free_size != 0) {
        Free_size = 0;
        Temporary_Free_Memory = Free_memory;
        for(int i = 0;i < Free_memory.size();i++){
            Free_memory.remove(i);
        }

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
Vector<Block> Memory::Get_address(Process process) {
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
int Memory::Get_Process_Memory(Process process) {
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
=======
        // Situation when the new process requires all the SPT to be removed.
        if (SPT_need_remove_size > Previous_SPT_Size){
            for (Process process:Process_Queue){


                // Modify All_Process_Size
                Vector<int> process_size = All_Process_Size[process];
                // Here is to modify the SPT size of the exist processes.
                process_size[1] = 0;
                All_Process_Size[process] = process_size;

            // Modify Occupied_memory and All_Process_Status
            // Step1: Remove the original processes' SPT blocks in Occupied_memory and Assign empty SPT block vector to them in All_Process_Status.

                process_status ps = All_Process_Status[process];
                Vector<Block> SPT = ps.SPT;
                // Remove the corresponding blocks in the occupied_memory.
                for (Block block : SPT){
                Occupied_memory.removeValue(block);
                Temporary_Free_Memory.add(block);
                }
                // Assign an empty vector of block to SPT and then assign the new process status back to the map.
                Vector<Block> Empty;
                ps.SPT = Empty;
                All_Process_Status[process] = ps;
            }
            // Step2: Assign New space for the new process in the Occupied_memory and A_P_St.

            int free_accumulated_memory = 0; // This is to record the accumulated memory during the process of iterating the blocks in Free_memory.

            // Do a deep copy of Temporary_Free_memory for iteration
                Vector<Block> Free_memory_copy = Temporary_Free_Memory;
                Vector<Block> TLB_blocks;


            // Start the for loop iteration to arrange space for TLB of the new process.
              for (Block Free_Blocks : Free_memory_copy){
                    free_accumulated_memory += Free_Blocks.Block_size; // Use this variable to denote the accumulated memory added.

                    if (free_accumulated_memory < Actual_TLB_declared) {
                        // Get the address for the free block at this iteration
                        page_index Free_Block_Start = Free_Blocks.start;
                        page_index Free_Block_Terminate = Free_Blocks.terminate;


                        // Remove the block in temporary free memory
                        Temporary_Free_Memory.remove(0);


                        // Add occupation on the Occupied_memory
                        Block new_occupied;
                        new_occupied.start = Free_Block_Start;
                        new_occupied.terminate = Free_Block_Terminate;
                        new_occupied.Block_size = Free_Blocks.Block_size;
                        Occupied_memory.add(new_occupied);

                        TLB_blocks.add(new_occupied);
                        continue;
                    }
                    else{

                        int remaining_memory = free_accumulated_memory - Actual_TLB_declared;

                        /*This is to check whether the remaining memory is 0 modulo 4.
                          Eg. The remaining memory = 5kb, then 5kb/4 = 1; then 1*4<5, we should return 1 page.
                              However, the reamaining memory should be 1*4  = 4kb as in this situation there will be fragments.
                            */

                        // This variable counts the number of pages needed to free from the buttom.
                        int pages_from_bottom = remaining_memory/SINGLE_PAGE_SIZE;

                        if (pages_from_bottom * SINGLE_PAGE_SIZE < remaining_memory) {
                            remaining_memory = pages_from_bottom*SINGLE_PAGE_SIZE;
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
                            divide_free.outer_page_index -= 1;}

                        if (divide_free.inner_page_index > 1023) {
                            divide_free.inner_page_index -= inner_page_total;
                            divide_free.outer_page_index += 1;}



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

                        Temporary_Free_Memory[0] = Free;
                        Occupied_memory.add(Occupied);

                        TLB_blocks.add(Occupied);
                        // This makes sure that if going into this else statement, the for loop will terminate.
                        break;
                    }

                }
              Vector<Block> SPT_blocks;
              // Put all the remaining free space as occupied SPT_blocks for the new process.
              for (Block spt : Temporary_Free_Memory){
                  SPT_blocks.add(spt);
                  Occupied_memory.add(spt);
              }
              process_status new_process_status;
              new_process_status.SPT = SPT_blocks;
              new_process_status.TLB = TLB_blocks;
              All_Process_Status[process] = new_process_status;


        }




        // Situation when we have to move partial SPT from exist processes.
        else{

            int Accumulated_Exist_Process_SPT_Size = 0;
            int Process_Queue_index = 0;
            for (Process process : Process_Queue){

                Vector<int> Process_size = All_Process_Size[process];
                int Process_SPT_Size = Process_size[1];
                Accumulated_Exist_Process_SPT_Size += Process_SPT_Size;

                if (Accumulated_Exist_Process_SPT_Size > SPT_need_remove_size) break;

                Process_Queue_index += 1;

                // Below are the situation when we have to remove the complete SPT part of one of the exist process.
                // Here is to modify the SPT size of the exist processes.
                Process_size[1] = 0;
                All_Process_Size[process] = Process_size;

                // Modify Occupied_memory and All_Process_Status
                // Step1: Remove the original processes' SPT blocks in Occupied_memory and Assign empty SPT block vector to them in All_Process_Status.

                process_status ps = All_Process_Status[process];
                Vector<Block> SPT = ps.SPT;
                // Remove the corresponding blocks in the occupied_memory.
                for (Block block : SPT){
                Occupied_memory.removeValue(block);
                Temporary_Free_Memory.add(block);
                }
                // Assign an empty vector of block to SPT and then assign the new process status back to the map.
                Vector<Block> Empty;
                ps.SPT = Empty;
                All_Process_Status[process] = ps;
            }


                // Jump out of the for loop and deal with the specific SPT block we need.
                int remaining_SPT_size = Accumulated_Exist_Process_SPT_Size - SPT_need_remove_size;
                // Retrieve the specific process that we need to modify.
                Process Specific_Process = Process_Queue[Process_Queue_index];
                // Deal with the process' blocks that is not specific
                process_status ps = All_Process_Status[Specific_Process];
                int Specific_Process_SPT_size = All_Process_Size[Specific_Process][1];
                int Single_SPT_need_remove_size = Specific_Process_SPT_size - remaining_SPT_size;
                Vector<Block> SPT = ps.SPT;
                int accumulated_block_size = 0;
                int block_index = 0;
                for (Block block:(ps.SPT)){
                    int block_size = block.Block_size;
                    accumulated_block_size += block_size;
                    if (accumulated_block_size > Single_SPT_need_remove_size) break;
                    block_index += 1;
                    Occupied_memory.removeValue(block);
                    SPT.removeValue(block);
                    Temporary_Free_Memory.add(block);
                }


                // The specific block and start to cut
                Block Specific_Block = ps.SPT[block_index];


                // ************ copy from previous ************
                int remaining_memory = accumulated_block_size - Single_SPT_need_remove_size;


                // This variable counts the number of pages needed to free from the buttom.
                int pages_from_bottom = remaining_memory/SINGLE_PAGE_SIZE;

                if (pages_from_bottom * SINGLE_PAGE_SIZE < remaining_memory) {
                    remaining_memory = pages_from_bottom*SINGLE_PAGE_SIZE;
                }

                /*Take 4*1024*1024 as an example, then if the remaining memory takes 1026 pages, then
                 * inner_page_index_pre = 2, out_page_number_from_bottom = 1
                 * Hence, we can use this to calculate the index needed.
                .*/
                int inner_page_index_pre = pages_from_bottom % inner_page_total;
                int out_page_number_from_bottom = (pages_from_bottom-inner_page_index_pre)/inner_page_total;

                page_index Free_Block_Start = Specific_Block.start;
                page_index Free_Block_Terminate = Specific_Block.terminate;

                page_index divide_free;
                divide_free.inner_page_index = Free_Block_Terminate.inner_page_index - inner_page_index_pre + 1;
                divide_free.outer_page_index = Free_Block_Terminate.outer_page_index - out_page_number_from_bottom;
                if (divide_free.inner_page_index < 0) {
                    divide_free.inner_page_index += inner_page_total;
                    divide_free.outer_page_index -= 1;}

                if (divide_free.inner_page_index > 1023) {
                    divide_free.inner_page_index -= inner_page_total;
                    divide_free.outer_page_index += 1;}



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


                Block Removed;
                Block Reserved;

                Removed.start = Free_Block_Start;
                Removed.terminate = divide_occupied;
                Removed.Block_size = Specific_Block.Block_size - remaining_memory;

                Reserved.start = divide_free;
                Reserved.terminate = Free_Block_Terminate;
                Reserved.Block_size = remaining_memory;

                //************** End copy ***************

                Occupied_memory.removeValue(Specific_Block);
                Occupied_memory.add(Reserved);
                SPT.removeValue(Specific_Block);
                SPT.add(Reserved);
                Temporary_Free_Memory.add(Removed);

                // Renew the SPT table for the process.
                ps.SPT = SPT;
                All_Process_Status[Specific_Process] = ps;

                // ALL PROCESS SIZE modification towards this specific process.
                Vector<int> Specific_Process_Size = All_Process_Size[Specific_Process];
                Specific_Process_Size[1] = remaining_SPT_size;
                All_Process_Size[Specific_Process] = Specific_Process_Size;



                // Arrange new space for the new input process.
                // STEP2!!!!!!!!!!!!!




                int free_accumulated_memory = 0; // This is to record the accumulated memory during the process of iterating the blocks in Free_memory.

                // Do a deep copy of Temporary_Free_memory for iteration
                    Vector<Block> Free_memory_copy = Temporary_Free_Memory;
                    Vector<Block> TLB_blocks;


                // Start the for loop iteration to arrange space for TLB of the new process.
                  for (Block Free_Blocks : Free_memory_copy){
                        free_accumulated_memory += Free_Blocks.Block_size; // Use this variable to denote the accumulated memory added.

                        if (free_accumulated_memory < Actual_TLB_declared) {
                            // Get the address for the free block at this iteration
                            page_index Free_Block_Start = Free_Blocks.start;
                            page_index Free_Block_Terminate = Free_Blocks.terminate;


                            // Remove the block in temporary free memory
                            Temporary_Free_Memory.remove(0);


                            // Add occupation on the Occupied_memory
                            Block new_occupied;
                            new_occupied.start = Free_Block_Start;
                            new_occupied.terminate = Free_Block_Terminate;
                            new_occupied.Block_size = Free_Blocks.Block_size;
                            Occupied_memory.add(new_occupied);

                            TLB_blocks.add(new_occupied);
                            continue;
                        }
                        else{

                            int remaining_memory = free_accumulated_memory - Actual_TLB_declared;

                            /*This is to check whether the remaining memory is 0 modulo 4.
                              Eg. The remaining memory = 5kb, then 5kb/4 = 1; then 1*4<5, we should return 1 page.
                                  However, the reamaining memory should be 1*4  = 4kb as in this situation there will be fragments.
                                */

                            // This variable counts the number of pages needed to free from the buttom.
                            int pages_from_bottom = remaining_memory/SINGLE_PAGE_SIZE;

                            if (pages_from_bottom * SINGLE_PAGE_SIZE < remaining_memory) {
                                remaining_memory = pages_from_bottom*SINGLE_PAGE_SIZE;
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
                                divide_free.outer_page_index -= 1;}

                            if (divide_free.inner_page_index > 1023) {
                                divide_free.inner_page_index -= inner_page_total;
                                divide_free.outer_page_index += 1;}



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

                            Temporary_Free_Memory[0] = Free;
                            Occupied_memory.add(Occupied);

                            TLB_blocks.add(Occupied);
                            // This makes sure that if going into this else statement, the for loop will terminate.
                            break;
                        }

                    }
                  Vector<Block> SPT_blocks;
                  // Put all the remaining free space as occupied SPT_blocks for the new process.
                  for (Block spt : Temporary_Free_Memory){
                      SPT_blocks.add(spt);
                      Occupied_memory.add(spt);
                  }
                  process_status new_process_status;
                  new_process_status.SPT = SPT_blocks;
                  new_process_status.TLB = TLB_blocks;
                  All_Process_Status[process] = new_process_status;
            }
        }

        // Handle All_Process_size
        Vector<int> Process_size;
        Process_size.add(Actual_TLB_declared);

        // This is the case when the input new process is extremely large.
        if (Actual_SPT_declared > SPT_size){
            Actual_SPT_declared = SPT_size;
        }
        Process_size.add(Actual_SPT_declared);
        All_Process_Size.add(process,Process_size);

    }


int Memory::Correction_on_size(int input_size){
    if (input_size % SINGLE_PAGE_SIZE == 0) return input_size;
    else{
        int page_needed = input_size/SINGLE_PAGE_SIZE + 1;
        int actual_memory = page_needed * SINGLE_PAGE_SIZE;
        return actual_memory;
    }
}

>>>>>>> ChenYuheng
