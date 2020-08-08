#pragma once
#include "./define.h"
#include <vector>

using namespace rapidxml;
using namespace std;

typedef vector<char>* stringBuffer;

class database
{
private:
	static database* Instance;

	stringBuffer rootBuffer; 

	xml_document<> rootDocument;

public:
	static database* getInstance();

	void getData(const char* dataPath);

	xml_node<>* getRoot(const char* name);
};
