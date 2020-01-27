#include "../include/commonHeader.h"
#include <stdio.h>

const string name = "trace";
const string fileName = "./traceBin/" + name;

int getRandomDigit(int maxUpperLimit = 10) { return rand() % maxUpperLimit; }

char getRandomCharacter(int maxShift = 1) {
  maxShift = minOf(abss(maxShift), 26);
  return (char)(getRandomDigit(maxShift) + 97);
}

void deleteFile() {
  if (remove(fileName.c_str()) != 0)
    perror("Error deleting file");
  else
    puts("Old Trace file successfully deleted");
}

int writeToFile(int alphabetLength = 4, long traceLength = 100) {

  string traceString;
  ofstream myfile;

  deleteFile();
  myfile.open(fileName);
  for (long i = 0; i < traceLength; i++) {
    traceString +=
        getRandomCharacter(alphabetLength) + to_string(getRandomDigit(100000));
  }

  traceString += getRandomCharacter(alphabetLength) + "\n";
  myfile << traceString;
  myfile.close();

  return true;
}

int main() {

  cout << "Enter the following details to generate synthetic trace" << endl;

  long traceLength;
  cout << "Trace Length : ";
  cin >> traceLength;

  int alphabetLength;
  cout << "Alphabet Length : ";
  cin >> alphabetLength;

  if (writeToFile(alphabetLength, traceLength) != 0) {
    cout << "Trace file generated successfully" << endl;
  } else {
    cout << "Something went wrong" << endl;
  }
  return 0;
}