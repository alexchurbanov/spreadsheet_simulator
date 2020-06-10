#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <ppl.h>
#include "Evaluator.h"
#include "Error.h"

using std::vector;
using std::unordered_map;
using std::istringstream;
using std::pair;


class SpreadSheet
{
	public:
			SpreadSheet(vector<string> strings);
			SpreadSheet(){};
			~SpreadSheet();
			vector<string> getSheet();
			void evaluate();
	
	protected:
			void getColumnName(string& string);
			string expression(const string address,string cell);
			string label(string labelString);
			bool isReference(string refStr);
			bool isNumber(string numbStr);
	
	private:
			unordered_map<string,string> sheet;
			int sheetLenght;
			int *sheetWidth;
			
};

#endif // SPREADSHEET_H