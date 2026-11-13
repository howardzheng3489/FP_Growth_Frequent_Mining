#include <iostream>
#include <string.h>
#include <unordered_map>
#include <fstream>   
#include <sstream>   
#include <algorithm>
#include <stdexcept>
#include "Preprocessor.hpp"

using namespace std;
string bin( double num, double quartile1, double quartile2){
    if(num <= quartile1){
        return "Low";
    }
    else if(num <= quartile2){
        return "Medium";
    }
    else{
        return "High";
    }
}


string bin_int( int num, int quartile1, int quartile2){
    if(num <= quartile1){
        return "Low";
    }
    else if(num <= quartile2){
        return "Medium";
    }
    else{
        return "High";
    }
}
int get_total_instances(const string& filename){
    ifstream infile(filename);
    if(!infile.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return 0;
    }
    int number_of_lines=0;
    string line;
    while(getline(infile,line)){            //count number of lines 
        number_of_lines++;
    }
    return number_of_lines;             //return the number of lines

}

void process_validation(const string& filename){            //create a simple small database
    ofstream outfile(filename);
    outfile << "X,Y,A,Z\nY,Z,X,W\nY,X,W,Z\nX,Y,W,Z";
    outfile.close();
    return;
}

void preprocess_cars(const string& filename,const string& outfilename){
    ifstream infile(filename);            //creates a stream of the file
    if(!infile.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    vector<string> attributes={             //represents the feature of each column
        "Buying_Price",
        "Maintenance_Price",
        "#_of_Doors",
        "Passenger_Capacity",
        "Lug_Boot",
        "Safety"
    };
    ofstream outfile(outfilename);          //prepare the output file
    string line;
    int current_charcteristic=0;
    while(getline(infile,line)){            //go through each line of the file
        current_charcteristic=0;
        stringstream ss(line);
        string cell;
        while(getline(ss,cell,',')){        //go through each item of the line
            if(current_charcteristic < attributes.size()){          //print each item with the attribute listed for each column
                outfile << attributes[current_charcteristic] << "_" << cell <<" ";
                current_charcteristic++;
            }
        }
        outfile << "\n";
    }
    outfile.close();
    infile.close();
}

void preprocess_mushroom(const string& filename,const string& outfilename){
    ifstream infile(filename);            //creates a stream of the file
    if(!infile.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    vector<string> characteristics={            //each feature of mushroom
        "Edibility",
        "Cap-shape",
        "Cap-surface",
        "Cap-color",
        "Bruises",
        "Odor",
        "Gill-Attachment",
        "Gill-Spacing",
        "Gill-size",
        "Gill-Color",
        "Stalk-Shape",
        "Stalk-root",
        "Stalk-surface-above-ring",
        "Stalk-surface-below-ring",
        "Stalk-color-above-ring",
        "Stalk-color-below-ring",
        "Veil-type",
        "Veil-color",
        "Ring-number",
        "Ring-type",
        "Spore-Print-Color",
        "Population",
        "Habitat"
    };
    ofstream outfile(outfilename);
    string line;
    int current_charcteristic=0;        //says the current feature of the current column
    while(getline(infile,line)){        //read each line of the file
        current_charcteristic=0;
        string characteristic;
        stringstream ss(line);
        while(getline(ss,characteristic,',')){        //as long as character go into mushroom
            if(characteristic == "?"){      //if attribute is missing skip it
                current_charcteristic++;
                continue;
            }
            if(current_charcteristic < characteristics.size()) {            //otherwise print the parameter
                outfile << characteristics[current_charcteristic] <<"_"<< characteristic << " ";
                current_charcteristic++;

            }
            
        }
        outfile <<"\n";
    }
    outfile.close();
    infile.close();
}


void preprocess_chess_2(const string& filename,const string& outfilename){
     ifstream file(filename);            //creates a stream of the file
    if(!file.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
     vector<string> characteristics={
       "bkblk","bknwy","bkon8","bkona","bkspr","bkxbq","bkxcr","bkxwp","blxwp","bxqsq","cntxt","dsopp","dwipd",
        "hdchk","katri","mulch","qxmsq","r2ar8","reskd","reskr","rimmx","rkxwp","rxmsq","simpl","skach","skewr",
        "skrxp","spcop","stlmt","thrsk","wkcti","wkna8","wknck","wkovl","wkpos","wtoeg","result"
     };
     ofstream outfile(outfilename);
     string line;
     int index=0;
    while(getline(file,line)){
        string cell;
        stringstream ss(line);
        index=0;
        while(getline(ss,cell,',')){

            outfile << characteristics[index] <<"_" <<cell <<" ";
            index++;
        }
        outfile<< "\n";
    }
    outfile.close();
    file.close();
}




void preprocess_abalone(const string& filename,const string& outfilename){
    ifstream file(filename);            //creates a stream of the file
    if(!file.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    vector<vector<double>> columns;     //vector of doubles for each column of the data, each column hopefully representing a different attribute
    string line;                        //each line of the data
    
    vector<string> labels;              //for entries in data that weren't numbers
    while(getline(file,line)){          //process the file line by line
        string cell;                    //represents the current entry
        stringstream ss(line);          //puts the line in a stream
        int col_index=0;                //an index representing the current column
        while(getline(ss,cell,',')){        //go through each entry of the line, it specifically stops at commas as that would be the end of the current entry
           try {        
                double val = stod(cell);           //try to convert the current entry into a double
                if (col_index >= columns.size()) {      //if that's successful and col_index is greater than the size of columns
                    columns.push_back({});              //push a empty vector into columns
                }
                columns[col_index].push_back(val);      //then push the current value into the current column
                col_index++;                            //increment col_index
            } 
            catch (const invalid_argument&) {      //if the conversion to double didn't work
                labels.push_back(cell);                 //that means it was a letter/word so push it into another vector called vectors
            } 
            catch (const out_of_range&) {          //if the number is out of bounds skip it
            }
        }
    }
    vector<pair<double,double>> quartiles;          //a vector with pairs of number, with the first number representing the first quartile and the other representing the second
    for(int i=0; i<columns.size(); i++){            //go through the columns 2d vector
        vector<double> curr_col=columns[i];         //for each column
        sort(curr_col.begin(),curr_col.end());      //sort the column
        int col_size=columns[i].size();             //extract the size of column
        quartiles.push_back({curr_col[col_size/3],curr_col[col_size/3 * 2]});       //take the number size/3 and 2*size/3 representing the 2 quartiles
    }
    const vector<string> feature_names = {              //these are the feature names that will be used to replace the numbers, this changes depending on the file
        "Length", 
        "Diameter", 
        "Height", 
        "Whole_Weight",
        "Shucked_Weight",
        "Viscera_Weight",
        "Shell_Weight",
        "Rings"
    };
    ofstream outfile(outfilename);                      //now make a stream for the output file
    for(int i=0; i<columns[0].size(); i++){             //go through the columns     
        outfile <<labels[i]<<" ";                       //print label first
        for(int j=0; j<columns.size(); j++){            //go through the row

           string level=bin(columns[j][i],quartiles[j].first,quartiles[j].second);      //determine if the current level is low, medium or high
           outfile <<feature_names[j]<<"_"<< level <<" ";                               //print out the entry
        }
        outfile <<"\n";                     //indent
    }
    outfile.close();
    file.close();

}

void preprocess_wine(const string& filename,const string& outfilename){
     ifstream infile(filename);            //creates a stream of the file
    if(!infile.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    vector<vector<double>> columns;     //vector of doubles for each column of the data, each column hopefully representing a different attribute
    vector<int> wine_class;
    string line;                        //each line of the data
    while(getline(infile,line)){
        string cell;                    //represents the current entry
        stringstream ss(line);          //puts the line in a stream
        int col_index=0;                //an index representing the current column
        while(getline(ss,cell,',')){
            if(col_index == 0){
                int w_class = stoi(cell);
                wine_class.push_back(w_class);
                col_index++;
            }
            else{
                double val = std::stod(cell);           //try to convert the current entry into a double
                if (col_index - 1 >= columns.size()) {      //if that's successful and col_index is greater than the size of columns
                    columns.push_back({});              //push a empty vector into columns
                }
                columns[col_index - 1].push_back(val);      //then push the current value into the current column
                col_index++;                            //increment col_index

            }
        }
    }
     vector<pair<double,double>> quartiles;          //a vector with pairs of number, with the first number representing the first quartile and the other representing the second
        for(int i=0; i<columns.size(); i++){            //go through the columns 2d vector
            vector<double> curr_col=columns[i];         //for each column
            sort(curr_col.begin(),curr_col.end());      //sort the column
            int col_size=columns[i].size();             //extract the size of column
            quartiles.push_back({curr_col[col_size * 0.33],curr_col[col_size * 0.66]});       //take the number size/3 and 2*size/3 representing the 2 quartiles
        }
        const vector<string> feature_name={
            "Alcohol",
            "Malic_acid",
            "Ash",
            "Alaclinity_of_Ash",
            "Magnesium",
            "Total_Phenols",
            "Flavanoids",
            "Nonflavanoid_Phenols",
            "Proanthocyanins",
            "Color_intensity",
            "Hue",
            "OD280/315_Of_Diluted",
            "Proline"
        };
    ofstream outfile(outfilename);                      //now make a stream for the output file
     for(int i=0; i<columns[0].size(); i++){             //go down the columns     
        outfile <<"Class_"<<wine_class[i]<<" ";                       //print label first
        for(int j=0; j<columns.size(); j++){            //go through the row
           string level=bin(columns[j][i],quartiles[j].first,quartiles[j].second);      //determine if the current level is low, medium or high
           outfile <<feature_name[j]<<"_"<< level <<" ";                               //print out the entry
        }
        outfile <<"\n";                     //indent
    }
    outfile.close();
    infile.close();
}


void preprocess_iris(const string& filename,const string& outfilename){
    ifstream file(filename);            //creates a stream of the file
    if(!file.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    vector<vector<double>> columns;     //vector of doubles for each column of the data, each column hopefully representing a different attribute
    string line;                        //each line of the data
    
    vector<string> labels;              //for entries in data that weren't numbers
    while(getline(file,line)){          //process the file line by line
        string cell;                    //represents the current entry
        stringstream ss(line);          //puts the line in a stream
        int col_index=0;                //an index representing the current column
        while(getline(ss,cell,',')){        //go through each entry of the line, it specifically stops at commas as that would be the end of the current entry
           try {        
                double val = stod(cell);           //try to convert the current entry into a double
                if (col_index >= columns.size()) {      //if that's successful and col_index is greater than the size of columns
                    columns.push_back({});              //push a empty vector into columns
                }
                columns[col_index].push_back(val);      //then push the current value into the current column
                col_index++;                            //increment col_index
            } 
            catch (const invalid_argument&) {      //if the conversion to double didn't work
                labels.push_back(cell);                 //that means it was a letter/word so push it into another vector called vectors
            } 
            catch (const out_of_range&) {          //if the number is out of bounds skip it
            }
        }
    }
    vector<pair<double,double>> quartiles;          //a vector with pairs of number, with the first number representing the first quartile and the other representing the second
    for(int i=0; i<columns.size(); i++){            //go through the columns 2d vector
        vector<double> curr_col=columns[i];         //for each column
        sort(curr_col.begin(),curr_col.end());      //sort the column
        int col_size=columns[i].size();             //extract the size of column
        quartiles.push_back({curr_col[col_size/3],curr_col[col_size/3 * 2]});       //take the number size/3 and 2*size/3 representing the 2 quartiles
    }
    const vector<string> feature_names = {              //these are the feature names that will be used to replace the numbers, this changes depending on the file
        "Sepal_length",
        "Sepal_width",
        "Petal_length",
        "Petal_width"
    };
    ofstream outfile(outfilename);                      //now make a stream for the output file
    for(int i=0; i<columns[0].size(); i++){             //go through the columns     
        for(int j=0; j<columns.size(); j++){            //go through the row
           string level=bin(columns[j][i],quartiles[j].first,quartiles[j].second);      //determine if the current level is low, medium or high
           outfile <<feature_names[j]<<"_"<< level <<" ";                               //print out the entry
        }
        outfile <<labels[i]<<"\n";                     //indent
    }
    outfile.close();
    file.close();
}

void preprocess_skinsegmentation(const string& filename,const string& outfilename){
     ifstream infile(filename);            //creates a stream of the file
    if(!infile.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    ofstream outfile(outfilename);
      if(!outfile.is_open()){                //if file doesn't exist
        cerr <<"Output File failed to open" <<std::endl;
        return;
    }
    string line;
    vector<vector<int>> columns;
    int val;       
    while(getline(infile,line)){
        stringstream ss(line);          //puts the line in a stream
        int col_index=0;                //an index representing the current column
        while(ss >> val){
            if (col_index >= columns.size()) {      //if that's successful and col_index is greater than the size of columns
                columns.push_back({});              //push a empty vector into columns
            }
            columns[col_index].push_back(val);      //then push the current value into the current column
            col_index++;                            //increment col_index
        }
    }
    vector<pair<int,int>> quartiles;          //a vector with pairs of number, with the first number representing the first quartile and the other representing the second
    for(int i=0; i<3; i++){            //go through the columns 2d vector
        vector<int> curr_col=columns[i];         //for each column
        sort(curr_col.begin(),curr_col.end());      //sort the column
        int col_size=columns[i].size();             //extract the size of column
        quartiles.push_back({curr_col[col_size/3],curr_col[col_size/3 * 2]});       //take the number size/3 and 2*size/3 representing the 2 quartiles
    }

    vector<string> features={
        "B_",
        "G_",
        "R_",
        "Y_"
    };

    for(int i=0; i<columns[0].size(); i++){             //go through the columns     
        for(int j=0; j<3; j++){            //go through the row
           string level=bin_int(columns[j][i],quartiles[j].first,quartiles[j].second);      //determine if the current level is low, medium or high
           outfile <<features[j]<< level <<" ";                               //print out the entry
        }
        outfile <<features[3] << columns[3][i]<<"\n";                     //indent
    }
    outfile.close();
    infile.close();
    
}


void preprocess_sepsis(const string& filename,const string& outfilename){
    ifstream infile(filename);            //creates a stream of the file
    if(!infile.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    ofstream outfile(outfilename);
      if(!outfile.is_open()){                //if file doesn't exist
        cerr <<"Output File failed to open" <<std::endl;
        return;
    }
    vector<pair<string,bool>> integer_or_not={
        {"Age_",true},
        {"Sex_",false},
        {"Episodes_",true},
        {"Outcome_",false}
    };
    string dummy;
    getline(infile,dummy);
    string line;
    int index=0;
    unordered_map<int,vector<int>> continuous_values;
    while(getline(infile,line)){
        string cell;
        index=0;
        stringstream ss(line);
        while(getline(ss,cell,',')){
            if(integer_or_not[index].second){
                int val=stoi(cell);
                continuous_values[index].push_back(val);
            }
            index++;
        }
    }
    unordered_map<int,pair<int,int>> quartiles;
    for(auto& pair:continuous_values){
        std::sort(pair.second.begin(),pair.second.end());
        int col= pair.first;
        int size_vec=pair.second.size();
        if(size_vec == 0) continue;
        quartiles.insert({col,{pair.second[size_vec /3],pair.second[2* size_vec /3]}});
    }
    infile.clear();
    infile.seekg(0,std::ios::beg);
    getline(infile,dummy);
    while(getline(infile,line)){
        string cell;
        index=0;
        stringstream ss(line);
        while(getline(ss,cell,',')){
            int val = stoi(cell);
            if(integer_or_not[index].second){
                string level=bin_int(val,quartiles[index].first,quartiles[index].second);
                outfile << integer_or_not[index].first << level <<" ";
            }
            else{
                if(index == 1){
                    if(val == 1){
                       outfile << integer_or_not[index].first << "Female" <<" "; 
                    }
                    else{
                        outfile << integer_or_not[index].first << "Male" <<" "; 
                    }
                }
                else{
                    if(val == 1){
                       outfile << integer_or_not[index].first << "Alive" <<" "; 
                    }
                    else{
                        outfile << integer_or_not[index].first << "Death" <<" "; 
                    }
                }
            }
            index++;
        }
        outfile <<"\n";

    }
    infile.close();
    outfile.close();

}





void preprocess_connect4(const string& filename,const string& outfilename){
    ifstream infile(filename);            //creates a stream of the file
    if(!infile.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    ofstream outfile(outfilename);
      if(!outfile.is_open()){                //if file doesn't exist
        cerr <<"Output File failed to open" <<std::endl;
        return;
    }
    vector<string> positions={
        "a1","a2","a3", "a4", "a5", "a6",
        "b1","b2","b3", "b4", "b5", "b6",
        "c1","c2","c3", "c4", "c5", "c6",
        "d1","d2","d3", "d4", "d5", "d6",
        "e1","e2","e3", "e4", "e5", "e6",
        "f1","f2","f3", "f4", "f5", "f6",
        "g1","g2","g3", "g4", "g5", "g6", 
        "result"
    };
    string line;
    int index=0;
    while(getline(infile,line)){
        index=0;
        string cell;
        stringstream ss(line);
        while(getline(ss,cell,',')){
            outfile << positions[index] << "_" << cell <<" ";
            index++;
        }
        outfile << "\n";
    }
    infile.close();
    outfile.close();
    return;
}


void preprocess_chess(const string& filename,const string& outfilename){
    ifstream infile(filename);            //creates a stream of the file
    if(!infile.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    ofstream outfile(outfilename);
      if(!outfile.is_open()){                //if file doesn't exist
        cerr <<"Output File failed to open" <<std::endl;
        return;
    }
    vector<string> feature_names={
        "White_King_File",
        "White_King_rank",
        "White_Rook_File",
        "White_Rook_Rank",
        "Black_King_File",
        "Black_King_Rank",
        "Depth_of_Win"
    };
    string line;
    int index=0;
    while(getline(infile,line)){
        string cell;
        stringstream ss(line);
        index=0;
        while(getline(ss,cell,',')){
            outfile << feature_names[index] <<"_" << cell <<" ";
            index++;
        }
        outfile <<"\n";
    }
    outfile.close();
    infile.close();
}


void preprocess_adult(const string& filename,const string& outfilename){
    ifstream infile(filename);            //creates a stream of the file
    if(!infile.is_open()){                //if file doesn't exist
        cerr <<"File failed to open" <<std::endl;
        return;
    }
    vector<pair<string, int>> continuous_or_not={
        {"age",1},
        {"workclass",0},
        {"fnlwgt",1},
        {"education",0},
        {"education-num",1},
        {"marital-status",0},
        {"occupation",0},
        {"relationship",0},
        {"race",0},
        {"sex",0},
        {"capital-gain",1},
        {"capital-loss",1},
        {"hours-per-week",1},
        {"native-country",0},
        {"income",0}
    };     //vector of doubles for each column of the data, each column hopefully representing a different attribute
    string line;  
    int index=0;
    unordered_map<int,vector<double>> continuous_values;
    while(getline(infile,line)){
        string cell;                    //represents the current entry
        stringstream ss(line);          //puts the line in a stream
        index=0;
        while(getline(ss,cell,',')){
            const string whitespace = " \t\r\n";
            // 1. Find the first non-whitespace character
            size_t first = cell.find_first_not_of(whitespace);

            if (first == string::npos) {
                cell= ""; // The cell was empty or contained only spaces
            } else {
                // 2. Find the last non-whitespace character and trim
                size_t last= cell.find_last_not_of(whitespace);
                cell= cell.substr(first, (last - first + 1));
            }
            if(cell == "?" || cell == ""){
                index++; 
                continue;
            }
           if(continuous_or_not[index].second == 1){
               continuous_values[index].push_back(stod(cell));
           }
           index++;
        }
        
    }
    unordered_map<int,pair<double,double>> quartiles;
    for(auto& pair:continuous_values){
            std::sort(pair.second.begin(),pair.second.end());
            int size_vector=pair.second.size();
            if(size_vector == 0) continue;
            quartiles.insert({pair.first,{pair.second[size_vector/3],pair.second[size_vector * 2 / 3]}});
    }
    ofstream outfile(outfilename);
    infile.clear();
    infile.seekg(0,std::ios::beg);
    while(getline(infile,line)){
        string cell;                    //represents the current entry
        stringstream ss(line);          //puts the line in a stream
        index=0;
        while(getline(ss,cell,',')){
            const string whitespace = " \t\r\n";
            // 1. Find the first non-whitespace character
            size_t first = cell.find_first_not_of(whitespace);

            if (first == string::npos) {
                cell= ""; // The cell was empty or contained only spaces
            } else {
                // 2. Find the last non-whitespace character and trim
                size_t last= cell.find_last_not_of(whitespace);
                cell= cell.substr(first, (last - first + 1));
            }
            if(cell == "?" || cell == ""){
                index++;
                continue;
            }

            if(continuous_or_not[index].second == 0){
                outfile<< continuous_or_not[index].first <<"_" << cell <<" ";
            }
            else{
                int val= stod(cell);
                string section=bin(val,quartiles[index].first,quartiles[index].second);
                outfile<< continuous_or_not[index].first <<"_" << section <<" ";

            }
            index++;
        }
        outfile <<"\n";
    }
    infile.close();
    outfile.close();
}