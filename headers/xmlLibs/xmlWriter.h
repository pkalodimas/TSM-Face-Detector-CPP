
#ifndef XMLWRITER_H_
#define XMLWRITER_H_

#include <string>
#include "../tsmLibs/Box.cpp"
#include "XMLParser.cpp"
#include "rapidXML/rapidxml_print.hpp"

namespace tsm {

template <typename T>
class xmlWriter: public xmlParser<T> {
	std::string *xmlString;
	std::string *xmlFilePath;
public:
	xmlWriter();
	xmlWriter(std::string *filepath);
	virtual ~xmlWriter();

	bool boxes2xml(Vector< Box<T> > *boxes);
	bool writeXML();

	static std::string* boxes2xml(std::string *xmlString, Vector< Box<T> > *boxes);
	static bool writeXML(std::string *xmlString, std::string *filepath);
	static bool writeXMLBoxes(Vector< Box<T> > *boxes, std::string *filepath);
};

} /* namespace tsm */

#endif /* XMLWRITER_H_ */
