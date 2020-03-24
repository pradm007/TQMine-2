
#include "../include/ragelGenerator.h"
#define LOCAL_DEBUG 0

void RagelGenerator::prepareAlphabetArr(int alphabetLength) {
    alphabetArr.clear();
    alphabetVisitied.clear();
    overallPattern.clear();


    if (alphabetLength <= MAX_EVENT_REPRESENTATION) {
        for (int i = 0; i < alphabetLength; i++) {
            string repsentationMarker(1,(char)(97 + i));
            alphabetArr.push_back(repsentationMarker);
            alphabetVisitied.push_back(0);
        }
        eventRepresentationLength = 1;
    } else {
        //If alphabet length exceeds alphabetical representation then use numerical suffix as A1,A2,...,B1,B2...

        int bucketSize = alphabetLength/MAX_EVENT_REPRESENTATION;
        int bucketMoreOver = alphabetLength % MAX_EVENT_REPRESENTATION;
        int maxBucketSize = bucketSize + (bucketMoreOver > 0 ? 1 : 0);

        eventRepresentationLength = 1 + to_string(maxBucketSize-1).size(); //Minus 1 is because we are starting from 0 index as A0 -> A9 is 10 elements
        
        for (int j=0;j<MAX_EVENT_REPRESENTATION;j++) {
            for (int i = 0; i < bucketSize; i++) {
                string repsentationMarker(1,(char)(97 + j));
                repsentationMarker += to_string(i);

                alphabetArr.push_back(repsentationMarker);
                alphabetVisitied.push_back(0);
            }

            if (bucketMoreOver > 0) {
                //Insert the bucketMoreOver once horiztally
                string repsentationMarker(1,(char)(97 + j));
                string currentNumber = to_string(bucketSize);
                string prefixer( (maxBucketSize - 1 - currentNumber.size() ) , '0'); //All of this to just make it uniform as A01, A02...A11...A99

                repsentationMarker += prefixer + currentNumber;

                alphabetArr.push_back(repsentationMarker);
                alphabetVisitied.push_back(0);
                bucketMoreOver--;
            }
        }
    }

    if (LOCAL_DEBUG) {
        cout << "Events are : ";
        for (int _i=0; _i < alphabetArr.size(); _i++) {
            cout << alphabetArr[_i] << " ";
        }
        cout << endl;

        cout << "eventRepresentationLength : " << eventRepresentationLength << endl;

    }
}

void RagelGenerator::generateExpression(string &dynamicRegexExpression, int currentIndex, int alphabetIndex, const int alphabetLength, string prefix) {
    if (currentIndex == dynamicRegexExpression.length()) {
        if (overallPattern.length() > 0) {
            overallPattern += " | ";
        }
        overallPattern += "(" + prefix + ")";
        return;
    }

    if (dynamicRegexExpression.at(currentIndex) == NUM_TRACK_SYMBL) {
        prefix += "("+ NUM_SYMBL + " $from(" + NUM_TRACK_RAGEL_ACK + "))";
        generateExpression(dynamicRegexExpression, currentIndex+1, alphabetIndex, alphabetLength, prefix);
    } else if (dynamicRegexExpression.at(currentIndex) == NUM_NOT_TRACK_SYMBL) {
        prefix += "("+ NUM_SYMBL + " $from(" + NUM_NOT_TRACK_RAGEL_ACK + "))";
        generateExpression(dynamicRegexExpression, currentIndex+1, alphabetIndex, alphabetLength, prefix);
    } else {
        for (int index=0; index< alphabetLength; index++) {
            if (alphabetVisitied[index] == 0) {
                alphabetVisitied[index] = 1;
                prefix.push_back((char) 39);
                {   //Insert each character into the prefix
                    for(int _j=0; _j < alphabetArr[index].size(); _j++) {
                        prefix.push_back(alphabetArr[index].at(_j));
                    }
                }
                prefix.push_back((char) 39);
                generateExpression(dynamicRegexExpression, currentIndex+1, alphabetIndex, alphabetLength, prefix);
                prefix.pop_back();
                {   //Pop the same mount of character out
                    for(int _j=0; _j < alphabetArr[index].size(); _j++) {
                        prefix.pop_back();
                    }
                }
                prefix.pop_back();
                alphabetVisitied[index] = 0;
            }
        }

        return;
    }
}

