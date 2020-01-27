/**
 * @author Pradeep Mahato
 * @email pradeepmahato007@gmail.com
 * @create date 2020-01-26 12:35:52
 * @modify date 2020-01-26 12:35:52
 */

#include "../include/commonHeader.h"

string pattern;
int alphabetLength = 0;

int main(int argc, char *argv[]) {
	cout << "Enter Regex pattern : ";
	cin >> pattern;

	cout << "Enter event length (max 26) : ";
	cin >> alphabetLength;

	alphabetLength = maxOf(alphabetLength, 26);

	return 0;
}