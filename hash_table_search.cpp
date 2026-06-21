/* 
Program: hash_table_search.cpp
How to run:
g++ hash_table_search.cpp -o hash_table_search -std=c++17 -O3
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

    // Standard search returning boolean. No I/O inside to ensure accurate benchmarking.
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

    // Helper: Finds the start index of the longest continuous cluster to force Worst Case
    int getWorstCaseHashIndex() {
        int maxCluster = 0, currentCluster = 0;
        int bestStart = 0, currentStart = -1;

        // Loop twice to handle clusters wrapping around the end of the array
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
    vector<long long> raw_keys; // We only need keys for searching
    
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
        // Dummy string, since we only benchmark integer search time
        ht.insert(raw_keys[i], "abcde"); 
    }

    // ==========================================
    // 1. BEST CASE PREPARATION
    // ==========================================
    // Search for a key that we know has exactly 0 collisions (e.g., the very first item inserted)
    long long bestCaseKey = raw_keys[0]; 

    auto startBest = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        ht.search(bestCaseKey); // Perform n searches
    }
    auto endBest = chrono::high_resolution_clock::now();
    chrono::duration<double> timeBest = endBest - startBest;

    // ==========================================
    // 2. AVERAGE CASE PREPARATION
    // ==========================================
    // Search for every single key in the dataset exactly once (Total: n searches)
    auto startAvg = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        ht.search(raw_keys[i]); 
    }
    auto endAvg = chrono::high_resolution_clock::now();
    chrono::duration<double> timeAvg = endAvg - startAvg;

    // ==========================================
    // 3. WORST CASE PREPARATION
    // ==========================================
    // Force the search to probe through the absolute longest cluster in the table before failing.
    int worstHashIndex = ht.getWorstCaseHashIndex();
    
    // Construct a non-existent key that deliberately hashes to the worstHashIndex
    // Hash function is `key % size`, so `(size * arbitrary_large_number) + worstHashIndex` works.
    long long worstCaseKey = ((long long)ht.size * 1000000LL) + worstHashIndex; 

    auto startWorst = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        ht.search(worstCaseKey); // Perform n searches
    }
    auto endWorst = chrono::high_resolution_clock::now();
    chrono::duration<double> timeWorst = endWorst - startWorst;

    // ==========================================
    // PRINT OUTPUTS
    // ==========================================
    
    // Extract base filename
    string baseName = filename;
    size_t dotPos = baseName.find_last_of(".");
    if (dotPos != string::npos) baseName = baseName.substr(0, dotPos);
    
    string outFilename = "hash_table_search_" + baseName + ".txt";
    ofstream outFile(outFilename);
    
    // Write to File
    outFile << "Best case time: " << fixed << timeBest.count() << " seconds\n";
    outFile << "Average case time: " << fixed << timeAvg.count() << " seconds\n";
    outFile << "Worst case time: " << fixed << timeWorst.count() << " seconds\n";
    outFile.close();

    // Print to Command Prompt Window (Required for screenshots)
    cout << "Dataset Size (n): " << n << endl;
    cout << "Best case time: " << fixed << timeBest.count() << " seconds" << endl;
    cout << "Average case time: " << fixed << timeAvg.count() << " seconds" << endl;
    cout << "Worst case time: " << fixed << timeWorst.count() << " seconds" << endl;
    cout << "Running times written to: " << outFilename << endl;

    return 0;
}