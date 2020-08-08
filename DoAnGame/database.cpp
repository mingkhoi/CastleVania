#include "./database.h"
#include "./game.h"
#include <sstream>
#include <string>

database* database::Instance = NULL;

database* database::getInstance() {
	if (Instance == NULL)
		Instance = new database();
	return Instance;
}

void database::getData(const char* dataPath) {
	std::ifstream inputStream(dataPath);

	rootBuffer = new vector<char>((istreambuf_iterator<char>(inputStream)),
		istreambuf_iterator<char>());
	rootBuffer->push_back('\0');

	this->rootDocument.parse<parse_declaration_node | 
		parse_no_data_nodes>(&(*rootBuffer)[0]);
	
	inputStream.close();
}

xml_node<>* database::getRoot(const char* name) {
	xml_node<>* temp = this->rootDocument.first_node();

	return temp->first_node(name);
}
