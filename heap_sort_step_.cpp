/*
Program : heap_sort_step.cpp
Course: CCP6214 Algorithm Design and Analysis
 Lecture Class: TC6L
 Tutorial Class: T21L
 Trimester: 2610
 Member_1: 242UC244L8 | VIDHYA DARINEY A/P RAJASINGAM | vidhya.dariney.rajasingam@student.mmu.edu.my | 0176506405
 Member_2: 242UC244PT | JASMYNE YAP | jasmyne.yap@student.mmu.edu.my | 01163464323
 Member_3: 242UC244CK | WAN HANANI IMAN BINTI WAN MOHD AZIDI | wan.hanani.iman@student.mmu.edu.my | 0199660664

 Task Distribution 
Member_1: Vidhya Dariney A/P Rajasingam - 242UC244L8
Implement Heap Sort (both the step-by-step version and the full sort)
Document heap sort's time/space complexity
Run experiments for heap sort on 10+ input sizes
Write the comparison conclusion between radix and heap sort

Member_2: Jasmyne Yap - 242UC244PT
Implement Hash Table Search (including the step-by-step version)
Document best/average/worst case times
Write the theory comparison between array-based AVL vs linked-list AVL
Run experiments for hash table search on 10+ input sizes

Member_3:  Wan Hanani Iman Binti Wan Mohd Azidi - 242UC244CK
Write the dataset generator program
Implement Radix Sort (both the step-by-step version and the full sort)
Document radix sort's time/space complexity
Run experiments for radix sort on 10+ input sizes

run code:
1. g++ -O3 heap_sort_step.cpp -o heap_sort_step
2. ./heap_sort_step

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct Record {
    long long id;    
    string key;      
};

// Helper function to maintain the Max-Heap property
void heapify(vector<Record>& arr, int n, int i) {
    int largest = i;       
    int left = 2 * i + 1;  
    int right = 2 * i + 2; 

    if (left < n && arr[left].id > arr[largest].id)
        largest = left;

    if (right < n && arr[right].id > arr[largest].id)
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Function to load the CSV dataset file
vector<Record> loadDataset(const string& filename) {
    vector<Record> dataset;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return dataset;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string id_str, key_str;
        
        if (getline(ss, id_str, ',') && getline(ss, key_str)) {
            Record rec;
            rec.id = stoll(id_str); 
            rec.key = key_str;
            dataset.push_back(rec);
        }
    }
    file.close();
    return dataset;
}

int main() {
    string sizeStr;
    int start_row, end_row;

    // --- INTERACTIVE TERMINAL PROMPTS ---
    cout << "==========================================" << endl;
    cout << "       HEAP SORT STEP-BY-STEP TRACER      " << endl;
    cout << "==========================================" << endl;
    
    cout << "Enter dataset size (e.g., 1000): ";
    cin >> sizeStr;
    
    cout << "Enter start row index (e.g., 0): ";
    cin >> start_row;
    
    cout << "Enter end row index (e.g., 5): ";
    cin >> end_row;
    cout << "------------------------------------------" << endl;

    string inputFilename = "dataset_" + sizeStr + ".csv";
    string outputFilename = "dataset_" + sizeStr + "_heap_sorted_step_" + to_string(start_row) + "_" + to_string(end_row) + ".txt";

    cout << "Loading dataset: " << inputFilename << "..." << endl;
    vector<Record> dataset = loadDataset(inputFilename);
    
    if (dataset.empty()) {
        return 1;
    }

    // Input bounds validation check
    if (start_row < 0 || end_row >= dataset.size() || start_row > end_row) {
        cerr << "Error: Invalid row ranges provided! Dataset size is " << dataset.size() << " rows." << endl;
        return 1;
    }

    ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        cerr << "Error: Could not create step output file " << outputFilename << endl;
        return 1;
    }

    int n = dataset.size();

    // Step 1: Build the Max-Heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(dataset, n, i);
    }

    cout << "Max-Heap built successfully. Recording execution steps..." << endl;

    int step_counter = 1;
    // Step 2: One by one extract elements and trace specified rows
    for (int i = n - 1; i > 0; i--) {
        swap(dataset[0], dataset[i]);
        heapify(dataset, i, 0);

        // Print the header for the current tracking step
        outFile << "Step " << step_counter << ":\n";

        // Print data only within the requested row boundary slice
        for (int r = start_row; r <= end_row; r++) {
            outFile << dataset[r].id << "," << dataset[r].key << "\n";
        }
        outFile << "---------------------------------------\n";
        step_counter++;
    }

    outFile.close();
    cout << "\n>>> Tracking complete! Output saved to: " << outputFilename << " :>" << endl;

    return 0;
}