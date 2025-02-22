#include <iostream>

#include "parserWrapper.h"
#include "exprtk.hpp"

float parseExpression(const std::string& expression)
{
	exprtk::parser<double> parser;
	exprtk::expression<double> expr;

	if (parser.compile(expression, expr))
	{
		float result = expr.value();	
		return result;
	}
	else
	{
		std::cerr << "Error: " << parser.error() << std::endl;
		return 0;
	}
}