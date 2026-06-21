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
1. compile (command : g++ hash_table_search.cpp -o hash_table_search -std=c++17 -O3)
2. run (command : ./hash_table_search <dataset_size>)
********************************************************************************************
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

struct Record {
    long long number = -1;
    string str = "";
};

class HashTable {
public:
    vector<Record> table;
    int size;

    HashTable(int n) {
        size = n * 2;
        table.resize(size);
    }

    int hashFunction(long long key) {
        return key % size;
    }

    void insert(long long key, string val) {
        int index = hashFunction(key);
        while (table[index].number != -1) {
            index = (index + 1) % size;
        }
        table[index].number = key;
        table[index].str = val;
    }

    bool search(long long target) {
        int index = hashFunction(target);
        int start_index = index;

        while (table[index].number != -1) {
            if (table[index].number == target) return true;
            index = (index + 1) % size;
            if (index == start_index) break;
        }
        return false;
    }

    int getWorstCaseHashIndex() {
        int maxCluster = 0, currentCluster = 0;
        int bestStart = 0, currentStart = -1;

        for (int i = 0; i < size * 2; i++) {
            int idx = i % size;
            if (table[idx].number != -1) {
                if (currentCluster == 0) currentStart = idx;
                currentCluster++;
                if (currentCluster > maxCluster) {
                    maxCluster = currentCluster;
                    bestStart = currentStart;
                }
            } else {
                currentCluster = 0;
            }
        }
        return bestStart;
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

    int n = 0;
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

    HashTable ht(n);
    for (int i = 0; i < n; i++) {
        ht.insert(raw_keys[i], "abcde"); 
    }

    // ==========================================
    // ANTI-OPTIMIZATION SAFEGUARD
    // ==========================================
    // We use a volatile variable to force the compiler to execute the loops.
    // It prevents 'Dead Code Elimination' caused by the -O3 flag.
    volatile int dummy_count = 0;

    // ==========================================
    // 1. BEST CASE PREPARATION
    // ==========================================
    long long bestCaseKey = raw_keys[0]; 

    auto startBest = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        if (ht.search(bestCaseKey)) dummy_count++;
    }
    auto endBest = chrono::high_resolution_clock::now();
    chrono::duration<double> timeBest = endBest - startBest;

    // ==========================================
    // 2. AVERAGE CASE PREPARATION
    // ==========================================
    auto startAvg = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        if (ht.search(raw_keys[i])) dummy_count++; 
    }
    auto endAvg = chrono::high_resolution_clock::now();
    chrono::duration<double> timeAvg = endAvg - startAvg;

    // ==========================================
    // 3. WORST CASE PREPARATION
    // ==========================================
    int worstHashIndex = ht.getWorstCaseHashIndex();
    long long worstCaseKey = ((long long)ht.size * 1000000LL) + worstHashIndex; 

    auto startWorst = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        if (ht.search(worstCaseKey)) dummy_count++;
    }
    auto endWorst = chrono::high_resolution_clock::now();
    chrono::duration<double> timeWorst = endWorst - startWorst;

    // ==========================================
    // PRINT OUTPUTS
    // ==========================================
    string baseName = filename;
    size_t dotPos = baseName.find_last_of(".");
    if (dotPos != string::npos) baseName = baseName.substr(0, dotPos);
    
    string outFilename = "hash_table_search_" + baseName + ".txt";
    ofstream outFile(outFilename);
    
    // We force a 6-decimal precision specifically for Windows/Powershell formatting
    outFile.precision(6);
    outFile << fixed;
    outFile << "Best case time: " << timeBest.count() << " seconds\n";
    outFile << "Average case time: " << timeAvg.count() << " seconds\n";
    outFile << "Worst case time: " << timeWorst.count() << " seconds\n";
    outFile.close();

    cout.precision(6);
    cout << fixed;
    cout << "Dataset Size (n): " << n << endl;
    cout << "Best case time: " << timeBest.count() << " seconds" << endl;
    cout << "Average case time: " << timeAvg.count() << " seconds" << endl;
    cout << "Worst case time: " << timeWorst.count() << " seconds" << endl;
    cout << "Running times written to: " << outFilename << "\n" << endl;

    return 0;
}