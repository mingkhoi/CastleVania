#include "./listGameObjectGenerator.h"
#include "./database.h"
#include "./define.h"
#include "./game.h"

#include <fstream>
#include <unordered_set>

using namespace std;

listGameObjectGenerator* listGameObjectGenerator::Instance;

listGameObjectGenerator* listGameObjectGenerator::getInstance() {
	if (Instance == NULL)
		Instance = new listGameObjectGenerator();
	return Instance;
}

void listGameObjectGenerator::getListClass() {
	unordered_set<string> data;
	xml_n* root = database::getInstance()->getRoot("start_screen");

	for (xml_n* scene = root; scene; scene = scene->next_sibling()) {
		string nodeName = (string)scene->name();

		if (nodeName == "singleton" || nodeName == "mapGameObject")
			continue;

		xml_n* mapComponent = scene->first_node("map_component");
		xml_n* gameObject = scene->first_node("game_object");


		for (xml_n* component = mapComponent->first_node(); component;
			component = component->next_sibling()) {
			data.insert((string)component->name());
		}

		for (xml_n* component = gameObject->first_node(); component;
			component = component->next_sibling()) {
			string componentName = (string)component->name();

			if (componentName.size() > 2 && componentName[0] == '_' && componentName[1] == '_') {
				data.insert(componentName.substr(2));
			}
			else data.insert(componentName);
		}
	}

	vector<string> result;
	result.reserve(data.size());

	for (unordered_set<string>::iterator it = data.begin();
		it != data.end(); it++) {
		result.push_back(*it);
	}

	listClass = result;
}

string listGameObjectGenerator::generateEnum() {
	string result;

	result += "enum classType {\n";

	for (auto& x : listClass) {
		result += "_" + x + ",\n";
	}
	result += "_NULL, \n};\n";

	return result;
}

string listGameObjectGenerator::generateInclude() {
	string result;

	result += "#include \"./listGameobject.h\"\n";
	result += "#include \"./gameObject.h\"\n";

	for (auto& x : listClass) {
		result += "#include\"./" + x + ".h\"\n";
	}
	return result;
}

string listGameObjectGenerator::generateInitFunction() {
	string result;

	result += "void listGameobject::init() {\n";

	for (auto& x : listClass) {
		result += "table[\"" + x + "\"]=_" + x + ";\n";
	}
	result += "}\n";

	return result;
}

string listGameObjectGenerator::generateReflectFunction() {
	string result;

	result += "LPGAMEOBJECT listGameobject::reflect(string className) {\n";
	result += "int id = table[className];\n";
	result += "switch (id) {\n";

	for (auto& x : listClass) {
		result += "case _" + x + ": return new " + x + "(Counter::index++);\n";
	}
	result += "default: return NULL;\n}\n";
	result += "}\n";

	return result;
}

void listGameObjectGenerator::generateHeader() {
	xml_n* root = database::getInstance()->getRoot("mapGameObject")->
		first_node("header");

	string fileName = (string)root->value();

	ofstream file(fileName);

	file << "#pragma once\n";
	file << "#include \"./gameObject.h\"\n";
	file << "#include <string>\n";
	file << "#include <unordered_map>\n";
	file << "using namespace std;";

	file << "class listGameobject {\npublic:\n";

	file << generateEnum();

	file << "static listGameobject *Instance;\n";
	file << "unordered_map<string, classType> table;\n";
	file << "static listGameobject *getInstance();\n";
	file << "void init();\n";
	file << "LPGAMEOBJECT reflect(string className);\n};\n";
}

void listGameObjectGenerator::generateBody() {
	xml_n* root = database::getInstance()->getRoot("mapGameObject")->
		first_node("body");

	string fileName = (string)root->value();

	ofstream file(fileName);

	file << generateInclude();

	file << "listGameobject *listGameobject::Instance;\n";
	file << "listGameobject *listGameobject::getInstance() {\n";
	file << "if (Instance == NULL) Instance = new listGameobject();\n";
	file << "return Instance;\n}\n";

	file << generateInitFunction();

	file << generateReflectFunction();
}