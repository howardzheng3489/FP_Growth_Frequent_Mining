#include <iostream>
#include <string.h>
#include <unordered_map>
#include <fstream>   
#include <sstream>   
#include <algorithm>
#include <stdexcept>
#include "FPGrowth.hpp"

using namespace std;

void process_data(const string& filename, int min,unordered_map<string, int>& item_counts){     //pass 1 of the fptree
    ifstream file(filename);                            //get a stream from the file and if it can't open return
    if(!file.is_open()){
        cerr<<"File failed to open" << std::endl;
        return;
    }
    string line;                    //extract a line
    while(getline(file, line)){
        string item;                //item represents each entry of the line
        replace(line.begin(),line.end(),',',' ');       //remove any commas of the current line just for ease of processing
        stringstream ss(line);                          //put the line in a stream
        while(ss >> item){                              //take each item
            unordered_map<string, int>:: iterator it=item_counts.find(item);        //check whether item exists in unordered_map item-counts
            if(it == item_counts.end()){                //if not make a new entry
                item_counts.insert({item,1});
            }
            else{                                        //if it does exist increment that entries count
               item_counts[item]++;
            }

        }
    }
    vector<string> to_remove;          
    for (const auto& pair : item_counts) {           //now go through the map
        if (pair.second < min) {                 //if the count of the entry is below the minimum
            to_remove.push_back(pair.first);    //add it to the vector that denotes what to remove
        }
    }

    for (const string& key : to_remove) {       //go through the vector that says which to remove
        item_counts.erase(key);                 //remove that entry from the map
    }   
    file.close();                               //close the file
}
treenode* process_data_2                        //second pass through the database
(const string& filename, int min,const unordered_map<string, int>& item_counts,unordered_map<string, HeaderEntry*>& Table)
{
    ifstream file(filename);            //go through the file
     if(!file.is_open()){
        cerr<<"File failed to open" << std::endl;
        return nullptr;             //if opening file failes just return a nullptr
    }
    string line;                    //represents each line of file          
    treenode* fptree=new treenode{"root",0,nullptr,{},nullptr};         //root of the fptree
    vector<string> transactions;                                    //vector to save list of items/transaction
    while(getline(file,line)){                                      //go through the file
        string item;
        transactions.clear();                                       //clear transaction first
        replace(line.begin(),line.end(),',',' ');                   //clear the line for commas
        stringstream ss(line);
        while(ss >> item){                                          //go through the line
          auto it=item_counts.find(item);                           //now check if the current item in this line is in item_counts
           if(it != item_counts.end()){                             //if it does exist that means it passed the first minimum count filter
                transactions.push_back(item);                       //push it into transaction
           }
        }
        sort(transactions.begin(),transactions.end(),[&item_counts](const string& a, const string& b){         
            if(item_counts.at(a) != item_counts.at(b)){              //once finished sort transactions based on item_count
                return item_counts.at(a)>item_counts.at(b);
            }
            return a > b;
        });
        insert_path(transactions,fptree,Table);                 //insert the transaction into the tree

    }
    return fptree;                                              //return the fptree
}
void insert_path                                            //path creation
(vector<string>& transactions,treenode* root,unordered_map<string, HeaderEntry*>& Header_Table){
    treenode* curr=root;                                               //start from the root of the tree
    for(int i=0; i<transactions.size();i++){                            //for each item in the vector
        unordered_map<string, treenode*>:: iterator child=curr->children.find(transactions[i]);         //check if the current node has a child transaction[i]
        if(child == curr->children.end()){                                                  //if it doesn't
            treenode* newchild= new treenode{transactions[i],1,nullptr,{},nullptr};         //make a new entry for that item witha frequency of 1
            newchild->parent=curr;                                                         //link the child to parent and vice versa
            curr->children.insert({transactions[i],newchild});
            curr=newchild;                                                                //have current point to the new child
            unordered_map<string, HeaderEntry*>::iterator it=Header_Table.find(transactions[i]);        //now check the header table
            if(it ==Header_Table.end()){                                        //if there is no header table entry for transaction[i]
                HeaderEntry* new_entry = new HeaderEntry{1,curr};               //make that entry
                Header_Table.insert({transactions[i],new_entry});               //and have that entry point to the current node
            }
            else{                                                               //header entry exists
                newchild->same_next = Header_Table[transactions[i]]->head;      //have this new node connect to the node that the entry is currently connected to        
                Header_Table[transactions[i]]->head = newchild;                 //have the header entry now point to this new child
                Header_Table[transactions[i]]->total_count++;                   //increment the header entry
            }
        }
        else{                                                   //child was found
            child->second->frequency++;                         //that means we're going through either a path that's already be done before or something very similar
            curr=child->second;                                 //all you need to do is increment the count of both the node's frequency and header entry
            Header_Table[transactions[i]]->total_count++;
        }

    }
}

