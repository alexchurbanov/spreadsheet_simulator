#include "Evaluator.h"

string Evaluator::getResult(string formula)
{
	size_t indx;
	list<float> numbers;
	list<char> operators;

	trim(formula);

	while(true)
	{
		numbers.push_back(stof(formula,&indx));
		
		if(indx+1 <= formula.length())
		{
			operators.push_back(formula[indx]);
			formula = formula.substr(indx+1);
		}
		else
			break;
	}
	
	if(operators.empty() && numbers.size() == 1)
		return toString(numbers.front());


	string operFirstPrior = "*/";
	int pos,i = 0;
	float result,leftOperand,rightOperand;
	char oper;

	list<float>::iterator nIter;
	list<char>::iterator opIter;

	while(!operators.empty())
	{
		if(i <= 1)
			pos = findPos(operators.begin(),operators.end(),operFirstPrior[i]);
		else
			pos = 0;
		
		if( pos != -1)
		{
			nIter = numbers.begin();
			
			if(i <= 1)
				opIter = find(operators.begin(),operators.end(),operFirstPrior[i]);
			else
				opIter = operators.begin();

			for(int j = 0; j < pos; j++)
				nIter++;

			oper = *opIter;
			leftOperand = *nIter;
			rightOperand = *++nIter;
			
			switch(oper)
			{
				case '*':
						  result = leftOperand * rightOperand;
						  break;
				case '/':
						  if( rightOperand == 0 ) 
							  return Error::BAD_SYNTAX;
						  result = leftOperand / rightOperand;
						  break;
				case '+':
						  result = leftOperand + rightOperand;
						  break;
				case '-':
						  result = leftOperand - rightOperand;
						  break;
			}		
			
			operators.erase(opIter);
			numbers.erase(nIter--);
			numbers.erase(nIter++);
			numbers.insert(nIter,result);
		}
		else 
			i++;
	}
	
	return toString(numbers.front());
}
	
int findPos(list<char>::iterator first,list<char>::iterator last,char character)
{
	int i = 0;
	while(first != last)
	{
		if(*first == character)
			return i;
		i++;
		first++;
	}
	return -1;
}

void trim(string& string)
{
	while( string.find(' ') != string::npos )
	{
		string.erase(string.find(' '),1);
	}
}

string toString(float number)
{
	float fract,intPart;
	fract = modf(number,&intPart);
	if(fract == 0)
		return to_string(int(number));
	else
	{
		string str = to_string(number);
		while(str[str.length()-1] == '0')
			 str.pop_back();
		return str;
	}
}

void trimLeft(string& string)
{
	int i = 0;

	while( string[i] == ' ' && !string.empty() )
	{
		string.erase(i,1);
	}
}