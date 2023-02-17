#include <iostream>
#include <sstream>
#include <fstream>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <vector>
#include <algorithm>


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

std::string my_to_string(int num)
{
    std::stringstream stream;
    std::string str;
    stream << num;
    stream >> str;
    return str;
}

void  toUpperCase(std::string& str)
{
    for(size_t i = 0 ; i < str.size(); ++i) 
    { str[i] = std::toupper(str[i]); }
}

std::string get_time(void)
{
    time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime ( &rawtime );
    std::string dt = asctime (timeinfo);
    std::vector<std::string>  data_time;
    std::string temp;
    int pos = 0;
    
    temp = dt.substr(pos, dt.find(" ", pos) - pos);
    while(!temp.empty())
    {
        while(dt[pos] == ' ') pos++;
        temp = dt.substr(pos, dt.find(" ", pos + 1) - pos);
        pos += temp.size();
        data_time.push_back(temp);
    }
    data_time[4].resize(4);
    temp = " DATE : /" + data_time[0] + "/ " + data_time[2] + " ";
    temp += data_time[1] + " " + data_time[4] + "\n";
    temp += "TIME : " + data_time[3];
    return temp;
}

std::string    ltrim(const std::string &s, const std::string &trimed)
{
    size_t start = s.find_first_not_of(trimed);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string    rtrim(const std::string &s, const std::string &trimed)
{
    size_t end = s.find_last_not_of(trimed);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string    trim(const std::string &s, const std::string &trimed) { 
    return rtrim(ltrim(s, trimed), trimed);
}