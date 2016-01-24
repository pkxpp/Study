#include <iostream>
using namespace std;

// NamedObject1 
template <typename T>
class NamedObject1{
public:
	NamedObject1(const char* name, const T& value);
	NamedObject1(const std::string& name, const T& value);

private:
	std::string nameValue;
	T objectValue;
};

// NamedObject1 complete
template<typename T>
NamedObject1<T>::NamedObject1(const char* name, const T& value){
	nameValue = name;
	objectValue = value;
	std::cout << "nameValue: " << nameValue << std::endl;
	std::cout << "objectValue: " << objectValue << std::endl;
}


// NamedObject2 
template <class T>
class NamedObject2{
public:
	NamedObject2(const char* name, const T& value);
	NamedObject2(std::string& name, const T& value);

private:
	//std::string nameValue;
	std::string& nameValue;
	const T objectValue;
};

// NamedObject2 complete
template<typename T>
NamedObject2<T>::NamedObject2(const char* name, const T& value)
:nameValue(name), objectValue(value)
{
	//nameValue = name;
	//objectValue = value;
	std::cout << "nameValue: " << nameValue << std::endl;
	std::cout << "objectValue: " << objectValue << std::endl;
}


// NamedObject2 complete
template<typename T>
NamedObject2<T>::NamedObject2(std::string& name, const T& value){
	nameValue = name; //05.h:51: error: uninitialized member ‘NamedObject2<int>::objectValue’ with ‘const’ type ‘const int’
	//objectValue = value;
	std::cout << "string_nameValue2: " << nameValue << std::endl;
	std::cout << "string_objectValue2: " << objectValue << std::endl;
}
