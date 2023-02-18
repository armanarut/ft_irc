#include <iostream>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <map>

void	prog_error(std::string err)
{
	if (errno)
		perror(err.c_str());
	else
		std::cerr << err << std::endl;
	exit(1);
}

bool	is_valid(std::string str)
{
	std::string::iterator ch = str.begin();
	if (!std::isalpha(*ch))
		return false;
	for (; ch != str.end(); ++ch)
		if (!std::isprint(*ch))
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

std::vector<std::string>  splitStr(const std::string &arg)
{
    std::vector<std::string> ret;
    std::string temp;
    size_t argSize = arg.size();
    size_t pos0 = 0;
    size_t pos;
    while((pos = arg.find(",", pos0)) <= argSize)
    {
        temp = arg.substr(pos0, pos - pos0);
        ret.push_back(temp);
        temp = "";
        pos0 = pos + 1;
    }
    temp = arg.substr(pos0, pos);
    ret.push_back(temp);
    return ret;
}

std::map<std::string, std::string> strTransMap(std::string &channel, std::string &passw)
{
    std::map<std::string, std::string> ret;
    std::vector<std::string> ch = splitStr(channel);
    std::vector<std::string> pass = splitStr(passw);
    size_t chan = ch.size();
    size_t vpas = pass.size();
    for (size_t i = 0; i < chan; i++) {
        
        if (vpas > i)
            ret[ch[i]] = pass[i];
        else
            ret[ch[i]] = "";
    }
    return ret;
}