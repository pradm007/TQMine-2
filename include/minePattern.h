#ifndef MINE_TRACE
#define MINE_TRACE

#include "../include/commonHeader.h"
#include "../include/ragelGenerator.h"
#include <stdio.h>

string pattern = "0M1M2";
int alphabetLength = 3;

void _main_mineTrace() {
    cout << "Enter Regex pattern : ";
    cin >> pattern;

    cout << "Enter event length (max 26) : ";
    cin >> alphabetLength;

    alphabetLength = minOf(alphabetLength, 26);

    RagelGenerator rg;
    rg.generateRagelFile(pattern, alphabetLength);
}

#endif