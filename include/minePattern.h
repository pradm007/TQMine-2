#ifndef MINE_TRACE
#define MINE_TRACE

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

    cout << "Enter event length (max 26) : ";
    cin >> alphabetLength;

    alphabetLength = minOf(alphabetLength, 26);

    // Generate ragel file
    RagelGenerator rg;
    rg.generateRagelFile(pattern, alphabetLength);

    // Compile ragel file
    if (Util::fexists("./bin/fsm.rl")) {
        system("ragel -C ./bin/fsm.rl -o ./bin/fsm.cpp && g++ -std=c++11 -fpic -w -g -shared "
                "-o ./bin/fsm.so -ldl ./bin/fsm.cpp -ldl");
    } else {
        perror("Ragel file not found !!");
        return;
    }

    // Perform mining
    TracePattern tp;
    tp.loadAndTrace();
}

#endif