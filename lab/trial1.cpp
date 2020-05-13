#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> eventTimeBound;
vector<vector<int>> quantTimeBound;
string dynamicEventTimeDefinition;

string getRagelExpression(string &dynamicRegexExpression,
                          vector<vector<int>> &eventTimeBound,
                          vector<vector<int>> &quantTimeBound) {

  string overallPattern = "";
  string eventSec = "eventSection+";
  string quantSec = "numberSection+";

  int eventBracket = -1; // 1-< and 0->
                         //   int timeBracket = -1;         // 1-[ and 0-]
  int eventSectionInserted = 0; // 0-false, 1 true
  int quantSectionInserted = 0; // 0-false, 1 true
  //   int timeShouldStart = 0;      // 0-true, 1-false
  int TREcount = 0;
  int commaFaced = 0; // 0-did not face 1-faced

  // Count the TREs and prepare the ragel expression
  for (int i = 0; i < dynamicRegexExpression.size(); i++) {
    // cout << " Evaluating " << dynamicRegexExpression[i] << endl;
    if (dynamicRegexExpression[i] == '<') {
      eventBracket = 1;
    } else if (dynamicRegexExpression[i] == '>') {
      eventBracket = 0;
    } else if (dynamicRegexExpression[i] == 'M') {
      if (quantSectionInserted == 0) {
        overallPattern += quantSec + " ";
        quantSectionInserted = 1;
      }
    } else if ((dynamicRegexExpression[i] - '0') >= 0 &&
               (dynamicRegexExpression[i] - '0') <= 9) {

      if (eventBracket == 1) {
        // This is going through event section now
        if (eventSectionInserted == 0) {
          overallPattern += eventSec + " ";
          eventSectionInserted = 1;
        }
      } else if (eventSectionInserted) {
        // This is for time bound of event section
        int val = dynamicRegexExpression[i] - '0';
        if (commaFaced == 0) {
          // For entry bound

          int _val = eventTimeBound[eventTimeBound.size() - 1][commaFaced];

          if (_val == -1)
            _val = 0; // For first time
          _val = _val * 10 + val;

          eventTimeBound[eventTimeBound.size() - 1][commaFaced] = _val;
        } else {
          // For exit bound
          int _val = eventTimeBound[eventTimeBound.size() - 1][commaFaced];

          if (_val == -1)
            _val = 0; // For first time
          _val = _val * 10 + val;

          eventTimeBound[eventTimeBound.size() - 1][commaFaced] = _val;
        }
      } else if (quantSectionInserted) {
        // This is for time bound of quant section
        int val = dynamicRegexExpression[i] - '0';
        if (commaFaced == 0) {
          // For entry bound

          int _val = quantTimeBound[quantTimeBound.size() - 1][commaFaced];

          if (_val == -1)
            _val = 0; // For first time
          _val = _val * 10 + val;

          quantTimeBound[quantTimeBound.size() - 1][commaFaced] = _val;
        } else {
          // For exit bound
          int _val = quantTimeBound[quantTimeBound.size() - 1][commaFaced];

          if (_val == -1)
            _val = 0; // For first time
          _val = _val * 10 + val;

          quantTimeBound[quantTimeBound.size() - 1][commaFaced] = _val;
        }
      }
    } else if (dynamicRegexExpression[i] == '[') {
      //   cout << "eventSectionInserted " << eventSectionInserted << endl;
      //   cout << "quantSectionInserted " << quantSectionInserted << endl;
      if (quantSectionInserted == 1) {
        vector<int> _quantTimeBound;
        _quantTimeBound.push_back(-1);
        _quantTimeBound.push_back(-1);

        quantTimeBound.push_back(_quantTimeBound);
      } else if (eventSectionInserted == 1) {
        vector<int> _eventTimeBound;
        _eventTimeBound.push_back(-1);
        _eventTimeBound.push_back(-1);

        eventTimeBound.push_back(_eventTimeBound);
      }
    } else if (dynamicRegexExpression[i] == ']') {
      quantSectionInserted = 0;
      eventSectionInserted = 0;
      commaFaced = 0;
    } else if (dynamicRegexExpression[i] == ',') {
      commaFaced = 1;
    }
  }

  TREcount = quantTimeBound.size();

  return "((" + overallPattern + "' |') $to(CHUNK) %to(ACCEPT) $lerr(ERR));";
}

void getDynamicEventTimeDefinition() {
  dynamicEventTimeDefinition = "//Input Event Time\n";
  dynamicEventTimeDefinition += "int eventTRE_perInstance = " + to_string(eventTimeBound.size()) + ";\n";
  dynamicEventTimeDefinition += "int inputEventTime[" + to_string(eventTimeBound.size()) + "][2] = {";
  
  for (int i = 0; i < eventTimeBound.size(); i++) {
    dynamicEventTimeDefinition += "{";
    for (int j = 0; j < eventTimeBound[i].size(); j++) {
      dynamicEventTimeDefinition += to_string(eventTimeBound[i][j]);
      if (j == 0)
        dynamicEventTimeDefinition += ",";
    }
    dynamicEventTimeDefinition += "}";
    
    if (i < eventTimeBound.size()-1)
        dynamicEventTimeDefinition += ",";
  }

  dynamicEventTimeDefinition += "}\n";
  dynamicEventTimeDefinition += "int inputQuantTime[" + to_string(eventTimeBound.size() - 1) + "][2] = {";
  
  for (int i = 0; i < quantTimeBound.size(); i++) {
    dynamicEventTimeDefinition += "{";
    for (int j = 0; j < quantTimeBound[i].size(); j++) {
      dynamicEventTimeDefinition += to_string(quantTimeBound[i][j]);
      if (j == 0)
        dynamicEventTimeDefinition += ",";
    }
    dynamicEventTimeDefinition += "}";
    
    if (i < quantTimeBound.size()-1)
        dynamicEventTimeDefinition += ",";
        
  }
  dynamicEventTimeDefinition += "}\n\n";

  dynamicEventTimeDefinition += "int processingEventTime[" + to_string(eventTimeBound.size()) + "][2] = {";
  
  for (int i = 0; i < eventTimeBound.size(); i++) {
    dynamicEventTimeDefinition += "{";
    for (int j = 0; j < eventTimeBound[i].size(); j++) {
      dynamicEventTimeDefinition += "0";
      if (j == 0)
        dynamicEventTimeDefinition += ",";
    }
    dynamicEventTimeDefinition += "}";
    
    if (i < eventTimeBound.size()-1)
        dynamicEventTimeDefinition += ",";
  }
  dynamicEventTimeDefinition += "}; // This stores entry and exit for each TRE event scope\n";
}

void printArray(vector<vector<int>> &vec, string label = "array") {
  cout << "Output for " << label << endl;
  for (int i = 0; i < vec.size(); i++) {
    for (int j = 0; j < vec[i].size(); j++) {
      cout << vec[i][j] << " ";
    }
    cout << endl;
  }
}

int main() {

  string inp = "<0.1>[0,2]M[0,10]<2.3>[9,10]M[0,10]<2.3>[9,10]";

  string expression = getRagelExpression(inp, eventTimeBound, quantTimeBound);
  getDynamicEventTimeDefinition();

  cout << "Input : " << inp << endl;
  cout << "Expression is " << expression << endl;
  printArray(eventTimeBound, "eventTimeBound");
  printArray(quantTimeBound, "quantTimeBound");
  
  cout << "getDynamicEventTimeDefinition \n" << dynamicEventTimeDefinition << endl;

  return 0;
}