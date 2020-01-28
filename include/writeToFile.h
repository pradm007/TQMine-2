
#include "../include/commonHeader.h"
#include <stdio.h>

bool fexists(const string filename) {
  ifstream ifile(filename.c_str());
  return (bool) ifile;
}

int deleteFile(const string fileName) {
  if (!fexists(fileName.c_str())){
    puts("No old file found. ");
    return true;
  } else if (remove(fileName.c_str()) != 0) {
    perror("Error deleting file");
    return false;
  } else {
    puts("Old file successfully deleted");
    return true;
  }
}

int writeToFile(const string fileName, const string fileContent) {

  ofstream myfile;

  if (!deleteFile(fileName))
    return false;
  
  myfile.open(fileName);
  myfile << fileContent;
  myfile.close();

  return true;
}