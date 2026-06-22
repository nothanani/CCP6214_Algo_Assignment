/*********************************************************************************************
Program : hash_table_search.cpp
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
Run experiments for two sorting algorithms (radix sort and heap sort) and one searching algorithm (hash table search) on 10+ input sizes

Member_2: Vidhya Dariney A/P Rajasingam - 242UC244L8
Implement Heap Sort (both the step-by-step version and the full sort)
Document heap sort's time/space complexity
Run experiments for two sorting algorithms (radix sort and heap sort) and one searching algorithm (hash table search) on 10+ input sizes
Write the comparison conclusion between radix and heap sort

Member_3:  Wan Hanani Iman Binti Wan Mohd Azidi - 242UC244CK
Write the dataset generator program
Implement Radix Sort (both the step-by-step version and the full sort)
Document radix sort's time/space complexity
Run experiments for two sorting algorithms (radix sort and heap sort) and one searching algorithm (hash table search) on 10+ input sizes
********************************************************************************************
how to run!!
- be sure to be in the same directory as this file, then follow the steps below:
1. compile (command : g++ hash_table_search.cpp -o hash_table_search -std=c++17 -O3)
2. run (command : ./hash_table_search <dataset filename>)
********************************************************************************************
*/

// This program reads a dataset file, stores the keys in a hash table,
// and measures the time needed for best, average, and worst-case searches.
// It also generates a secondary output file containing exactly 10% of the dataset keys.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <cmath>

using namespace std;

// Each dataset row stores a numeric key and its string value.
struct Record {
    long long number = -1;
    string str = "";
};

// Hash table using open addressing with linear probing.
class HashTable {
public:
    vector<Record> table;
    long long size;

    // Allocate a table that is larger than the dataset size.
    HashTable(long long n) {
        size = n * 2;
        table.resize(size);
    }

    // Simple modulo-based hash function. Uses abs() to safely handle negative keys.
    long long hashFunction(long long key) {
        return abs(key) % size;
    }

    // Insert a record into the table by probing forward until an empty slot is found.
    void insert(long long key, string val) {
        long long index = hashFunction(key);
        while (table[index].number != -1) {
            index = (index + 1) % size;
        }
        table[index].number = key;
        table[index].str = val;
    }

    // Search for a key using linear probing.
    bool search(long long target) {
        long long index = hashFunction(target);
        long long start_index = index;

        while (table[index].number != -1) {
            if (table[index].number == target) return true;
            index = (index + 1) % size;
            if (index == start_index) break;
        }
        return false;
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

    // Read all keys from the CSV file.
    long long n = 0;
    string line;
    vector<long long> raw_keys;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string numStr, str;
        getline(ss, numStr, ',');
        getline(ss, str, ',');
        raw_keys.push_back(stoll(numStr));
        n++;
    }
    file.close();

    // Build the hash table using the loaded dataset.
    HashTable ht(n);
    for (long long i = 0; i < n; i++) {
        ht.insert(raw_keys[i], "abcde"); 
    }

    // Determine 10% of the dataset size for the search cases.
    long long ten_percent = n / 10;
    if (ten_percent == 0) ten_percent = 1; 

    // Prepare the 3 search arrays to hold the keys for each case.
    vector<long long> best_keys;
    vector<long long> avg_keys;
    vector<long long> worst_keys;

    // Setup random number generators for average and worst cases.
    mt19937 rng(12345); 
    uniform_int_distribution<long long> distPos(1000000000LL, 9999999999LL);
    uniform_int_distribution<long long> distNeg(-9999999999LL, -1000000000LL);

    for (long long i = 0; i < ten_percent; i++) {
        best_keys.push_back(raw_keys[i]);      // Best Case: 10% original keys
        avg_keys.push_back(distPos(rng));      // Average Case: random 10-digit positive keys
        worst_keys.push_back(distNeg(rng));    // Worst Case: random 10-digit negative keys
    }

    // Arrays to store the search results (FOUND / NOT FOUND) without printing during the timer.
    vector<bool> best_results(ten_percent);
    vector<bool> avg_results(ten_percent);
    vector<bool> worst_results(ten_percent);

