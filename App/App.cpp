// LZ78.cpp : Defines the entry point for the application.
//

#include "App.h"
#include "Compressor.hpp"

using namespace std;

int main()
{
	Compressor c;
	vector<int> a = c.compress("ALABALAPORTOKALA");
	cout << "ZIP: ";
	for(auto e :a)
		cout << e << " ";
	cout << endl;
	cout << "UNZIP: " << c.decompress(a);
	return 0;
}
