#include <iostream>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cctype>

void	prog_error(std::string err)
{
	if (errno)
		perror(err.c_str());
	else
		std::cerr << err << std::endl;
	exit(1);
}

bool	is_all_alpha(std::string str)
{
	for (std::string::iterator ch = str.begin(); ch != str.end(); ++ch)
		if (!std::isalpha(*ch))
			return false;
	return true;
}

bool	is_all_digit(std::string str)
{
	for (std::string::iterator ch = str.begin(); ch != str.end(); ++ch)
		if (!std::isdigit(*ch))
			return false;
	return true;
}