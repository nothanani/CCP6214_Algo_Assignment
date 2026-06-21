/*********************************************************************************************
Program : hash_table_search_step.cpp
Course: CCP6214 Algorithm Design and Analysis
Lecture Class: TC6L
Tutorial Class: T21L
Trimester: 2610
Member_1: 242UC244PT | JASMYNE YAP | jasmyne.yap@student.mmu.edu.my | 01163464323
Member_2: 242UC244L8 | VIDHYA DARINEY A/P RAJASINGAM | vidhya.dariney.rajasingam@student.mmu.edu.my | 0176506405
Member_3: 242UC244CK | WAN HANANI IMAN BINTI WAN MOHD AZIDI | wan.hanani.iman@student.mmu.edu.my | 0199660664
********************************************************************************************
Task Distribution
Member_1: Jasmyne Yap - 242UC244PT
Implement Hash Table Search (including the step-by-step version)
Document best/average/worst case times
Write the theory comparison between array-based AVL vs linked-list AVL
Run experiments for hash table search on 10+ input sizes

Member_2: Vidhya Dariney A/P Rajasingam - 242UC244L8
Implement Heap Sort (both the step-by-step version and the full sort)
Document heap sort's time/space complexity
Run experiments for heap sort on 10+ input sizes
Write the comparison conclusion between radix and heap sort

Member_3:  Wan Hanani Iman Binti Wan Mohd Azidi - 242UC244CK
Write the dataset generator program
Implement Radix Sort (both the step-by-step version and the full sort)
Document radix sort's time/space complexity
Run experiments for radix sort on 10+ input sizes
********************************************************************************************
how to run!!
- be sure to be in the same directory as this file, then follow the steps below:
1. compile (command : g++ hash_table_search_step.cpp -o hash_table_step -std=c++17 -O3)
2. run (command : ./hash_table_search_step <dataset_size>)
********************************************************************************************
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Structure to hold dataset records
struct Record {
    long long number = -1; // -1 indicates an empty slot in the hash table
    string str = "";
};

class HashTable {
private:
    vector<Record> table;
    int size;

    // Modulo hash function
    int hashFunction(long long key) {
        return key % size;
    }

public:
    // Initialize hash table size to 2x the dataset to maintain a 0.5 load factor
    HashTable(int n) {
        size = n * 2;
        table.resize(size);
    }

    // Insert using Linear Probing
    void insert(long long key, string val) {
        int index = hashFunction(key);
        while (table[index].number != -1) {
            index = (index + 1) % size; // Wrap around if needed
        }
        table[index].number = key;
        table[index].str = val;
    }

    // Step-by-step search that logs the search path to a text file
    void searchStepByStep(long long target, const string& baseFilename) {
        string outFilename = baseFilename + "_hash_table_search_step_" + to_string(target) + ".txt";
        ofstream outFile(outFilename);
        
        if (!outFile.is_open()) {
            cerr << "Failed to open output file: " << outFilename << endl;
            return;
        }

        int index = hashFunction(target);
        int start_index = index;
        bool found = false;

        // Traverse using linear probing
        while (table[index].number != -1) {
            if (table[index].number == target) {
                // Target found format
                outFile << target << " = " << table[index].number << "/" << table[index].str << endl;
                found = true;
                break;
            } else {
                // Logging the search path for collisions
                outFile << table[index].number << " != " << target << endl;
            }
            index = (index + 1) % size;
            
            // Break to avoid infinite loop if table is completely full and element not found
            if (index == start_index) break; 
        }

        if (!found) {
            // Target not found format
            outFile << "-1 != " << target << endl;
        }

        outFile.close();
        cout << "Search step output written to: " << outFilename << endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <dataset_n.csv>" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open dataset: " << filename << endl;
        return 1;
    }

    // Count lines to determine dataset size
    int n = 0;
    string line;
    vector<pair<long long, string>> raw_data;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string numStr, str;
        getline(ss, numStr, ',');
        getline(ss, str, ',');
        raw_data.push_back({stoll(numStr), str});
        n++;
    }
    file.close();

    // Initialize and populate Hash Table
    HashTable ht(n);
    for (const auto& record : raw_data) {
        ht.insert(record.first, record.second);
    }

    // Extract base filename (e.g., "dataset_1000" from "dataset_1000.csv")
    string baseName = filename;
    size_t dotPos = baseName.find_last_of(".");
    if (dotPos != string::npos) baseName = baseName.substr(0, dotPos);

    // =====================================================================
    // DEMO VARIABLES: Edit these target values for the Demo
    
    // =====================================================================
    // Pick an existing key from the dataset for the "Found" demo
    long long target_found = raw_data[0].first; 
    
    // Pick a key guaranteed not to be in the dataset (e.g. 11 digits)
    long long target_not_found = 123456789LL; 

    cout << "Executing step-by-step search for FOUND target: " << target_found << endl;
    ht.searchStepByStep(target_found, baseName);

    cout << "Executing step-by-step search for NOT FOUND target: " << target_not_found << endl;
    ht.searchStepByStep(target_not_found, baseName);

    return 0;
}