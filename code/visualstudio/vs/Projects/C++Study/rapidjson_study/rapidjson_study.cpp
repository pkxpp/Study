// rapidjson_study.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <string>

#include "string_study.h"

void WriteTestJson()
{
	FILE *fp = NULL;

	fopen_s(&fp, "test.json", "wt+");
	if (fp == 0)
	{
		printf("can't open file\n");
		return;
	}
	fseek(fp, 0, SEEK_END);
	long filelen = ftell(fp);
	rewind(fp);

	rapidjson::Document doc;
	rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
	doc.SetObject();

	rapidjson::Value strIndex;
	rapidjson::Value strContent(rapidjson::kStringType);
	std::string strIndex1;
	for (int i = 0; i <2; ++i)
	{
		strContent.SetString("test");
		
		// way1: error
		//doc.AddMember(rapidjson::StringRef(std::to_string(i).c_str()), strContent, allocator);

		// way2: error
		strIndex1 = std::to_string(i);
		//doc.AddMember(rapidjson::StringRef(strIndex1.c_str()), strContent, allocator);

		// way3: correct
		strIndex.SetString(std::to_string(i).c_str(), allocator);
		doc.AddMember(strIndex, strContent, allocator);
	}

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	fwrite(buffer.GetString(), buffer.GetSize(), 1, fp);
	fclose(fp);
}

int main()
{
	WriteTestJson();
    return 0;
}