    // ==========================================
    // 1. BEST CASE MEASUREMENT
    //    Search using 10% of the original keys.
    // ==========================================
    auto startBest = chrono::high_resolution_clock::now();
    for (long long i = 0; i < ten_percent; i++) {
        best_results[i] = ht.search(best_keys[i]);
    }
    auto endBest = chrono::high_resolution_clock::now();
    chrono::duration<double> timeBest = endBest - startBest;

    // ==========================================
    // 2. AVERAGE CASE MEASUREMENT
    //    Search using random positive 10-digit keys.
    // ==========================================
    auto startAvg = chrono::high_resolution_clock::now();
    for (long long i = 0; i < ten_percent; i++) {
        avg_results[i] = ht.search(avg_keys[i]);
    }
    auto endAvg = chrono::high_resolution_clock::now();
    chrono::duration<double> timeAvg = endAvg - startAvg;

    // ==========================================
    // 3. WORST CASE MEASUREMENT
    //    Search using random negative keys guaranteed to cause full probing.
    // ==========================================
    auto startWorst = chrono::high_resolution_clock::now();
    for (long long i = 0; i < ten_percent; i++) {
        worst_results[i] = ht.search(worst_keys[i]);
    }
    auto endWorst = chrono::high_resolution_clock::now();
    chrono::duration<double> timeWorst = endWorst - startWorst;

    // ==========================================
    // OUTPUT FILE 1: THE RUNNING TIMES
    // ==========================================
    string baseName = filename;
    size_t dotPos = baseName.find_last_of(".");
    if (dotPos != string::npos) baseName = baseName.substr(0, dotPos);
    
    string file1Name = "hash_table_times_" + baseName + ".txt";
    ofstream outFile1(file1Name);
    
    outFile1.precision(6);
    outFile1 << fixed;
    outFile1 << "--- RUNNING TIMES (for " << ten_percent << " searches) ---\n";
    outFile1 << "Best case time: " << timeBest.count() << " seconds\n";
    outFile1 << "Average case time: " << timeAvg.count() << " seconds\n";
    outFile1 << "Worst case time: " << timeWorst.count() << " seconds\n";
    outFile1.close();

    // ==========================================
    // OUTPUT FILE 2: THE 10% DATA & RESULTS
    // ==========================================
    string file2Name = "hash_table_10percent_data_" + baseName + ".txt";
    ofstream outFile2(file2Name);

    outFile2 << "--- SEARCH RESULTS (Best Case: 10% Original Keys) ---\n";
    for(long long i = 0; i < ten_percent; i++) {
        outFile2 << best_keys[i] << " : " << (best_results[i] ? "FOUND" : "NOT FOUND") << "\n";
    }

    outFile2 << "\n--- SEARCH RESULTS (Average Case: 10% Random Positive Keys) ---\n";
    for(long long i = 0; i < ten_percent; i++) {
        outFile2 << avg_keys[i] << " : " << (avg_results[i] ? "FOUND" : "NOT FOUND") << "\n";
    }

    outFile2 << "\n--- SEARCH RESULTS (Worst Case: 10% Negative Keys) ---\n";
    for(long long i = 0; i < ten_percent; i++) {
        outFile2 << worst_keys[i] << " : " << (worst_results[i] ? "FOUND" : "NOT FOUND") << "\n";
    }
    outFile2.close();

    // ==========================================
    // COMMAND PROMPT OUTPUT
    // ==========================================
    cout.precision(6);
    cout << fixed;
    cout << "Dataset Size (n): " << n << endl;
    cout << "Searches Performed per Case (10%): " << ten_percent << endl;
    cout << "Best case time: " << timeBest.count() << " seconds" << endl;
    cout << "Average case time: " << timeAvg.count() << " seconds" << endl;
    cout << "Worst case time: " << timeWorst.count() << " seconds" << endl;
    cout << "-> Created File 1 (Times): " << file1Name << endl;
    cout << "-> Created File 2 (10% Data): " << file2Name << "\n" << endl;

    return 0;
}