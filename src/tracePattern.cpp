#include "../include/tracePattern.h"

void TracePattern::loadAndTrace() {
  void *lib = dlopen("./bin/fsm.so", RTLD_NOW);
  if (!lib) {
    printf("dlopen failed: %s\n", dlerror());
    return;
  }

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
    clock_t start = clock();
    void (*f)(char *);
    f = (void (*)(char *))dlsym(lib, "mine_pattern");
    if (f) {
      f(input);
    } else {
      printf("dlsym for f1 failed: %s\n", dlerror());
    }
    clock_t end = clock();
    clock_t tot_Time = end - start;
    printf("Trace completed in %lf sec\n", ((float)tot_Time) / CLOCKS_PER_SEC);
  }

  dlclose(lib);
}