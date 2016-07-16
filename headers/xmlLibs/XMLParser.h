
#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <stdio.h>
#include <string>
#include "../tsmLibs/Def.cpp"
#include "../tsmLibs/Filter.cpp"
#include "../tsmLibs/Component.cpp"
#include "rapidXML/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

namespace tsm {

template <typename T>
class xmlParser {
public:
	xmlParser();
	virtual ~xmlParser();

	static std::string *arrayData2XML(Array<T>* table);
	static std::string *num2XML(T num);
	static T* getXmlArrayData(char *xmldata, T *data, int size);
	static T* getXmlArrayData(char *xmldata, int size);
	static Array<T>* xml2Array(xml_node<> *xmlArray, Array<T> *cArray);
	static Array<T>* xml2Array(xml_node<> *xmlArray);
	static int getXmlVectorSize(char *xmlstring);
	static Vector<T> *xml2Vector(xml_node<> *xmlVector, Vector<T> *cvector);
	static Vector<T> *xml2Vector(xml_node<> *xmlVector);
	static Vector< Def<T> > *xml2Defs(xml_node<> *xmlDefs, Vector< Def<T> > *defs);
	static Vector< Def<T> > *xml2Defs(xml_node<> *xmlDefs);
	static Vector< Filter<T>* > *xml2Filters(xml_node<> *xmlFilters, Vector< Filter<T>* > *filters);
	static Vector< Filter<T>* > *xml2Filters(xml_node<> *xmlfilters);
	static Component<T> *xml2Component(xml_node<> *xmlComponent, Component<T> *component);
	static Component<T> *xml2Component(xml_node<> *xmlComponent);
	static Vector< Component<T>* > *xml2Components(xml_node<> *xmlComponents, Vector< Component<T>* > *components);
	static Vector< Component<T>* > *xml2Components(xml_node<> *xmlComponents);
};

} /* namespace tsm */

#endif /* XMLPARSER_H_ */
