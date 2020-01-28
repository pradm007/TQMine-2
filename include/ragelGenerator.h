#include "../include/commonHeader.h"
#include "../include/writeToFile.h"
#include <boost/algorithm/string.hpp>

class RagelGenerator {

    public :
    const string NUM_SYMBL = "[0-9]+";
    const char NUM_TRACK_SYMBL = 'M';
    const char NUM_NOT_TRACK_SYMBL = 'N';
    const string NUM_TRACK_RAGEL_ACK = "NUM";
    const string NUM_NOT_TRACK_RAGEL_ACK = "NUM_NOTRACK";


    const string name = "fsm";
    const string fileName = "./bin/" + name + ".rl";

    string overallPattern = "";

    vector<char> alphabetArr;
    vector<int> alphabetVisitied;

    void prepareAlphabetArr(int alphabetLength = 1);
    void generateExpression(string &dynamicRegexExpression, int currentIndex, int alphabetIndex, const int alphabetLength, string prefix);
    string getRagelExpression(string &dynamicRegexExpression, int alphabetLength = 1);
    string getFullRagelContent(string &fullRagelExpression);
    void generateRagelFile(string &dynamicRegexExpression, int alphabetLength = 1);

};