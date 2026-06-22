/*********************************************************************************************
Program : radix_sort_step.cpp
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
1. compile (command : g++ radix_sort_step.cpp -o radix_sort_step -std=c++17 -O3)
2. run (command : ./radix_sort_step <dataset_size>)
Example: ./radix_sort dataset_1000.csv 1 1000  
note :                                  ^^1 is start row, 1000 is end row 
********************************************************************************************
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Record{
    long long number;
    string str;
};

//Estimate the working space used by the record vector.
size_t estimateSpaceBytes(const vector<Record> &data) {
    size_t bytes = data.capacity() * sizeof(Record);
    for (const auto &r : data) {
        bytes += r.str.capacity();
    }
    return bytes;
}

//read csv file and returns rows
vector<Record> readCSV(const string& filename, int startRow, int endRow){
    ifstream file(filename);
    if(!file.is_open()){
        cerr<<"Failed to open file: "<<filename<<endl;
        return {};
    }

vector<Record> data;
string line;
int currentRow =0;

while (getline(file, line)){
    currentRow++;

    //process rows in specified range
    if(currentRow<startRow) continue;
    if(currentRow>endRow) break;

    //parsing
    stringstream ss(line);
    string numStr, str; //temporary variables to hold parsed values
    getline(ss,numStr,','); //get number as string
    getline(ss,str,','); //get string

    Record r;
    r.number=stoll(numStr); //convert string to long long
    r.str=str; //assign string
    data.push_back(r);
}

file.close();
return data;
}

//write current state of array to output
void writeCSV(ofstream &outFile, const vector<Record> &data, const string &label){
    outFile << "[";
    for(size_t i = 0; i < data.size(); i++){
        outFile << data[i].number << "/" << data[i].str;
        if (i < data.size() - 1) {
            outFile << ", ";
        }
    }
    outFile << "] " << label << endl;
}

//algo................................................................................................................
//get the digit at position d (d=1 is rightmost/ones, d=10 is leftmost for 10-digit numbers)
int getDigit(long long num, int d) {
    //divide by 10^(d-1) then mod 10
    long long divisor = 1;
    for (int i = 1; i < d; i++) divisor *= 10;
    return (num / divisor) % 10;
}  

//counting sort based on digit d
void countingSortByDigit(vector<Record> &data, int d){
    int n=data.size();
    vector<Record> output(n); //output array
    int count[10]={0}; //count array for digits 0-9


//count occurrences of each digit
for (int i=0; i<n; i++){
    int digit=getDigit(data[i].number,d);
    count[digit]++;
}

//change count[i] so that it contains actual position of this digit in output[]
for (int i=1; i<10; i++){
    count[i]+=count[i-1];
}

//build output array — traverse from right to left for stability
for (int i = n - 1; i >= 0; i--) {
        int digit = getDigit(data[i].number, d);
        output[count[digit] - 1] = data[i];
        count[digit]--;
    }

//Copy output back to data
for (int i = 0; i < n; i++) {
        data[i] = output[i];
    }
}


int main(int argc, char* argv[]){
    string filename;
    int startRow = 0;
    int endRow = 0;

    if (argc >= 4) {
        filename = argv[1];
        startRow = atoi(argv[2]);
        endRow = atoi(argv[3]);
    } else {
        cout << "Enter the dataset CSV filename (e.g. dataset_1000.csv): ";
        getline(cin, filename);
        cout << "Enter the start row (e.g. 1): ";
        cin >> startRow;
        cout << "Enter the end row (e.g. 1000): ";
        cin >> endRow;
    }

    vector<Record> data = readCSV(filename, startRow, endRow);
 
    if (data.empty()) {
        cerr << "Error: No data loaded. Check your row range." << endl;
        return 1;
    }

    auto startTime = chrono::high_resolution_clock::now();
    size_t estimatedSpace = estimateSpaceBytes(data);

    //Build output filename:
    //dataset_1000_radix_sorted_step_startrow_endrow.txt
    //Extract the size n from filename (e.g. "dataset_1000.csv" -> "1000")
    string baseName = filename;
    size_t slashPos = baseName.find_last_of("/\\");
    if (slashPos != string::npos) baseName = baseName.substr(slashPos + 1);
    size_t dotPos = baseName.find_last_of(".");
    if (dotPos != string::npos) baseName = baseName.substr(0, dotPos); // "dataset_1000"
 
    string outFilename = baseName + "_radix_sorted_step_"
                         + to_string(startRow) + "_"
                         + to_string(endRow) + ".txt";
 
    ofstream outFile(outFilename);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot open output file " << outFilename << endl;
        return 1;
    }
 
    //write original state
    writeCSV(outFile, data, "original");
 
    //10-digit integers → 10 passes, from rightmost (d=10) to leftmost (d=1)
    //The sample output shows d=10 first (rightmost digit), down to d=1
    for (int d = 10; d >= 1; d--) {
        countingSortByDigit(data, d);
        writeCSV(outFile, data, "d=" + to_string(d));
    }
 
    outFile.close();
    auto endTime = chrono::high_resolution_clock::now();
    auto elapsedMs = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

    cout << "Step-by-step output written to: " << outFilename << endl;
    cout << "Elapsed time: " << elapsedMs << " ms" << endl;
    cout << "Estimated working space: " << estimatedSpace << " bytes" << endl;

 
    return 0;
}
 