#ifndef MINE_TRACE
#define MINE_TRACE

#ifndef THREADS
#define THREADS 16
#endif

#include "../include/commonHeader.h"
#include "../include/ragelGenerator.h"
#include "../include/tracePattern.h"
#include <stdio.h>

string pattern = "0M1M2";
int alphabetLength = 3;

void _main_mineTrace() {

    // Take the input
    cout << "Enter Regex pattern : ";
    cin >> pattern;

    cout << "Enter event length (recommended max limit 100): ";
    cin >> alphabetLength;

    // Generate ragel file
    RagelGenerator rg;
    rg.generateRagelFile(pattern, alphabetLength);

    // Compile ragel file
    if (Util::fexists("./bin/fsm.rl")) {
        double t = omp_get_wtime();
        
        string exec = "ragel -C -T0 ./bin/fsm.rl -o ./bin/fsm.cpp && g++ -std=c++11 -fopenmp -Ofast -fpic -w -g -shared -D THREADS=" + to_string(THREADS) + " -o ./bin/fsm.so -ldl ./bin/fsm.cpp -ldl";

        system(exec.c_str());

        printf("State machine created successfully [Elapsed time: %.6f ms]\n", (1000 * (omp_get_wtime() - t)));
    } else {
        perror("Ragel file not found !!");
        return;
    }

    // // Perform mining
    TracePattern tp; 
    tp.loadAndTrace();
}

#endif