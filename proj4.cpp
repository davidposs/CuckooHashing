//*****************************************************************************
// David Poss
// CPSC 335
// Project 4
// 11/28/2016
//*****************************************************************************

#include <iostream>
#include <string>
#include <fstream>

const int tablesize = 19; // Individual cuckoo table size       
std::string table[tablesize][2]; // 2-dimenstional table to store hashes
bool place_in_hash_tables (std::string); // Function to store a string in table
size_t compute_hashes(std::string, size_t);   // Compute the hash functions

int main(int argc, char* argv[]) {
    std::string filename = argv[1];  // String of input file name
    std::string string_to_hash;      // String to hash into the cuckoo table
    std::string string_from_file;    // Line read from the input file
    std::fstream input_file;         // File to read from
    input_file.open(filename, std::ios::in);
    if (input_file.is_open()) {
        while (getline(input_file, string_from_file)) { //Read 1 line at a time
            if (!place_in_hash_tables(string_from_file)) { //Place the string
                std::cout << "Placement has failed" << std::endl;
                return -1;
            }
        }
        input_file.close();
    }
    else { // File did not open correctly
       std::cout << "There was an error opening the file." << std::endl;
       return -1;
    }
    return 0;
}

bool place_in_hash_tables (std::string str) {
    bool placed = false; // Verifies placement was successful
    int counter = 0;  // Used to detect loops in hashing
    size_t index = 0; // Start at first cuckoo table
    std::string temp_s = str, temp;
    int pos = compute_hashes(temp_s, index);
    while((!placed) && (counter < 2*tablesize)) {
        if (table[pos][index].empty()) { 
            std::cout << "String <" << temp_s << "> will be placed at";
            std::cout  << " t[" << pos <<"][" << index << "]" << std::endl;
            table[pos][index] = temp_s;
            return true;
        }
        else { // Entry at table[pos][index] is not available
            std::cout << "String <" << temp_s << "> will be placed at";// << i;
            std::cout << " t[" << pos <<"][" << index << "]" << " replacing <";
            std::cout << table[pos][index] << ">" << std::endl;
            temp = table[pos][index];
            table[pos][index] = temp_s;
            index = abs(--index); //if index = 1, make it 0, if its 0 make it 1
            pos = compute_hashes(temp, index);
            counter++;
            temp_s = temp;
        }
    }
    return placed;
}

size_t compute_hashes(std::string str, size_t index) {
    // index == 0 means the first hash function, 1 means the second
    int val, temp;
    size_t po = 1, len = str.length();
    if (index == 0) {
        val = str[0] % tablesize;
        if (val < 0) { val += tablesize; }
        if (len == 1) { return val; }
        for (int i = 1; i < len; i++) {
            temp = str[i];
            po *= 37;
            po %= tablesize;
            if (po < 0) { po += tablesize; }
            val += temp * po;
            val %= tablesize;
            if (val < 0) { val += tablesize; }
        }
        return val;
    }
    else {
        val = str[len - 1] % tablesize;
        if (len == 1) { return val; }
        for (int i = 1; i < len; i++) {
            temp = str[len - i - 1];
            po *= 37;
            po %= tablesize;
            val += temp * po;
            val %= tablesize; 
        }
        if (val < 0) { val += tablesize; }
        return val;
    }
}
