#include <bits/stdc++.h>
using namespace std;

string getFullRagelContent(string &dynamicRegexExpression, int alphabetLength = 1) {
        string content = R"(
            #include <bits/stdc++.h>
            using namespace std;
            #define DEBUG 1


            const string numberListPattern = \"[0-9]+\";
            int _tempPatternListIndex = 0;

            \%\% machine foo;
            \%\% write data;

            string getString(char ch) { 
                string temp = "";
                return "" + ch;    
            } 

            void insertIntoTempPatternList(vector<string>  &tempPatternList, char element, int where) {
                switch(where) {
                    case 0:
                        if (tempPatternList.empty()) { //completely empty. This happens in the first match
                            string temp = \"\";
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
                    cout << \"Found \" << fullPattern << endl;
                    auto itr = patternMap.find(fullPattern);
                    vector<vector<int> >  oldNumberList = itr->second;
                    for (int j=0;j<newNumberList.size();j++) {
                        oldNumberList.push_back(newNumberList[j]);
                    }
                    patternMap[itr->first] = oldNumberList;
                } else {
                    cout << \"Did not find \" << fullPattern << \" thus inserting new \" << endl;
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
                    cout << pattern << \" :\n\";
                    vector<vector<int> > numberList = itr->second;	
                    for (int i =0; i < numberList.size(); i++) {
                        printf(\"\tList %d : \n\t\t\t\", i+1);
                        for (int j=0;j<numberList[i].size(); j++) {
                            printf(\"%d \",numberList[i][j]);
                        }
                        printf(\"\n\");
                    }

                }
            }
        )";

        return content;
    }

int main() {
    string pat = "0M1";
    string frc = getFullRagelContent(pat, 2);
    return 0;
}