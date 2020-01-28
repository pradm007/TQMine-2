/**
 * @author Pradeep Mahato
 * @email pradeepmahato007@gmail.com
 * @create date 2020-01-26 12:35:52
 * @modify date 2020-01-26 12:35:52
 */

#include "../include/commonHeader.h"
#include "./ragelGenerator.cpp"

string pattern = "0M1M2";
int alphabetLength = 3;

int main(int argc, char *argv[]) {
	cout << "Enter Regex pattern : ";
	cin >> pattern;

	cout << "Enter event length (max 26) : ";
	cin >> alphabetLength;

	alphabetLength = minOf(alphabetLength, 26);

	RagelGenerator rg;
	rg.generateRagelFile(pattern, alphabetLength);

	return 0;
}