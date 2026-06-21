/*********************************************************************************************
Program : heap_sort.cpp
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
1. compile (command : g++ -O3 heap_sort.cpp -o heap_sort )
2. run (command : ./heap_sort <dataset size>)
3. dataset will be sorted
********************************************************************************************
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

struct Record // data
{ 
    long long id; // 10 digits
    string key; // 5 letters
};

// maxheap
void heapify(vector<Record>& arr, int n, int i) 
{
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

// heapsort
void heapSort(vector<Record>& arr) 
{
    int n = arr.size();

    // apply maxheap
    for (int i = n / 2 - 1; i >= 0; i--) 
    {
        heapify(arr, n, i);
    }

    // move largest to the end, resort, rine and repeat
    for (int i = n - 1; i > 0; i--) 
    {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);

    }

}

// handle dataset
vector<Record> loadDataset(const string& filename) 
{
    vector<Record> dataset;
    ifstream file(filename);
    
    if (!file.is_open()) 
    {
        cerr << "Error: Could not open file " << filename << endl;
        return dataset;
    }

    string line;
    while (getline(file, line)) 
    {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string id_str, key_str;
        
        if (getline(ss, id_str, ',') && getline(ss, key_str)) 
        {
            Record rec;
            rec.id = stoll(id_str);
            rec.key = key_str;
            dataset.push_back(rec);
        }

    }

    file.close();
    return dataset;
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        cerr << "Usage: " << argv[0] << " <dataset_size>" << endl;
        return 1;
    }

    string sizeStr = argv[1];
    string inputFilename = "dataset_" + sizeStr + ".csv";
    string outputFilename = "heap_sort_dataset_" + sizeStr + ".csv";

    cout << "Loading dataset: " << inputFilename << "..." << endl;
    vector<Record> dataset = loadDataset(inputFilename);
    
    if (dataset.empty()) 
    {
        return 1;
    }

    cout << "Dataset loaded successfully. Commencing Heap Sort..." << endl;

    auto start_time = chrono::high_resolution_clock::now(); // start running time record
    heapSort(dataset); //sort data
    auto end_time = chrono::high_resolution_clock::now(); // end running time record

    // Calculate execution duration in seconds
    chrono::duration<double> duration = end_time - start_time;
    cout << "\n-----------------------------------------" << endl;
    cout << "Dataset Size: " << sizeStr << endl;
    cout << "Heap Sort Execution Time: " << duration.count() << " seconds" << endl;
    cout << "-----------------------------------------\n" << endl;

    // Write the sorted array back into a CSV file
    cout << "Writing sorted data to: " << outputFilename << "..." << endl;
    ofstream outFile(outputFilename);
    if (outFile.is_open()) {
        for (const auto& rec : dataset) {
            outFile << rec.id << "," << rec.key << "\n";
        }
        outFile.close();
        cout << "Sorting and file write completed! :D \n" << endl;
    } else {
        cerr << "Error: Could not write output file!" << endl;
    }

    return 0;
}