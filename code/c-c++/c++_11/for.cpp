#include <iostream>

int main()
{
	std::string str = "hello, world";
	for (auto ch : str)
	{
		std::cout << ch << std::endl;
	}
	return 0;
}
