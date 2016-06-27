#include <sstream>

template<class T>
std::string make_color(T tvalue)
{
	char szPre[32] = {0};
	std::stringstream stream;
	stream << tvalue;
	std::string str = stream.str();
	snprintf(szPre, sizeof(szPre)-1, "<font color = \'%s\'", "#ff0000");
	str = szPre + str + "</font>";
	return str;
}

int main()
{
	make_color(5);
	return 0;
}
