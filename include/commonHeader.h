#ifndef COMMON_HEADER
#define COMMON_HEADER


#ifndef CSVOUTPUT
#define CSVOUTPUT 0
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef THREADS
#define THREADS 16
#endif

#ifndef DISPLAY_MAP
#define DISPLAY_MAP 0
#endif

#include <bits/stdc++.h>
#include <fstream>
#include <omp.h>

#define maxOf(a, b) ((a) > (b) ? (a) : (b))
#define minOf(a, b) ((a) < (b) ? (a) : (b))
#define abss(a) ((a) < 0 ? -1 * (a) : (a))
typedef long long unsigned lln;
typedef long double lld;

using namespace std;
#endif