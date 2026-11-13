#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cstdlib>
#include <malloc.h>
#include <algorithm>
#include "FPGrowth.hpp"
#include "Preprocessor.hpp"

using namespace std;
size_t current_bytes =0;                                    //global variable representing the current # of bytes    
size_t stage_peak_bytes =0;                                 //global variable representing the peak memory usage of each stage

void* operator new(size_t size) {                                             //overloads the new operator so that each time a new is called
    void* ptr =malloc (size);       
    if (ptr != nullptr){                                                 //as long as the new ptr isn't a nullptr
        current_bytes += _msize(ptr);                                 //the size of that ptr is added to the global variable current_bytes
        stage_peak_bytes = max(stage_peak_bytes, current_bytes);    //the peak is taken by comparing current bytes to the current peak bytes
    }
    return ptr;
}

void operator delete(void* ptr,size_t size) noexcept {          //overloads the delete operation so that when delete is called with a designated number of bytes
    if (current_bytes>= size) current_bytes -= size;          //as long as the global current variable is equal to or more than the bytes specified to remove
    free(ptr);                                              //free the pointer
}

void operator delete(void* ptr) noexcept {              //similar as the abover overload but instead its just the pointer itself
    if (ptr != nullptr) current_bytes -= _msize (ptr);
    free(ptr);
}

template <typename Func>                                          //template to represent the function
void measureStage(const string& stageName, Func func) {         //this is the function used each time a stage of the algorithm is called
    auto start_time= chrono::high_resolution_clock::now();     //timer is started
    auto start_mem= current_bytes;                             //the start memory is first set as whatever the current memory is
    stage_peak_bytes = current_bytes;                           //peak memory is also set as the current number of bytes

    func();                                                     //function is executed
    auto end_time =chrono::high_resolution_clock::now();        //end time is counted
    auto duration_time = chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();           //the duration is calculated
    auto peak_mem_usage= stage_peak_bytes - start_mem;                                                             //the peak memory usage is calculated by subtracting the peak memory to the start memory
    
    cout << stageName << ": " << duration_time << " msec" << " | Peak Memory: " << peak_mem_usage << " Bytes\n";         //the result for this specific stage is printed out
}


int main(){
    unordered_map<string,int> item_counts;
    unordered_map<string,HeaderEntry*> Header_Table;
    treenode* tree=nullptr;
    vector<string> current_suffix={};
    string inputfile = "iris.data";
    string preprocessed_file = "processed_iris.txt";
    string outputfile_name = "mined_iris.txt";
    ofstream outfile(outputfile_name);
    int instances=get_total_instances(inputfile);
    int min= instances*0.1;           //it will be considered frequent if it appear to equal to or more then 40% out of the total instances 
    cout<<"Starting.."<<endl;
    measureStage("Stage 1 : Preprocessing",[&](){               //preprocessing stage for each database
        preprocess_iris(inputfile,preprocessed_file);
    });
   
    measureStage("Stage 2 : Datapass 1",[&](){                  //the first datapass through the database counting the frequencies and saving it into item_counts
        process_data(preprocessed_file,min,item_counts);
    });

    measureStage("Stage 3 : Datapass 2",[&](){              //second data pass that creates the actual tree
        tree=process_data_2(preprocessed_file,min,item_counts,Header_Table);
    });


    measureStage("Stage 4 : Mining Tree",[&](){             //mines the actual tree for frequent item sets
        tree_breakdown(min,tree,Header_Table,current_suffix,outfile);
    });
   


    measureStage("Stage 5 : Leftover deletion",[&](){           //deletion of any leftover variables
        delete_tree(tree);
        for(auto& [item,entry] :Header_Table){
            delete entry;
        }
        Header_Table.clear();
     });
     outfile.close();
    return 0;
}