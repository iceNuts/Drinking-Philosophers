#include "pthread.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

int total = -1;
int matrix[15][15];

int main(int argc, char* argv[]) {

    // Init

    for (int i = 0; i < 15; i++) {
        memset(matrix[i], 0, sizeof(matrix[i]));
    }

    // Read Cmd

    char * filename;

    if (argc < 3 || argc > 3) {
        cout << "Usage: -f filename" << endl;
    }
    else {
        filename = argv[2];
    }

    // Read File
    int p_count = 0;
    ifstream infile(filename);
    string line;
    while(getline(infile, line)) {
        istringstream iss(line);
        vector<string> tokens;
        copy(istream_iterator<string>(iss),
        istream_iterator<string>(),
        back_inserter(tokens));

        // update total number

        if (total == -1) {
            total = tokens.size();
        }  

        for (int j = 0; j < tokens.size(); j++) {
            matrix[p_count][p_count+j] = stoi(tokens[j]);
            matrix[p_count+j][p_count] = stoi(tokens[j]);
        }

        p_count++;
    }

    

    return 0;
}


































