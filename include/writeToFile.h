#ifndef WRITE_TO_FILE
#define WRITE_TO_FILE

#include "../include/commonHeader.h"
#include <stdio.h>

class Util {
  public:

  static bool fexists(const string filename) {
    ifstream ifile(filename.c_str());
    return (bool) ifile;
  }

  static int deleteFile(const string fileName) {
    cout << "Searching for file : " << fileName << endl;
    if (!fexists(fileName.c_str())){
      puts("\tNo old file found. ");
      return true;
    } else if (remove(fileName.c_str()) != 0) {
      perror("\tError deleting file");
      return false;
    } else {
      puts("Old file successfully deleted");
      return true;
    }
  }

  static int writeToFileFunc(const string fileName, const string fileContent) {

    ofstream myfile;

    if (!deleteFile(fileName))
      return false;
    
    myfile.flush();

    myfile.open(fileName);
    myfile << fileContent;
    myfile.close();

    return true;
  }
};

#endif