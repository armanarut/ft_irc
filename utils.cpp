#include <iostream>
#include <cerrno>
#include <cstdio>
#include <cstdlib>

void	prog_error(std::string err)
{
	if (errno)
		perror(err.c_str());
	else
		std::cerr << err << std::endl;
	exit(1);
}
