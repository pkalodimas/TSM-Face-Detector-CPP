
#ifndef XMLREADER_H_
#define XMLREADER_H_

#include "../tsmLibs/Model.cpp"
#include "../tsmLibs/Settings.cpp"
#include "XMLParser.cpp"
#include "rapidXML/rapidxml_utils.hpp"

using namespace std;
using namespace rapidxml;

namespace tsm {

template <typename T>
class xmlReader: public xmlParser<T> {
	std::string *xmlFilePath;

public:
	xmlReader();
	xmlReader(std::string *filepath);
	virtual ~xmlReader();

	Model<T>* readXMLModel();
	Settings<T>* readXMLSettings();

	static Model<T>* xml2Model(xml_document<> *doc);
	static Model<T>* readXMLModel(std::string *filepath);
	static Settings<T>* xml2Settings(xml_document<> *doc);
	static Settings<T>* readXMLSettings(std::string *filepath);
};

} /* namespace tsm */

#endif /* XMLREADER_H_ */
