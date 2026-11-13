#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>   
#include <sstream>   
#include <algorithm>


using namespace std;
struct treenode{
    string name;
    int frequency;
    treenode* parent;
    unordered_map<string, treenode*> children;
    treenode* same_next;
};

struct HeaderEntry{
    int total_count = 0;
    treenode* head = nullptr;
};

void delete_tree(treenode* node);
void process_data(const string& filename, int min,unordered_map<string, int>& item_counts);
void insert_path(vector<string>& transactions,treenode* root,unordered_map<string, HeaderEntry*>& Header_Table);
treenode* process_data_2
(const string& filename, int min,const unordered_map<string, int>& item_counts,unordered_map<string, HeaderEntry*>& Header_Table);
void tree_breakdown
(int min, treenode* root, unordered_map<string, HeaderEntry*>& Header_Table,vector<string> current_suffix,ofstream& outfile);
