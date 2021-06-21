#pragma once
#include <vector>

struct KVec2Test;
class StalicLib2
{
public:
	StalicLib2() {};
	~StalicLib2() {};
protected:
	
public:
	void UseStructInOhterLibAndCalledByOtherLib(KVec2Test v2Test);
	void UseStructInOhterLibAndCalledByOtherLib1(std::vector<KVec2Test>& Vec2TestList);
private:
};