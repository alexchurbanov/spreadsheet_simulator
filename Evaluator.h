#ifndef EVALUATOR_H
#define EVALUATOR_H


#include <string>
#include <list>
#include "Error.h"

using std::string;
using std::list;
using std::to_string;

class Evaluator
{
	public:
		    string getResult(string formula);
	
    protected:

	private:

};

int findPos(list<char>::iterator first,list<char>::iterator last,char character);
void trim(string& string);
void trimLeft(string& string);
string toString(float number);

#endif // EVALUATOR_H