/**
 * @param dynamicRegexExpression Either in format 0.M.1 or 0.N.1
 * @param alphabetLength Number of event
 */
string RagelGenerator::getRagelExpression(string &dynamicRegexExpression, int alphabetLength) {

    prepareAlphabetArr(alphabetLength);
    determineQuantPlaceholdersInExpression(dynamicRegexExpression);
    string prefix = "";

    // vector<string> dynamicRegexExpressionArr; 
    // boost::split(dynamicRegexExpressionArr, dynamicRegexExpression, boost::is_any_of("MN")); 

    string patternInside = "";
    string expressionInDynForm = "(";
    int j=0;
    generateExpression(dynamicRegexExpression, 0, 0, alphabetLength, prefix);

    return "((" + overallPattern + ")+) $to(CHUNK) %to(A) $lerr(E);";
}

/**
 * Function to determine the count of placeholders. This would help in dividing the chunks in proportions
 * @param dynamicRegexExpression Either in format 0.M.1 or 0.N.1
**/
void RagelGenerator::determineQuantPlaceholdersInExpression(string &dynamicRegexExpression) {
  for (int currentIndex = 0; currentIndex < dynamicRegexExpression.size();
       currentIndex++) {
    if (dynamicRegexExpression.at(currentIndex) == NUM_TRACK_SYMBL ||
        dynamicRegexExpression.at(currentIndex) == NUM_NOT_TRACK_SYMBL) {
      quantPlaceholderCount++;
    }
  }

  if (LOCAL_DEBUG) {
      cout << "Quant placeholders found to be " << quantPlaceholderCount << endl;
  }
}

