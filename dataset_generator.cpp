/*
Course: CCP6214 Algorithm Design and Analysis
 Lecture Class: TC6L
 Tutorial Class: T21L
 Trimester: 2610
 Member_1: 242UC244L8 | VIDHYA DARINEY A/P RAJASINGAM 
 Member_2: 242UC244PT | JASMYNE YAP 
 Member_3: 242UC244CK | WAN HANANI IMAN BINTI WAN MOHD AZIDI 

whats this file?
-> generating dataset :3

how to run!!
- be sure to be in the same directory as this file, then follow the steps below:
1. compile (command : g++ -o dataset_generator dataset_generator.cpp -std=c++17)
2. run (command : ./dataset_generator <dataset_size>)
3. dataset will be generated
*/

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>
#include <unordered_set>

using namespace std;

//generate random 5 letter lowercase string
string randomString(mt19937 &rng){ //mt19937 is a random number generator algo
    string s ="";
    uniform_int_distribution<int> letterDist(0,25); //uniform distribution for letters a-z
    for(int i=0; i<5; i++){
        s += (char)('a'+ letterDist(rng)); //generate a random letter and append to string
    }
    return s;
}

int main(int argc, char* argv[]){
    //checks user provided the dataset size as arg
    if(argc<2){
        cerr<<"Usage: "<<argv[0]<<" <dataset_size>"<<endl;
        return 1;
    }

    int datasetSize = atoi(argv[1]);
    if(datasetSize <= 0){
        cerr<<"Invalid dataset size. Please provide a positive integer."<<endl;
        return 1;
    }

    //group leader : 242UC244L8 -> 2421324428
    mt19937 rng(2421324428); //seed the random number generator with group leader's ID
    uniform_int_distribution<long long> intDist(1000000000LL, 9999999999LL); //uniform distribution for integers 1 to 1 million

    //use unordered_set library to store unique strings and integers
    unordered_set<long long> used;
    used.reserve(static_cast<unordered_set<long long>::size_type>(datasetSize)); //reserve space for dataset size to optimize performance

    //for output (csv)
    string filename = "dataset_"+to_string(datasetSize)+".csv";
    ofstream outFile(filename);
    if(!outFile.is_open()){
        cerr<<"Failed to open file: "<<filename<<endl;
        return 1;
    }

    cout<<"Generating dataset of size "<<datasetSize<<"..."<<endl;

    long long count = 0;
    while (count < static_cast<long long>(datasetSize)){
        long long num = intDist(rng); //generate random integer
        if(used.find(num) == used.end()){
            used.insert(num);
            string str = randomString(rng); //generate random string
            outFile<<num<<","<<str<<"\n";
            count++;
        }
        //prints output per 1mil record
         if (count % 1000000 == 0) {
            cout << "  Generated " << count << " / " << datasetSize << " records..." << endl;
        }
    }
    outFile.close();
    cout<<"Dataset generated successfully! :>"<<endl;
    return 0;
}

