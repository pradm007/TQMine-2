#ifndef GEN_TRACE
#define GEN_TRACE

#include "../include/commonHeader.h"
#include "../include/writeToFile.h"
#include <stdio.h>

const string nameEM = "traceEM", nameTime = "traceTime";

const string fileNameEM = "./traceBin/" + nameEM;
const string fileNameTime = "./traceBin/" + nameTime;

int getRandomDigit(int maxUpperLimit = 10) { return rand() % maxUpperLimit; }

char getRandomCharacter(int maxShift = 1) {
  maxShift = minOf(abss(maxShift), 26);
  return (char)(getRandomDigit(maxShift) + 97);
}


int writeToFile(int alphabetLength = 4, long traceLength = 100) {

  int result_fileNameTime = 0, result_fileNameEM = 0;
  string traceString = "";
  for (long i = 0; i < traceLength; i++) {
    string event(1, getRandomCharacter(alphabetLength)); 
    traceString += " " + event; //Add event
    // traceString += " " + event + to_string(getRandomDigit(10)) + to_string(getRandomDigit(10)) + to_string(getRandomDigit(10)) + to_string(getRandomDigit(10)) + to_string(getRandomDigit(10)); //Add event
    traceString += " " + to_string(getRandomDigit(1000));
    // traceString += " " + to_string(getRandomDigit(100));
    // traceString += " " + to_string(getRandomDigit(100));
  }

  traceString += " " + to_string(getRandomDigit(100000)) + "\n"; // last quant

  result_fileNameEM = Util::writeToFileFunc(fileNameEM, traceString);
  
  traceString = "";
  long long previousNumber = 0;
  for (long i = 0; i < traceLength; i++) {
    
    for (int j=0;j<2;j++) { //Since we are adding 4 events/quant i.e A01 23 43 23
      previousNumber += getRandomDigit(10);
      traceString += " " + to_string(previousNumber); //Add event
    }
  }
  previousNumber += getRandomDigit(10);
  traceString += " " + to_string(previousNumber); //Add time for the last quant
  traceString += "\n";
  
  result_fileNameTime = Util::writeToFileFunc(fileNameTime, traceString);
  
  return result_fileNameEM & result_fileNameTime;
  
}

void _main_generateTrace() {

  cout << "Enter the following details to generate synthetic trace" << endl;

  long traceLength;
  cout << "Trace Length : ";
  cin >> traceLength;

  int alphabetLength;
  cout << "Alphabet Length (max 26): ";
  cin >> alphabetLength;

  if (writeToFile(alphabetLength, traceLength) != 0) {
    cout << "Trace file generated successfully" << endl;
  } else {
    cout << "Something went wrong" << endl;
  }
}
#endif