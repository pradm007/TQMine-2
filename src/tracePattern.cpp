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
    while (getline(myfile, inp)) {
    }
    myfile.close();
  }
  input = (char *)malloc((inp.size() + 1) * sizeof(char *));
  strcpy(input, inp.c_str());

  if (input != NULL && !string(input).empty()) {
    double t = omp_get_wtime();
    void (*f)(char *);
    f = (void (*)(char *))dlsym(lib, "mine_pattern_parallelExecution");
    if (f) {
      printf("Dynamic Linker loaded successfully\n");
      f(input);
    } else {
      printf("Dynamic Linker loaded failed\n");
      printf("dlsym for function grab failed: %s\n", dlerror());
    }
    
	  printf("Trace completed [Elapsed time: %.6f ms]\n", (1000 * (omp_get_wtime() - t)));
  }

  dlclose(lib);
}