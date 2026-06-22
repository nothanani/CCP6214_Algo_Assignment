/*********************************************************************************************
Program : radix_sort.cpp
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
1. compile (command : g++ radix_sort.cpp -o radix_sort )
2. run (command : ./radix_sort <dataset_size>)
Example: ./radix_sort dataset_1000.csv 1 1000  
note :                                  ^^1 is start row, 1000 is end row 
********************************************************************************************
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

//A record holds one row from the CSV: integer + string
struct Record {
    long long num;
    string str;
};

// Estimate the working space used by the record vector (in bytes).
// Includes the vector's allocated capacity plus each string's heap allocation.
size_t estimateSpaceBytes(const vector<Record> &data) {
    size_t bytes = data.capacity() * sizeof(Record);
    for (const auto &r : data) {
        bytes += r.str.capacity();
    }
    return bytes;
}

//read all rows from a CSV file
vector<Record> readCSV(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        exit(1);
    }

    vector<Record> data;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string numStr, str;
        getline(ss, numStr, ',');
        getline(ss, str);

        Record r;
        r.num = stoll(numStr);
        r.str = str;
        data.push_back(r);
    }

    file.close();
    return data;
}

//get digit at position d (d=1 is rightmost, d=10 is leftmost for 10-digit numbers)
int getDigit(long long num, int d) {
    long long divisor = 1;
    for (int i = 1; i < d; i++) divisor *= 10;
    return (num / divisor) % 10;
}

// counting sort pass on digit position d
void countingSortByDigit(vector<Record> &data, int d) {
    int n = data.size();
    vector<Record> output(n);
    int count[10] = {0};

    //count occurrences of each digit
    for (int i = 0; i < n; i++) {
        count[getDigit(data[i].num, d)]++;
    }

    //cumulative count (prefix sum)
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    //build sorted output (right to left for stability)
    for (int i = n - 1; i >= 0; i--) {
        int digit = getDigit(data[i].num, d);
        output[count[digit] - 1] = data[i];
        count[digit]--;
    }

    for (int i = 0; i < n; i++) {
        data[i] = output[i];
    }
}

//LSD radix sort: 10 passes for 10-digit integers
void radixSort(vector<Record> &data) {
    for (int d = 1; d <= 10; d++) {
        countingSortByDigit(data, d);
    }
}

int main() {
    string filename;

    //Ask the user for the dataset filename instead of using command-line args
    cout << "Enter the dataset CSV filename (e.g. dataset_1000.csv): ";
    getline(cin, filename);

    //Read dataset (I/O not included in timing)
    vector<Record> data = readCSV(filename);
    long long n = data.size();

    if (data.empty()) {
        cerr << "Error: No data loaded from " << filename << ". Check the filename and try again." << endl;
        return 1;
    }

    cout << "Sorting " << filename << " (" << n << " elements)..." << endl;

    //START TIMING (excludes I/O) ----
    auto startTime = chrono::high_resolution_clock::now();

    radixSort(data);

    auto endTime = chrono::high_resolution_clock::now();
    //END TIMING ----

    chrono::duration<double> elapsed = endTime - startTime;
    double seconds = elapsed.count();

    //estimate working space used (measured AFTER sort, since vector size is unchanged)
    size_t estimatedSpace = estimateSpaceBytes(data);

    //build output filename: radix_sorted_dataset_n.csv
    string baseName = filename;
    size_t slashPos = baseName.find_last_of("/\\");
    if (slashPos != string::npos) baseName = baseName.substr(slashPos + 1);
    size_t dotPos = baseName.find_last_of(".");
    if (dotPos != string::npos) baseName = baseName.substr(0, dotPos);

    string outFilename = "radix_sorted_" + baseName + ".csv";

    //Write sorted output (I/O not included in timing)
    ofstream outFile(outFilename);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot open output file " << outFilename << endl;
        return 1;
    }

    for (const Record &r : data) {
        outFile << r.num << "," << r.str << "\n";
    }
    outFile.close();

    // Print running time to console (as required by assignment)
    cout << "Running time: " << seconds << " seconds" << endl;
    cout << "Estimated working space: " << estimatedSpace << " bytes" << endl;
    cout << "Sorted output written to: " << outFilename << endl;

    return 0;
}