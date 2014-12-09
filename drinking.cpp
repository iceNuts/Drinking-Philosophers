#include "pthread.h"
#include "semaphore.h"
#include "unistd.h"
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
int shot[15];

pthread_t phil[15];

sem_t semaphore;

struct thread_param
{
    int id;
};

void restore(int id) {
    for (int i = 0; i < total; i++) {
        matrix[i][id] *= -1;
        matrix[id][i] *= -1;
    }
}

void print_drink(int id, bool drinking) {
    string prefix = "Philosopher " + to_string(id) + ": ";
    if (drinking) {
        prefix += "drinking from bottles ";
    }
    else {
        prefix += "putting down bottles ";
    }
    for (int i = 0; i < total; i++) {
        if (matrix[id][i] == -1) {
            prefix += "(" + to_string(id) + " " + to_string(i) + ") ";
        }
    }
    printf("%s\n", prefix.c_str());
}

void * drink(void * arg) {
    int id = ((thread_param*)arg) -> id;
    while(shot[id] > 0) {
        sem_wait(&semaphore);
        // check dependency
        bool critical = true;
        bool empty = true;
        for (int i = 0; i < total; i++) {
            if (matrix[id][i] != 0) {
                empty = false;
            }
            if(matrix[id][i] == 1) {
                matrix[id][i] = -1;
                matrix[i][id] = -1;
            }
            else if (matrix[id][i] == -1 || matrix[i][id] == -1) {
                critical = false;
                break;
            }
        }

        // Ignore all zeros

        if (empty) {
            sem_post(&semaphore);
            pthread_exit(0);
        }

        if (!critical) {
            restore(id);
            sem_post(&semaphore);
            continue;
        }
        else {
            print_drink(id, true);
        }
        sem_post(&semaphore);
        
        // wait for drink
        sleep(rand()%20);

        sem_wait(&semaphore);
        print_drink(id, false);
        restore(id);
        shot[id]--;
        sem_post(&semaphore);
    }
    pthread_exit(0);
}

void spawn() {
    for (int i = 0; i < total; i++) {
        thread_param *tp;
        tp = (thread_param*)malloc(sizeof(*tp));
        tp -> id = i;
        if (pthread_create(&phil[i], NULL, drink, (void*)tp)) {
            cout << "some philosopher died at birth" << endl;
            exit(1);
        }
    }
}

// Main

int main(int argc, char* argv[]) {

    // Init

    memset(shot, 5, sizeof(shot));
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

    // pthread
    sem_init(&semaphore, 0, 1);
    spawn();
    for (int i = 0; i < total; i++) 
        pthread_join(phil[i], NULL);

    sem_destroy(&semaphore);

    return 0;
}


































