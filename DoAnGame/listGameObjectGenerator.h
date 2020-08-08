#pragma once
#include <vector>
#include <string>

using namespace std;

class listGameObjectGenerator
{
private:
	std::vector<string> listClass;

	std::string generateEnum();
	std::string generateInclude();
	std::string generateInitFunction();
	std::string generateReflectFunction();
public:
	static listGameObjectGenerator* Instance;

	static listGameObjectGenerator* getInstance();

	void getListClass();

	void generateHeader();
	void generateBody();
};