//sub tree creation used for mining the tree
void create_subtree(pair<int,vector<string>> cleaned_path,unordered_map<string, HeaderEntry*>& Sub_Table,treenode* sub_root){
    int path_size=cleaned_path.second.size();                   //very similar to insert path logic but instead of just incrementing, you're adding by the count of the cleaned path
    treenode* curr=sub_root;
    for(int i =0; i < path_size; i++){
        unordered_map<string,treenode*>:: iterator it=curr->children.find(cleaned_path.second[i]);
         if(it == curr->children.end()){
            treenode * newchild= new treenode{cleaned_path.second[i],cleaned_path.first,nullptr,{},nullptr};
            newchild->parent=curr;
            curr->children.insert({cleaned_path.second[i],newchild});
            curr=newchild;
            unordered_map<string, HeaderEntry*>::iterator entry=Sub_Table.find(cleaned_path.second[i]);
            if(entry == Sub_Table.end()){
                HeaderEntry* new_entry= new HeaderEntry{cleaned_path.first,curr};
                Sub_Table.insert({cleaned_path.second[i],new_entry});
            }
            else{
                newchild->same_next = Sub_Table[cleaned_path.second[i]]->head;
                Sub_Table[cleaned_path.second[i]]->head = newchild;
                Sub_Table[cleaned_path.second[i]]->total_count+=cleaned_path.first;
            }
         }
         else{
            it->second->frequency+=cleaned_path.first;
            curr=it->second;
            Sub_Table[cleaned_path.second[i]]->total_count+=cleaned_path.first;
        }

    }
}

void delete_tree(treenode* node){           //function to delete the tree
    if(!node) return;

    for(auto& [name,child]:node->children){
        delete_tree(child);
    }
    delete node;
}

bool is_single_path(treenode* root) {           //helper function to check if a tree is a single path   
    treenode* curr = root;                      //from the root
    while(curr->children.size() == 1) {         //if the number of children is 1
        curr = curr->children.begin()->second;     //go further down the tree
    }
    return curr->children.empty();          // Returns true if it reaches a leaf without branching
}

