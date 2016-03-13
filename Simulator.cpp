#include "parseELF.h"
#include "parseELF.cpp"

using namespace std;

int main(){
	char filename[50];
	cout << "Filename: ";
	cin >> filename;

	parseELF(filename);
	return 0;
}