string RagelGenerator::getFullRagelContent(string &fullRagelExpression) {
    string content = R"(
    #include <bits/stdc++.h>
    #include <omp.h>
    using namespace std;

    #ifndef DEBUG
    #define DEBUG 0
    #endif

    #ifndef MINIMAL
    #define MINIMAL 1
    #endif

    #ifndef FILEINPUT
    #define FILEINPUT 0
    #endif

    #ifndef MINIMAL_2
    #define MINIMAL_2 1
    #endif

    #ifndef DISPLAY_MAP
    #define DISPLAY_MAP 1
    #endif

    #ifndef THREADS
    #define THREADS 16
    #endif

    const string numberListPattern = "[0-9]+";
    unordered_map<string, vector<vector<string> > > patternMap;

    int g_reserveSize = 1e+7;
    const int CHUNK_DELIMITER_SIZE = 1e+5;
    int g_delimiterCount = 0;
    int g_totalCombination = 4;
    int THREAD_COUNT = THREADS;
    const char delimiter = '|';
    vector<string> inputStream_per_thread;
    int eventRepresentationLength = )" + to_string(eventRepresentationLength) + R"(;


    void chunkDivider(char *inp);
    void showChunks();
    void serialeExecution(char *);
    void parallelExecution(char *);
    void chunkDivider(char *, int );
    void releaseMemory(vector<vector<string> > &);

    %% machine foo;
    %% write data;

    void insertIntoTempPatternList(string  &tempPatternList, char element, int *flipperOnEvent, int *currentEventRepresentationLength, vector<string> *numberList) {
        if ((element >= 97 && element <= 122) || (element >= 48 && element <= 57 && *currentEventRepresentationLength < eventRepresentationLength)) { //its event or its a number bbut needs to be considered as event
	
            if (element >= 97 && element <= 122) { //Set the event length to 1. DO NOT do this before this as it would be only correct to start event length check after matching the first alphabetical event character
                *currentEventRepresentationLength = 0;
                numberList->push_back(","); //Add new vector for number tracing
            }

            tempPatternList += element;
            *flipperOnEvent = 1;
            (*currentEventRepresentationLength)++;
        } else { //its a number
            if ((char) tempPatternList[tempPatternList.size() - 1] != (char) numberListPattern[numberListPattern.size() - 1]) {
                tempPatternList += numberListPattern;
                // tempPatternList.push_back(numberListPattern);
            }
            *flipperOnEvent = 0;
        }
    }

    void insertIntoPatternList(unordered_map<string, vector < vector<string > > > &patternMap, string  &fullPattern, vector<string > *numberList) {

        auto itr = patternMap.find(fullPattern);
        const bool is_in = itr != patternMap.end();
        if (is_in) {
            if (!MINIMAL) {
                cout << "Found " << fullPattern << endl;
            }
            vector<vector<string > > *oldNumberList = &itr->second;
            oldNumberList->push_back(*numberList);
        } else {
            if (!MINIMAL) {
                cout << "Did not find " << fullPattern << " thus inserting new " << endl;
            }
            vector<vector<string> > *newNumberList = new vector<vector<string> >;

            // newNumberList->reserve(10000);
            newNumberList->push_back(*numberList);
            patternMap.emplace(fullPattern, *newNumberList);
        }

    }

    void resetPatternList(string &tempPatternList, int* currentEventRepresentationLength) {
        tempPatternList.clear();
        *currentEventRepresentationLength = 0;
    }

    void displayPatternList_Internal(vector<vector<string> > &numberList) {
        for (int i =0; i < numberList.size(); i++) {
                printf("\tList %d : \n\t\t\t", i+1);
                for (int j=0;j<numberList[i].size(); j++) {
                    cout << numberList[i][j] << "||";
                }
                printf("\n");
            }
    }

    void displayPatternList(unordered_map<string, vector<vector<string> > > &patternMap) {
        for (auto itr = patternMap.begin(); itr != patternMap.end(); itr++) {
            string pattern = "" + (string) itr->first;
            cout << pattern << " :\n";
            vector<vector<string> > numberList = itr->second;	
            /* for (int i =0; i < numberList.size(); i++) {
                printf("\tList %d : \n\t\t\t", i+1);
                for (int j=0;j<numberList[i].size(); j++) {
                    cout << numberList[i][j];
                }
                printf("\n");
            } */
            displayPatternList_Internal(numberList);
        }
    }


    void mergeList(unordered_map<string, vector<vector<string> > > &patternMapInternal) {

        for (auto itr = patternMapInternal.begin(); itr != patternMapInternal.end(); itr++) {
            auto itr_global = patternMap.find((string) itr->first);
            const bool is_in = itr_global != patternMap.end();

            vector<vector<string> > pMapInternalValue = (vector<vector<string> >) itr->second;

            if (is_in) { //Existing pattern
                vector<vector<string> >  *oldNumberList = &itr_global->second;
                
                int oldSize = pMapInternalValue.size();

                for (int i=0; i<pMapInternalValue.size();i++) {
                    oldNumberList->push_back(pMapInternalValue[i]);
                }

            } else { //Insert new pattern

                int reserveSize = g_reserveSize > pMapInternalValue.size() ? g_reserveSize : pMapInternalValue.size();
                vector<vector<string> >  *newNumberList = new vector<vector<string> >;
                newNumberList->reserve(reserveSize);

                // #pragma omp parallel for
                for (int i=0; i<pMapInternalValue.size(); i++) {
                    newNumberList->push_back( pMapInternalValue.at(i) );
                }

                patternMap.emplace((string) itr->first,  *newNumberList);
            }

        }
    }

    void releaseMemory(vector<vector<string> > &outVec) {
        vector<vector<string> >().swap(outVec);
    }

    void chunkDivider_singular(char *inp, int quantPlaceholderCount=1) {
        int currentIndex = 0, currentThreadIndex = inputStream_per_thread.size() - 1;
        int currentQuantCount = 0;
        int currentEventRepresentationLength = 0;
        int isNumber = 0;

        while (inp[currentIndex] != '\0') {
            if (inp[currentIndex] >= 48 && inp[currentIndex] <= 57) { //is a number
                if (isNumber == 0 && currentEventRepresentationLength < eventRepresentationLength) {
                    currentEventRepresentationLength++;
                } else if (isNumber == 0) { //Count quant only once for 
                    currentQuantCount++;
                    isNumber = 1;
                    currentEventRepresentationLength = 0;
                } else {
                    currentEventRepresentationLength = 0;
                }

                inputStream_per_thread[currentThreadIndex] += inp[currentIndex];
            } else { //is event
                currentEventRepresentationLength++;

                if (isNumber == 1) { // need to start feed to different thread chunk
                    inputStream_per_thread[currentThreadIndex] += inp[currentIndex];


                    //when to apply delimited
                    if (currentQuantCount == quantPlaceholderCount) {
                        if (currentEventRepresentationLength < eventRepresentationLength) {
                            //Go till the end of the event representation, then do a break.
                            int innerCurrentIndex = currentIndex + 1;

                            while(inp[innerCurrentIndex] != '\0' && currentEventRepresentationLength < eventRepresentationLength) {
                                inputStream_per_thread[currentThreadIndex] += inp[innerCurrentIndex];
                                currentEventRepresentationLength++;
                                innerCurrentIndex++;
                            }
                            currentEventRepresentationLength= 0;//reset to zero for a new beginning
                        }

                        inputStream_per_thread[currentThreadIndex] += delimiter;
                        g_delimiterCount++;

                        if (g_delimiterCount == CHUNK_DELIMITER_SIZE) { // start division for next chunk
                            currentThreadIndex = currentThreadIndex + 1;
                            g_delimiterCount = 0;
                            if (inp[currentIndex+1] != '\0') {
                                inputStream_per_thread.push_back("");
                            }
                        }

                        inputStream_per_thread[currentThreadIndex] += inp[currentIndex];
                        currentEventRepresentationLength++;//increment again for new count after the delimiter

                        currentQuantCount = 0;
                    }

                } else {
                    inputStream_per_thread[currentThreadIndex] += inp[currentIndex];
                }
                isNumber = 0;
            }
            currentIndex++;

        }

        //Chop off the excess... iterate from the back
        int last = inputStream_per_thread[currentThreadIndex].size() - 1;
        while ((char)inputStream_per_thread[currentThreadIndex][last] != delimiter) {
            last--;
        }
        inputStream_per_thread[currentThreadIndex] = inputStream_per_thread[currentThreadIndex].substr(0, last+1);
    }

    void chunkDivider(char *inp, int quantPlaceholderCount) {

        inputStream_per_thread.push_back("");

        for (int i=0;i<quantPlaceholderCount;i++) {
            //Identify starting marker
            char *startingMarker = inp;
            int currentEventCount = 0, isEvent = 0;
            int currentIndex = 0;

            while(startingMarker[currentIndex] != '\0') {
                if (startingMarker[currentIndex] >= 48 && startingMarker[currentIndex] <= 57) { //is a number
                    if (isEvent == 1) {
                        isEvent = 0;
                    }
                } else {
                    if (isEvent == 0) {
                        isEvent = 1;
                        currentEventCount++;
                    }
                }

                if (currentEventCount == (i+1) ) {
                    break;//Use the current marker 
                }

                currentIndex++;
            }

            chunkDivider_singular(&startingMarker[currentIndex], quantPlaceholderCount);
        }
    }


    void showChunks() {
        for (int i=0;i<inputStream_per_thread.size();i++) {
            cout << "Chunk " << (i+1) << " ~~~ " << inputStream_per_thread[i] <<endl;
        }
    }

    void mine_pattern(char *p) {
        int cs, res = 0;
        int totalLength = 0, currentLength = 0;
        string numbersInPattern;
        vector<string > *numberList = new vector<string>;	
        string tempPatternList;
        tempPatternList.reserve(5);

        int currentEventRepresentationLength = 0;
        int flipperOnEvent = 1; // flips to 0 in case of number

        unordered_map<string, vector<vector<string> > > patternMapInternal;

        cs = foo_start;
        totalLength = strlen(p);
        char *eof;
        if (!MINIMAL) {
            printf("Input is %s \n",p);
        }

        if (!MINIMAL) {
            printf("cs is %d and foo_start is %d\n", cs, foo_start);
        }

        %%{
            
            action CHUNK {
                if (DEBUG) {
                    cout << "Element -> " << (char) fc << endl;
                }
                if (!MINIMAL) {
                    cout << "Chunk called " << endl;
                }
                currentLength++;
                if ((fc >= 97 && fc <= 122) || (fc >= 48 && fc <= 57)) {
                    insertIntoTempPatternList(tempPatternList, (char) fc, &flipperOnEvent, &currentEventRepresentationLength, numberList);
                }
            }

            action A {
                res++;
                if (!MINIMAL) {
                    printf("Match happened.\n");
                }

                insertIntoPatternList(patternMapInternal, tempPatternList, numberList);

                resetPatternList(tempPatternList, &currentEventRepresentationLength);

                numberList = new vector<string>;

                cs = foo_start;
                p--;
            }
            action NUM {
                if (fc >= 48 && fc <= 57) {
                    if (numberList->empty()) {
                        numberList->push_back(",");
                    }

                    numberList->at(numberList->size() - 1) = numberList->at(numberList->size() - 1) + (char) fc;
                }
            }
            action NUM_NOTRACK {
                if (fc >= 48 && fc <= 57) {
                    // printf("Skipping Num =%c \n",fc);
                }
            }
            action E {
                //res = 0;
                if (DEBUG) {
                    printf("Error happened.\n");
                }
                cs = foo_start;
                if (DEBUG) {
                    printf("cs is %d and foo_start is %d\n", cs, foo_start);
                }

                numberList = new vector<string>;
                resetPatternList(tempPatternList, &currentEventRepresentationLength);

                if (currentLength >= totalLength) {
                    // Force break... very bad practice
                    if (DEBUG) {
                        cout << "Trying to break "<< endl;
                    }
                    fbreak;
                }
            }
        

            main := )" + fullRagelExpression + R"(

            write init nocs;
            write exec noend;
        }%%

        if (!MINIMAL_2)	{
            cout << "Finished processing \n\n";
        }
        
        if (DEBUG) {
            cout << "Displaying internal pattern map per thread" << endl;
            // #pragma omp critical
            displayPatternList(patternMapInternal);
        }

        if (!MINIMAL_2) {
            printf("For Thread %d \t", omp_get_thread_num());
            printf("Pattern matched %d times\n", res);
        }

        #pragma omp critical
        {
            if (DEBUG) {
                cout << "Merging internal and external list" << endl;
            }
            mergeList(patternMapInternal);
            if (DEBUG) {
                cout << "Merge finished for Thread " << omp_get_thread_num() << endl;
            }
        }

    }

    /**
     * Function for parallel Execution
     **/
    extern "C" {unordered_map<string, vector<vector<string> > >* mine_pattern_parallelExecution(char *inp);}
    unordered_map<string, vector<vector<string> > >* mine_pattern_parallelExecution(char *inp) {
        
        double t;

        if (DEBUG) {
            cout << "Initiating chunk division" << endl;
        }
        t = omp_get_wtime();
        chunkDivider(inp, )" + to_string(quantPlaceholderCount)+ R"();
        if (DEBUG) {
            printf("Finished chunk division in %.6f ms. \n", (1000 * (omp_get_wtime() - t)));
        }

        t = omp_get_wtime();
        if (DEBUG) {
            showChunks();
        }

        #pragma omp parallel for num_threads(THREAD_COUNT) shared(patternMap, inputStream_per_thread) firstprivate(numberListPattern, g_reserveSize, g_totalCombination)
        for (int i=0;i<inputStream_per_thread.size();i++) {

            string chunkForThread = inputStream_per_thread[i];
            char *inpPerThChar = (char *) malloc(sizeof(char)*(chunkForThread.size() + 1)); 
            strcpy(inpPerThChar, chunkForThread.c_str());
            mine_pattern(inpPerThChar);
            free(inpPerThChar);
            chunkForThread.clear();
            chunkForThread.shrink_to_fit();
        }

        cout << "Size of pattern Map " << patternMap.size() << endl;
        if (DISPLAY_MAP) {
            displayPatternList(patternMap);
        }

        /* calculate and print processing time*/
        t = 1000 * (omp_get_wtime() - t);
        printf("Finished in %.6f ms using %d threads. \n", t, THREAD_COUNT);

        return &patternMap;
    }

    )";

    return content;
}

void RagelGenerator::generateRagelFile(string &dynamicRegexExpression, int alphabetLength) {

    if (LOCAL_DEBUG) {
        prepareAlphabetArr(alphabetLength);
        string prefix = "";
        determineQuantPlaceholdersInExpression(dynamicRegexExpression);
        generateExpression(dynamicRegexExpression, 0, 0, alphabetLength, prefix);
        cout << "overallPattern : " << overallPattern << endl;
        return;
    }

    double t = omp_get_wtime();
    
    string fullRagelExpression = getRagelExpression(dynamicRegexExpression, alphabetLength);
    string ragelContent = getFullRagelContent(fullRagelExpression);

    if (Util::writeToFileFunc(fileName, ragelContent) != 0) {
    cout << "Ragel FSM file generated successfully" << endl;
    } else {
    cout << "Something went wrong" << endl;
    }

    printf("[Elapsed time:  %.6f ms]\n", (1000 * (omp_get_wtime() - t)));
}