//tree mining function
void tree_breakdown
(int min, treenode* root, unordered_map<string, HeaderEntry*>& Header_Table,vector<string> current_suffix,ofstream& outfile){
    vector<pair<string,int>> items_frequency;           //make a vector representing each item of the header table list and there frequency
    if(root->children.size() == 0) return;
    if (is_single_path(root)) {                               //if the tree is a single path
        vector<pair<string, int>> path_nodes;                //to store each node of this path with it's frequency
        treenode* curr = root;                              //starting from root
        while (curr->children.size() == 1) {               //while it remains a single path
            curr = curr->children.begin()->second;         //go down the tree and save the name of the item and it's frequency
            path_nodes.push_back({curr->name, curr->frequency});
        }
       int k=path_nodes.size();                     //take the size of the path
       int permutations= (1<< k) - 1;            //using the size of the path, calculate the number of possible permutations of the path
       vector<string> path=current_suffix;      //represents the path that was mined before this single path tree was created and this will be used for the new paths
       for(int bit_mask = 1; bit_mask <= permutations; ++bit_mask){        //go through each possible permutation by using a bit masl
            path=current_suffix;                                //start from the original path from before
            int min_count=INT_MAX;
            for(int i=0; i< k; i++){                        //go through every possible number from 1 to the path length
                if(bit_mask & (1 << i)){                    //if the bit at i of the bit_mask is 1            
                    path.push_back(path_nodes[i].first);        //you push item i of the single path into path
                    min_count=std::min(min_count,path_nodes[i].second);     
                    /*you extract the minimum count between the nodes,
                     this is because the occurences of that path is denoted by 
                     the node with the least frequency*/
                }
            }
            for(const auto& item : path) {              //print this path and its count
                outfile << item << " ";
            }
            outfile << "| Count:" << min_count << "\n";     //return
        }
        return;
    }
    for(const auto& [item,entry] : Header_Table){                          
        items_frequency.push_back({item,entry->total_count});
    }
    sort(items_frequency.begin(),items_frequency.end(),             //sort the list with the smaller number being first       
    [](const auto& a, const auto& b){
        return a.second < b.second;
    });
    vector<string> path;                                         //vector representing current path
    for(int i = 0; i<items_frequency.size(); i++){               //go through the vector of the header table
        vector<string> new_suffix=current_suffix;                //this makes a string representing the current pattern already mine before if this was recursively called which will be pushed into mined patterns
        new_suffix.push_back(items_frequency[i].first);         //pushes the current entries name into the pattern
        for(const auto& item : new_suffix) {
            outfile << item << " ";
        }
        outfile << "| Count:" << items_frequency[i].second << "\n";
       
        vector<pair<int,vector<string>>> conditional_pattern_base;              //this will represent the current conditional pattern base
        unordered_map<string,int>conditional_item;                              //each item that the current item has been in a list combination with
        treenode* curr=Header_Table[items_frequency[i].first]->head;            //now have current be pointing at the node the current header_entry table is pointing to
        while(curr != nullptr){             //while curr is not a null pointer
            path={};                        //empty path vector
            treenode* nav=curr->parent;     //go to the parent of curr
            int count= curr->frequency;     //extract the frequency of the current node, since that would be the amount of times this path has shown up
            while(nav != nullptr && nav->name != "root"){       //while you havent hit the root or a nullptr         
                path.push_back(nav->name);                      //push each parent into path
                nav= nav->parent;                               //move up in the tree
            }
            if(!path.empty()){                          //if path isn't empty in this process
                reverse(path.begin(),path.end());       //reverse this path
                conditional_pattern_base.push_back({count,path});       //push it into the pattern base   
            }
           curr=curr->same_next;                        //go to a node of the same type
        }
        for(const auto& [count,path]:conditional_pattern_base){         //go through the patterns you have
            for(const auto& item: path){                                //extract how many times each item appears in all of these patterns that included the current item
                conditional_item[item]+=count;                          
            }
        }
        vector<string> delete_items;                                    //go through the items
        for(const auto& [item,count]:conditional_item){                 //have any that didn't pass the minimum count be prepared for deletion
            if(count < min){
                delete_items.push_back(item);
            }
        }
        for(const string& key: delete_items){           //delete each item that didn't fulfill the min requirement
            conditional_item.erase(key);
        }
        vector<pair<int,vector<string>>> cleaned_pattern_base;          //this well represent the pattern base where all patterns fit themin requirement
         for(const auto& [count,path]:conditional_pattern_base){        //go through the pattern base 
            vector<string> cleaned_path;                                //vector representing cleaned path           
            for(const auto& item: path){
                unordered_map<string,int>::iterator it=conditional_item.find(item);     //see if the item was removed or not
                if(it != conditional_item.end()){           //if it wasnt removed save it into the cleaned path
                    cleaned_path.push_back(item);           //push it into cleaned path
                }
            }
            if(!cleaned_path.empty()){                  //if cleaned path isnt empty
                cleaned_pattern_base.push_back({count,cleaned_path});       //saved clean path with its count into the cleaned pattern base
            }
        } 
        if(cleaned_pattern_base.empty()) continue;                       //if at the end of this process there are no patterns somehow, skip to the next entry of the header table

        treenode* sub_root= new treenode{"root",0,nullptr,{},nullptr};          //start a sub tree and subtable
        unordered_map<string,HeaderEntry*> Sub_Table;
        for(int j=0; j < cleaned_pattern_base.size(); j++){                     //for each entry in the cleaned_pattern_base, insert each pattern into the subtree and subtable
            create_subtree(cleaned_pattern_base[j],Sub_Table,sub_root);
        }
        tree_breakdown(min,sub_root,Sub_Table,new_suffix,outfile);       //then recursively call this function to mine this subtree to get more patterns
        delete_tree(sub_root);                                          //delete the sub tree and table
        for(auto& [item,entry]:Sub_Table){
            delete entry;
        }
        Sub_Table.clear();

    }
}

