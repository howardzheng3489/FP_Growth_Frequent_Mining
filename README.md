# FP_Growth_Frequent_Mining
FP Growth Algorithm Implemented in C++ with single-fast-path optimization and preprocessors to process specific databases in the UC Irvine Machine Repository

---
# Key Features
Two-Pass FP Tree Creation ('FPGrowth.cpp'):
  Pass 1 (process_data): Counts every unique item's frequency in the dataset and stores it into an unordered_map. 
  Items with frequencies below the minimum threshold is pruned
  
  Pass 2 (process_data_2) : Sorts the remaining items in the unordered map into a vector 
  and creates the FP Tree and Header Table with this sorted vector and a second pass through the database. Each node in the tree has a pointer
  to the children, parent, and also to a node with the same type of item. 
  
  Frequent Item Set Mining(tree_breakdown) : Using the Header Table, it goes from the bottom of the Header Table, 
  the entry with the least frequency and traverses upwards in the tree, mining frequent item sets by creating pattern bases that pass the minimum threshold. 
  After getting the current pattern base, the tree is broken down into a subtree based on the current pattern base which is mined further with a recursive 
  tree_breakdown call. For subtrees that are a single path (is_single_path = true) the tree breakdown skips recursively breaking down the tree and instead looks through all
  2^k-1 combinations of the single path tree that pass the threshold and prints that.
  
Preprocessors for Databases(Preprocessor.cpp):
  Includes 11 dedicated preprocessors which cleans missing values and bins continuous values into discrete values labeling "Low", "Medium", and "High" Values using
  33rd and 66th percentiles. 

# Supported UCI Datasets:
| Dataset | Preprocessing Function | Data Type & Handling | Links|
| :--- | :--- | :--- | :--- |
| **Iris** | preprocess_iris | Continuous attributes binned into `Low`/`Medium`/`High`|https://archive.ics.uci.edu/dataset/53/iris 
| **Wine** | preprocess_wine| Continuous chemical features binned into tertiles |https://archive.ics.uci.edu/dataset/109/wine 
| **Abalone** | preprocess_abalone| Continuous physical measurements binned into tertiles | https://archive.ics.uci.edu/dataset/1/abalone
| **Adult (Census Income)** | preprocess_adult | Mixed categorical & continuous features; skips `?` entries | https://archive.ics.uci.edu/dataset/2/adult
| **Skin Segmentation** |preprocess_skinsegmentation| RGB integer values binned into tertiles | https://archive.ics.uci.edu/dataset/229/skin+segmentation
| **Sepsis Survival** | preprocess_sepsis| Age/episodes binned; binary sex & outcome mapped to labels | https://archive.ics.uci.edu/dataset/827/sepsis+survival+minimal+clinical+records
| **Mushroom** | preprocess_mushroom| 22 categorical features; skips missing `?` attributes | https://archive.ics.uci.edu/dataset/73/mushroom
| **Car Evaluation** | preprocess_cars | 6 categorical attributes mapped to `Attribute_Value` | https://archive.ics.uci.edu/dataset/19/car+evaluation
| **Connect-4** | preprocess_connect4| 42 board positions (`a1`–`g6`) + game outcome| https://archive.ics.uci.edu/dataset/26/connect+4
| **Chess (KRK)** | preprocess_chess| King/Rook file & rank coordinates + depth of win| https://archive.ics.uci.edu/dataset/23/chess+king+rook+vs+king
| **Chess (KR-vs-KP)** | preprocess_chess_2| 36 board state features + match result| https://archive.ics.uci.edu/dataset/22/chess+king+rook+vs+king+pawn



# How to run the program
In main.cpp, set your input dataset, the corresponding preprocessor, and minimum threshold (e.g. instances* 0.3 for 30% support)
> **Note:** Peak memory tracking in `main.cpp` uses `_msize` (`<malloc.h>`), which targets **Windows (MSVC or MinGW)**
Example:
(For iris dataset with 10% support)
```cpp
 string inputfile = "iris.data";
 string preprocessed_file = "processed_iris.txt";
 string outputfile_name = "mined_iris.txt";
 ofstream outfile(outputfile_name);
 int instances=get_total_instances(inputfile);
 int min= instances*0.1;

