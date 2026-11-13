#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>   
#include <sstream>  
#include <algorithm>

using namespace std;

string bin( double num, double quartile1, double quartile2);
int get_total_instances(const string& filename);
void process_validation(const string& filename);
void preprocess_skinsegmentation(const string& filename,const string& outfilename);
void preprocess_abalone(const string& filename,const string& outfilename);
void preprocess_iris(const string& filename,const string& outfilename);
void preprocess_mushroom(const string& filename,const string& outfilename);
void preprocess_wine(const string& filename,const string& outfilename);
void preprocess_cars(const string& filename,const string& outfilename);
void preprocess_chess(const string& filename,const string& outfilename);
void preprocess_adult(const string& filename,const string& outfilename);
void preprocess_connect4(const string& filename,const string& outfilename);
void preprocess_sepsis(const string& filename,const string& outfilename);
void preprocess_chess_2(const string& filename,const string& outfilename);
