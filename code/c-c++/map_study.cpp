/*#include<map>  
#include<string>  
#include<iostream>  
using namespace std;  
  
typedef pair<string, int> PAIR;  
  
ostream& operator<<(ostream& out, const PAIR& p) {  
  return out << p.first << "\t" << p.second;  
}  
  
int main() {  
  map<string, int> name_score_map;  
  name_score_map["LiMin"] = 90;   
  name_score_map["ZiLinMi"] = 79;   
  name_score_map["BoB"] = 92;   
  name_score_map.insert(make_pair("Bing",99));  
  name_score_map.insert(make_pair("Albert",86));  
  for (map<string, int>::iterator iter = name_score_map.begin();  
       iter != name_score_map.end();  
       ++iter) {  
    cout << *iter << endl;  
  }  
  return 0;  
}*/

#include<map>  
#include<string>  
#include<iostream>  
using namespace std;  
  
typedef pair<string, int> PAIR;  
  
ostream& operator<<(ostream& out, const PAIR& p) {  
  return out << p.first << "\t" << p.second;  
}  
  
/*int main() {  
  map<string, int, greater<string> > name_score_map;  
  name_score_map["LiMin"] = 90;   
  name_score_map["ZiLinMi"] = 79;   
  name_score_map["BoB"] = 92;   
  name_score_map.insert(make_pair("Bing",99));  
  name_score_map.insert(make_pair("Albert",86));  
  for (map<string, int>::iterator iter = name_score_map.begin();  
       iter != name_score_map.end();  
       ++iter) {  
    cout << *iter << endl;  
  }  
  return 0;  
}*/

struct CmpByKeyLength {  
  bool operator()(const string& k1, const string& k2) {  
    return k1.length() < k2.length();  
  }  
};

////////////////////////////////////////////////////////
// 2.map[] need be careful
void TestMapOperation()
{
	map<int, int> mapTest;
	int id = 1;

	if (mapTest[id] != 0)
	{
		cout << "Oh, It's right!!!" << endl;
	}
	else
	{
		cout << "mapTest[id] = " << mapTest[id] << ", But I have no data in map." << endl;
	}
}

int main() {  
/*
  map<string, int, CmpByKeyLength> name_score_map;  
  name_score_map["LiMin"] = 90;   
  name_score_map["ZiLinMi"] = 79;   
  name_score_map["BoB"] = 92;   
  name_score_map.insert(make_pair("Bing",99));  
  name_score_map.insert(make_pair("Albert",86));  
  for (map<string, int>::iterator iter = name_score_map.begin();  
       iter != name_score_map.end();  
       ++iter) {  
    cout << *iter << endl;  
  }*/  
	// 2. test map[]
	TestMapOperation();
  return 0;  
}    
