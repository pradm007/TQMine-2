
#include "../include/ragelGenerator.h"
#define LOCAL_DEBUG 0

void RagelGenerator::prepareAlphabetArr(int alphabetLength) {
    alphabetArr.clear();
    alphabetVisitied.clear();
    overallPattern.clear();

    for (int i = 0; i < alphabetLength; i++) {
    alphabetArr.push_back((char)(97 + i));
    alphabetVisitied.push_back(0);
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
                prefix.push_back((char)alphabetArr[index]);
                prefix.push_back((char) 39);
                generateExpression(dynamicRegexExpression, currentIndex+1, alphabetIndex, alphabetLength, prefix);
                prefix.pop_back();
                prefix.pop_back();
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
    string prefix = "";

    // vector<string> dynamicRegexExpressionArr; 
    // boost::split(dynamicRegexExpressionArr, dynamicRegexExpression, boost::is_any_of("MN")); 

    string patternInside = "";
    string expressionInDynForm = "(";
    int j=0;
    generateExpression(dynamicRegexExpression, 0, 0, alphabetLength, prefix);

    return "((" + overallPattern + ")+) $to(CHUNK) %to(A) $lerr(E);";
}

string RagelGenerator::getFullRagelContent(string &fullRagelExpression) {
    string content = R"(
    #include <bits/stdc++.h>
    using namespace std;
    #define DEBUG 1


    const string numberListPattern = "[0-9]+";
    int _tempPatternListIndex = 0;

    %% machine foo;
    %% write data;

    string getString(char ch) { 
        string temp = "";
        return "" + ch;    
    } 

    void insertIntoTempPatternList(vector<string>  &tempPatternList, char element, int where) {
        switch(where) {
            case 0:
                if (tempPatternList.empty()) { //completely empty. This happens in the first match
                    string temp = "";
                    temp.push_back((char) element);
                    tempPatternList.push_back(temp);
                } else {
                    tempPatternList[where].push_back((char) element);
                }

            break;
            case 1:
                if (tempPatternList.size() == where) { //no new element in the 2nd index
                    string temp = "";
                    temp.push_back((char) element);
                    tempPatternList.push_back(temp);
                } else {
                    tempPatternList[where].push_back((char) element);
                }

            break;
        };
    }

    void insertIntoPatternList(map<string, vector<vector<int> > > &patternMap, vector<string>  &tempPatternList, vector<vector<int> > &numberList) {
        string fullPattern = "";

        for (int i=0;i<tempPatternList.size();i++) {
            fullPattern += tempPatternList[i];
            if (tempPatternList.size()-1 > i) {
                fullPattern += numberListPattern;
            }
        }

        vector<vector<int> >  newNumberList = numberList;

        const bool is_in = patternMap.find(fullPattern) != patternMap.end();
        if (is_in) {
            cout << "Found " << fullPattern << endl;
            auto itr = patternMap.find(fullPattern);
            vector<vector<int> >  oldNumberList = itr->second;
            for (int j=0;j<newNumberList.size();j++) {
                oldNumberList.push_back(newNumberList[j]);
            }
            patternMap[itr->first] = oldNumberList;
        } else {
            cout << "Did not find " << fullPattern << " thus inserting new " << endl;
            patternMap.emplace(fullPattern, newNumberList);
        }

    }

    void resetPatternList(vector<string> &tempPatternList) {
        _tempPatternListIndex = 0;
        tempPatternList.clear();
    }

    void displayPatternList(map<string, vector<vector<int> > > &patternMap) {
        for (auto itr = patternMap.begin(); itr != patternMap.end(); itr++) {
            string pattern = "" + (string) itr->first;
            cout << pattern << " :\n";
            vector<vector<int> > numberList = itr->second;	
            for (int i =0; i < numberList.size(); i++) {
                printf("\tList %d : \n\t\t\t", i+1);
                for (int j=0;j<numberList[i].size(); j++) {
                    printf("%d ",numberList[i][j]);
                }
                printf("\n");
            }

        }
    }


    extern "C" {void mine_pattern(char *p);}
    void mine_pattern(char *p) {
        int cs, res = 0;
        int totalLength = 0, currentLength = 0;
        vector<int> numbersInPattern;
        vector<vector<int> > numberList;	
        vector<string> tempPatternList;

        map<string, vector<vector<int> > > patternMap;

        cs = foo_start;
        totalLength = strlen(p);
        char *eof;
        printf("Input is %s \n",p);
        vector<int> temp_numbersInPattern;
        printf("cs is %d and foo_start is %d\n", cs, foo_start);

        %%{
            
            action CHUNK {
                if (DEBUG) {
                    cout << "Element -> " << (char) fc << endl;
                }
                currentLength++;
                if (fc >= 97 && fc <= 122) {
                    insertIntoTempPatternList(tempPatternList, (char) fc, _tempPatternListIndex);
                }
            }

            action A {
                res++;
                printf("Match happened.\n");
                for (int i=0 ;i< temp_numbersInPattern.size(); i++) {
                    if (DEBUG) {
                        cout << temp_numbersInPattern[i] << " ~ ";
                    }
                    numbersInPattern.push_back(temp_numbersInPattern[i]);
                }
                if (DEBUG) {
                    printf("\n");
                }
                numberList.push_back(numbersInPattern);
                insertIntoPatternList(patternMap, tempPatternList, numberList);
                resetPatternList(tempPatternList);
                numberList.clear();
                numbersInPattern.clear();
                temp_numbersInPattern.clear();

                cs = foo_start;
                p--;
            }
            action NUM {
                if (fc >= 48 && fc <= 57) {
                    // printf("	Num =%c \n",fc);
                    temp_numbersInPattern.push_back( (char) (fc-48));
                    _tempPatternListIndex = 1;
                }
            }
            action NUM_NOTRACK {
                if (fc >= 48 && fc <= 57) {
                    // printf(\"Skipping Num =%c \n",fc);
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

                numberList.clear();
                temp_numbersInPattern.clear();
                resetPatternList(tempPatternList);

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

        cout << "Finished processing \n\n";
        


        printf("Pattern matched %d times\n", res);
        if (DEBUG) {
            displayPatternList(patternMap);
        }
    }
    )";

    return content;
}

void RagelGenerator::generateRagelFile(string &dynamicRegexExpression, int alphabetLength) {

    if (LOCAL_DEBUG) {
    prepareAlphabetArr(alphabetLength);
    string prefix = "";
    generateExpression(dynamicRegexExpression, 0, 0, alphabetLength, prefix);
    cout << "overallPattern : " << overallPattern << endl;
    return;
    }
    
    string fullRagelExpression = getRagelExpression(dynamicRegexExpression, alphabetLength);
    string ragelContent = getFullRagelContent(fullRagelExpression);

    if (Util::writeToFileFunc(fileName, ragelContent) != 0) {
    cout << "Ragel FSM file generated successfully" << endl;
    } else {
    cout << "Something went wrong" << endl;
    }
}