#include "SpreadSheet.h"

SpreadSheet::SpreadSheet(vector<string> strings)
{
	
	string buffer,colName;
	int rowName = 1;
	sheetLenght = strings.size();
	sheetWidth = new int[sheetLenght];
	
	for(auto iter : strings)
	{
		colName = "A";
		istringstream stream(iter);
		sheetWidth[rowName-1] = 0;

		while(getline(stream,buffer,'\t'))
		{
			trimLeft(buffer);
			if( buffer[0] != '\'' && !isdigit(buffer[0]) )
				trim(buffer);
			sheet[colName+to_string(rowName)] = buffer;
			getColumnName(colName);
			sheetWidth[rowName-1]++;
		}
		
		rowName++;
	}
}

SpreadSheet::~SpreadSheet()
{
	delete [] sheetWidth;
}

void SpreadSheet::getColumnName(string& string)
{
	int nZCount = 0;
	int strLen = string.length();
	bool isPrevZ = false;
	int i;
	
	for(i = strLen-1; i >= 0; i--)
	{
		if(string[i] == 'Z')
		{
			nZCount++;
			isPrevZ = true;
			
			if(i != 0)
				string[i] = 'A';
		}
		else if(isPrevZ || i == strLen-1)
		{
			string[i]++;
			isPrevZ = false;
		}
	}
	
	if(nZCount == strLen)
	{
		for(i = 0; i < strLen; i++)
			string[i] = 'A';
		string.append("A");
	}
}

vector<string> SpreadSheet::getSheet()
{
	string buffer,colName,coordinates;
	vector<string> resSheet;
	
	for(int i = 1; i <= this->sheetLenght; i++) 
	{		
		colName = "A";
		buffer = "";
		
		for(int j = 1; j <= this->sheetWidth[i-1]; j++)
		{
			coordinates = colName + to_string(i);
			buffer +=  coordinates + ": " + this->sheet[coordinates] + " ";
			getColumnName(colName);
		}
		
		buffer+="\n";
		resSheet.push_back(buffer);
	}
	return resSheet;
}

void SpreadSheet::evaluate()
{
	Concurrency::parallel_for_each(this->sheet.begin(),this->sheet.end(),
		[=](const pair<string,string> iter)
		{
			if( isdigit(iter.second[0]) )
			{
				if( !isNumber(iter.second) )
					this->sheet[iter.first] = Error::BAD_SYNTAX;
			}
			else if(iter.second[0] == '-')
			{
				if(!isdigit(iter.second[1]))
					this->sheet[iter.first] = Error::BAD_SYNTAX;
				else if( !isNumber(iter.second) )
					this->sheet[iter.first] = Error::BAD_SYNTAX;
			}
			else if(iter.second[0] != '=' && iter.second[0] != '\'' && !iter.second.empty())
			{
				this->sheet[iter.first] = Error::BAD_SYNTAX;
			}
		});
	
	for(auto iter : this->sheet)
	{
		if(iter.second[0] == '=')
			this->sheet[iter.first] = expression(iter.first,iter.second);
	}
	
	Concurrency::parallel_for_each(this->sheet.begin(),this->sheet.end(),
		[=](const pair<string,string> iter)
	{
		if(iter.second[0] == '\'')
			this->sheet[iter.first] = label(iter.second);
	});
}

string SpreadSheet::expression(const string address,string cell)
{

	if( cell.find(address) != string::npos )
		return Error::BAD_REFERENCE;
	
	cell.erase(0,1);
	
	size_t currentPos, lastPos = 0;
	int lastLen, currentLen;
	string operand;

	currentPos = cell.find_first_of("+-*/");
	operand = cell.substr(lastPos,currentPos);
	
	if( currentPos == 0 )
	{
		if( cell[currentPos] == '-' && operand.empty() )
		{
			lastPos = 1;
			currentPos = cell.find_first_of("+-*/",lastPos);
			operand = cell.substr(lastPos,currentPos-1);
		}
		else
			return Error::BAD_SYNTAX;
	}
	
	currentLen = cell.length();
	lastLen = currentLen;

	while( currentPos != string::npos+1 )
	{ 
		 if ( operand.empty() )
		 {
			return Error::BAD_SYNTAX;
		 }
		 else if( isdigit(operand[0]) || operand[0] == '-' ) 
		 {
			 if( !isNumber(operand) )
					return Error::BAD_SYNTAX;
		 }
		 else if( !isReference(operand) )
		 {	 
			 return Error::BAD_REFERENCE;
		 }
		 else
		 {
			 if( this->sheet[operand].find(address) != string::npos)
				 return Error::BAD_REFERENCE;
			 
			 
			 if( this->sheet[operand][0] == '=' )
				 this->sheet[operand] = expression(operand, this->sheet[operand]);
			 else if(this->sheet[operand][0] == '\'')
				 return Error::BAD_REFERENCE;
			 else if( this->sheet[operand].empty() )
				 return Error::BAD_REFERENCE;
			 else if(this->sheet[operand] == Error::BAD_REFERENCE || this->sheet[operand] == Error::BAD_SYNTAX)
				 return Error::BAD_REFERENCE;


			 cell.erase(lastPos,operand.length());
			 cell.insert(lastPos,this->sheet[operand]);
			 currentLen = cell.length();
		 }
		  
		 if( currentPos != string::npos )
		 {
			lastPos = currentPos + (currentLen-lastLen) + 1;
			currentPos = cell.find_first_of("+-*/",lastPos);
			operand = cell.substr(lastPos,currentPos-lastPos);
			
			if( currentPos != string::npos )
			{
				if( cell[currentPos] == '-' && operand.empty() )
				{
					lastPos = currentPos + 1;
					currentPos = cell.find_first_of("+-*/",lastPos);
					operand = cell.substr(lastPos,currentPos-lastPos);
				}
			}
			lastLen = currentLen;
		 }
		 else
			 currentPos++;
	}
	
	if(cell[0] == '-' && cell[1] == '-')
		cell = cell.substr(2);

	Evaluator eval;
	cell = eval.getResult(cell);

	return cell;
}

string SpreadSheet::label(string labelString)
{
	return labelString.substr(1);
}

bool SpreadSheet::isReference(string refStr)
{
	std::unordered_map<std::string,string>::const_iterator cell;
	cell = this->sheet.find(refStr);
	
	if( cell == this->sheet.end() )
		return false;
	else
		return true;
}

bool SpreadSheet::isNumber(string numbStr)
{
	size_t pos;
	stof(numbStr,&pos);
	if( pos == numbStr.length() )
		 return true;
	else
		return false;
}