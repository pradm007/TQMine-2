#include "../include/tracePattern.h"

void TracePattern::loadAndTrace() {
  void *lib = dlopen(shareObject_FileName.c_str(), RTLD_NOW);
  if (!lib) {
    printf("dlopen failed: %s\n", dlerror());
    return;
  }
  cout << "Dynamic Shared Object :" << endl;
  cout << "\t\t File Name : " << shareObject_FileName << endl;
  cout << "\t\t Status : Success" << endl;


  char *input = "";
  ifstream myfile("./traceBin/trace");
  string inp;
  if (myfile.is_open()) {
    inp.assign( (istreambuf_iterator<char>(myfile) ), (istreambuf_iterator<char>()) );
    myfile.close();
  } else {
    cout << "Something went wrong while opening file" << endl;
  }

  input = (char *)malloc((inp.size() + 1) * sizeof(char *));
  strcpy(input, inp.c_str());

  if (input != NULL && !inp.empty()) {
    double t = omp_get_wtime();
    unordered_map<string, vector<vector<string> > >* (*f)(char *);
    f = (unordered_map<string, vector<vector<string> > >* (*)(char *))dlsym(lib, "mine_pattern_parallelExecution");
    if (f) {
      printf("Dynamic Linker loaded successfully\n");
      unordered_map<string, vector<vector<string> > >* patternMap = f(input);

      if (CSVOUTPUT && inp.size() < 10000000) { //10 MB
        string outputFile = "./output/mine-map.csv";
        if (Util::writeToCSV(outputFile, *patternMap) != 0) {
          cout << "Output file generated successfully" << endl;
        } else {
          cout << "Something went wrong while generating output file" << endl;
        }
      }

        
    } else {
      printf("Dynamic Linker loaded failed\n");
      printf("dlsym for function grab failed: %s\n", dlerror());
    }
    
	  printf("Full trace completed [Elapsed time: %.6f ms]\n", (1000 * (omp_get_wtime() - t)));
  } else {
    cout << "File might be empty. Size detected " << inp.size() << endl;
  }

  dlclose(lib);
}