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

ostream & operator<<(ostream & os, Process process){
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


Memory::Memory()
{
size = Initial_total_size;
Free_size = size;
Occupied_size = 0;
inner_page_total = size/out_page_total/single_page_size;

// Add the free block(the complete memory) into the vector of Free_memory.
page_index Free_start;
page_index Free_terminate;
Free_start.outer_page_index = 0;
Free_start.inner_page_index = 0;
Free_terminate.outer_page_index = out_page_total-1; // 1023
Free_terminate.inner_page_index = inner_page_total-1; //1023

Block Free;
Free.start = Free_start;
Free.terminate = Free_terminate;
Free.Block_size = Free_size;
Free_memory.add(Free);

}

Memory::Memory(int size){
    // Remark: The size must be xxxGB, xxx must be an integer.
    this->size = size;
    Free_size = size;
    Occupied_size = 0;
    inner_page_total = (this->size)/out_page_total/single_page_size;

    // Add the free block(the complete memory) into the vector of Free_memory.
    page_index Free_start;
    page_index Free_terminate;
    Free_start.outer_page_index = 0;
    Free_start.inner_page_index = 0;
    Free_terminate.outer_page_index = out_page_total-1; // This time might not be 1023
    Free_terminate.inner_page_index = inner_page_total-1; //This time might not be 1023
    Block Free;
    Free.start = Free_start;
    Free.terminate = Free_terminate;
    Free.Block_size = Free_size;
    Free_memory.add(Free);

}

void Memory::Add_to_memory(Process process){
//    string App_name = process.App_name;
//    int process_index = process.process;
    int memory_declared = process.memory_declared;

    // Check whether the memory is full.
    if (memory_declared>Free_size){
        error("There is no enough memory space for this process!");
    }
    // Check whether you are adding the same process for the second times.
    else if (All_Process_Status.containsKey(process)){
        error("There is already such a process in the memory.");
    }

    else{
        Free_size -= memory_declared;
        Occupied_size += memory_declared;
        Current_Process.add(process);
        Vector<Block> page_table;


        int free_accumulated_memory = 0; // This is to record the accumulated memory during the process of iterating the blocks in Free_memory.
        int Free_memory_index = 0; // This is to record the index of the vector called Free_memory
        for (Block Free_Blocks : Free_memory){
            free_accumulated_memory += Free_Blocks.Block_size; // Use this variable to denote the accumulated memory added.
            if (free_accumulated_memory < memory_declared) {
                // Get the address for the free block at this iteration
                page_index Free_Block_Start = Free_Blocks.start;
                page_index Free_Block_Terminate = Free_Blocks.terminate;

                // Remove the block in free memory
                Free_memory.remove(Free_memory_index);
                Free_memory_index += 1;

                // Add occupation on the Occupied_memory
                Block new_occupied;
                new_occupied.start = Free_Block_Start;
                new_occupied.terminate = Free_Block_Terminate;
                new_occupied.Block_size = Free_Blocks.Block_size;
                Occupied_memory.add(new_occupied);

                // Update the page table for this process at this step.
                page_table.add(new_occupied);
                continue;
            }
            else{
                int remaining_memory = free_accumulated_memory - memory_declared;
                /*This is to check whether the remaining memory is 0 modulo 4.
                  Eg. The remaining memory = 5kb, then 5kb/4 = 1; then 1*4<5, we should return 1 page.
                      However, the reamaining memory should be 1*4  = 4kb as in this situation there will be fragments.
                    */

                // This variable counts the number of pages needed to free from the buttom.
                int pages_from_bottom = remaining_memory/single_page_size;

                if (pages_from_bottom * single_page_size < remaining_memory) {
                    remaining_memory = pages_from_bottom*single_page_size;
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


                page_index divide_occupied;
                divide_occupied.inner_page_index = divide_free.inner_page_index - 1;
                divide_occupied.outer_page_index = divide_free.outer_page_index;
                if (divide_occupied.inner_page_index <0){
                    divide_occupied.inner_page_index += inner_page_total;
                    divide_occupied.outer_page_index -= 1;
                }


                Block Occupied;
                Block Free;

                Occupied.start = Free_Block_Start;
                Occupied.terminate = divide_occupied;
                Occupied.Block_size = Free_Blocks.Block_size - remaining_memory;



                Free.start = divide_free;
                Free.terminate = Free_Block_Terminate;
                Free.Block_size = remaining_memory;

                Free_memory[Free_memory_index] = Free;
                Occupied_memory.add(Occupied);

                page_table.add(Occupied);

                // Update the map. Construct linkage between process and its coressponding page table.
                All_Process_Status.put(process,page_table);


            }

        }

    }

}

void Memory::Remove_from_memory(Process process){
        string App_name = process.App_name;
//        int process_index = process.process;
        int memory_declared = process.memory_declared;

        //Check whether there is such a process in the memory.
        if (! All_Process_Status.containsKey(process)) error("There is no such a process in the memory.");
        // If there is such a process, then we can start to remove.
        else {
            Vector<Block> page_table = All_Process_Status[process];


            // Clear the (process,page table) pair.
            All_Process_Status.remove(process);
            // Modify Free and Occupied size value
            Free_size += memory_declared;
            Occupied_size -= memory_declared;
            // Clear the process from the Current_Process vector.
            Current_Process.removeValue(process);
            // Adjust the two vectors that records the memory usage.
            for (Block page_table_blocks : page_table){
                Occupied_memory.removeValue(page_table_blocks);
                Free_memory.add(page_table_blocks);
            }
        }

}

Vector<Block> Memory::Get_address(Process process){
    // Check whether the input process is still in the memory or not.
    for (int i = 0; i<=Current_Process.size();i++){
        if (i == Current_Process.size()) error("There is no such a process");
        else if (Current_Process[i] == process) break;
    }

    // If there is such a process, then we can get the address(blocks) of the process.
    Vector<Block> process_block = All_Process_Status[process];
    return process_block;
}

int Memory::Get_App_memory(string App_name){
    Vector<int> App_memory_vector;
    for (Process processes : Current_Process){
        if (processes.App_name == App_name) {
            int App_memory = processes.memory_declared;
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


int Memory::Get_process_memory(Process process){
    if (! All_Process_Status.containsKey(process)) error("There is no such process in the memory.");
    else{
        int process_memory_declared = process.memory_declared;
        return process_memory_declared;
    }
}

int Memory::Get_Free_Memory_Size(){
    return Free_size;
}

int Memory::Get_Occupied_Memory_Size(){
    return Occupied_size;
}

Vector<Process> Memory::Get_Current_Process(){
    return Current_Process;
}
