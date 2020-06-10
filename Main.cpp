#include "SpreadSheet.h"
#include <iostream>

using std::cout;
using std::endl;
using std::cin;


void main()
{
	vector<string> strings;
	string buffer;
	
	cout << "Welcome to the Spreadsheet Simulator!!!\n"
		    "New row starts when you press Enter, new column when you press Tab.\n"
		    "To finish, in new empty line press Enter.\n"
		 << endl;

	while(getline(cin,buffer,'\n'),!buffer.empty())
	{
		strings.push_back(buffer);
	}
	
	SpreadSheet table1(strings);
	table1.evaluate();
	vector<string> result;
	result = table1.getSheet();
	
	cout << endl;
	
	for(auto iter : result)
	{
		cout << iter;
	}

	cout << endl;
	system("pause");
}