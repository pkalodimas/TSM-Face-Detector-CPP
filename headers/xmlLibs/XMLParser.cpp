
#ifndef XMLPARSER_CPP_
#define XMLPARSER_CPP_

#include "XMLParser.h"

namespace tsm {

template <typename T>
xmlParser<T>::xmlParser() {
}

template <typename T>
xmlParser<T>::~xmlParser() {
}

//--------------------------------------------------------------------------
//---------------------------- C to XML ------------------------------------

template <typename T>
std::string *xmlParser<T>::num2XML(T num) {

	std::string *str = new std::string();
	str->append(std::to_string(num));
	str->pop_back();

	return str;
}

template <typename T>
std::string *xmlParser<T>::arrayData2XML(Array<T>* table){

	if( !table ) return nullptr;

	std::string *str = new std::string();

	for( int i=0; i<table->length; i++ ){

		str->append(std::to_string(table->data[i]) + " ");
	}
	str->pop_back();

	return str;
}

//----------------------------------------------------------------------------
//------------------------ XML to C ------------------------------------------

template <typename T>
T* xmlParser<T>::getXmlArrayData(char *xmldata, T *data, int size){

	if( !xmldata ) return nullptr;
	if( !data )  data = new T[size];//
	char *xml = strtok(xmldata, " ");

	for( int i=0; i<size && xml; i++, xml = strtok(NULL, " ") ){ data[i] = (T) std::atof(xml); }

	return data;
}

template <typename T>
T* xmlParser<T>::getXmlArrayData(char *xmldata, int size){

	return getXmlArrayData(xmldata, nullptr, size);
}

template <typename T>
Array<T>* xmlParser<T>::xml2Array(xml_node<> *xmlArray, Array<T> *cArray){

	if( !xmlArray ) return nullptr;
	if( !cArray ) cArray = new Array<T>();

	xmlParser<int>::getXmlArrayData(xmlArray->first_attribute("size")->value(), cArray->dims, 3);
	cArray->length = cArray->dims[0] * cArray->dims[1] * cArray->dims[2];
	cArray->data = xmlParser<T>::getXmlArrayData(xmlArray->value(), cArray->length);

	return cArray;
}

template <typename T>
Array<T>* xmlParser<T>::xml2Array(xml_node<> *xmlArray){

	return xml2Array(xmlArray, nullptr);
}

template <typename T>
int xmlParser<T>::getXmlVectorSize(char *xmlstring){

	int *dims = xmlParser<int>::getXmlArrayData(xmlstring, 2);
	if( !dims ) return 0;
	return std::max(dims[0], dims[1]);
}

template <typename T>
Vector<T> *xmlParser<T>::xml2Vector(xml_node<> *xmlVector, Vector<T> *cVector){

	if( !cVector ) cVector = new Vector<T>();

	cVector->length = getXmlVectorSize(xmlVector->first_attribute("size")->value());
	cVector->data = xmlParser<T>::getXmlArrayData(xmlVector->value(), cVector->length);

	return cVector;
}

template <typename T>
Vector<T> *xmlParser<T>::xml2Vector(xml_node<> *xmlVector){

	return xml2Vector(xmlVector, nullptr);
}

//-------------------------------- Def --------------------------------
template <typename T>
Vector< Def<T> >* xmlParser<T>::xml2Defs(xml_node<> *xmlDefs, Vector< Def<T> > *defs){

	if( !xmlDefs ) return nullptr;
	if( !defs ) defs = new Vector< Def<T> >();

	defs->length = getXmlVectorSize(xmlDefs->first_attribute("size")->value());
	defs->init(defs->length);

	xml_node<> *xml_w = xmlDefs->first_node("w");
	xml_node<> *xml_i = xmlDefs->first_node("i");
	xml_node<> *xml_anchor = xmlDefs->first_node("anchor");

	for( int i=0; i<defs->length; i++ , xml_i = xml_i->next_sibling("i"), xml_w = xml_w->next_sibling("w"), xml_anchor = xml_anchor->next_sibling("anchor") ){

		//cout << "2";
		defs->data[i].i = (int) atoi(xml_i->value());
		defs->data[i].w = xmlParser<T>::xml2Vector(xml_w);
		defs->data[i].anchor = xmlParser<int>::xml2Vector(xml_anchor);
	}

	return defs;
}

template <typename T>
Vector< Def<T> >* xmlParser<T>::xml2Defs(xml_node<> *xmlDefs){

	return xml2Defs(xmlDefs, nullptr);
}

//------------------------------ Filters -----------------------------------------
template <typename T>
Vector< Filter<T>* > *xmlParser<T>::xml2Filters(xml_node<> *xmlFilters, Vector< Filter<T>* > *filters){

	if( !xmlFilters )  return nullptr;
	if( !filters ) filters = new Vector< Filter<T>* >();

	filters->length = getXmlVectorSize(xmlFilters->first_attribute("size")->value());
	filters->init(filters->length);

	xml_node<> *xml_i = xmlFilters->first_node("i");
	xml_node<> *xml_w = xmlFilters->first_node("w");

	for( int i=0; i<filters->length; i++ , xml_i = xml_i->next_sibling("i"), xml_w = xml_w->next_sibling("w") ){

		filters->data[i] = (Filter<T>*) xmlParser<T>::xml2Array(xml_w);
		filters->data[i]->i = (int) atoi(xml_i->value());
	}

	return filters;
}

template <typename T>
Vector< Filter<T>* > *xmlParser<T>::xml2Filters(xml_node<> *xmlfilters){

	return xml2Filters(xmlfilters, nullptr);
}

//----------------------------------- COMPONENT ------------------------------------------
template <typename T>
Component<T> *xmlParser<T>::xml2Component(xml_node<> *xmlComponent, Component<T> *component){

	if( !xmlComponent ) return NULL;
	if( !component ) component = new Component<T>();

	component->parts = new Vector< Part<T> >();
	component->parts->length = getXmlVectorSize(xmlComponent->first_attribute("size")->value());
	component->parts->init(component->parts->length);

	xml_node<> *xml_defid = xmlComponent->first_node("defid");
	xml_node<> *xml_filterid = xmlComponent->first_node("filterid");
	xml_node<> *xml_parent = xmlComponent->first_node("parent");

	for( int i=0; i<component->parts->length; i++ , xml_defid = xml_defid->next_sibling("defid"), xml_filterid = xml_filterid->next_sibling("filterid"), xml_parent = xml_parent->next_sibling("parent") ){

		component->parts->data[i].defid = (int) atoi(xml_defid->value());
		component->parts->data[i].filterid = (int) atoi(xml_filterid->value());
		component->parts->data[i].parent = (int) atoi(xml_parent->value());
	}

	return component;
}

template <typename T>
Component<T> *xmlParser<T>::xml2Component(xml_node<> *xmlComponent){

	return xml2Component(xmlComponent, nullptr);
}

//----------------------------------- COMPONENTS -----------------------------------------
template <typename T>
Vector< Component<T>* > *xmlParser<T>::xml2Components(xml_node<> *xmlComponents, Vector< Component<T>* > *components){

	if( !xmlComponents )  return NULL;
	if( !components ) components = new Vector< Component<T>* >();

	components->length = getXmlVectorSize(xmlComponents->first_attribute("size")->value());
	components->init(components->length);

	xml_node<> *xml_cell = xmlComponents->first_node("cell");

	for( int i=0; i<components->length; i++, xml_cell = xml_cell->next_sibling("cell") ) {

		components->data[i] = xml2Component(xml_cell);
		components->data[i]->id = i + 1;
	}

	return components;
}

template <typename T>
Vector< Component<T>* > *xmlParser<T>::xml2Components(xml_node<> *xmlComponents){

	return xml2Components(xmlComponents, nullptr);
}

} /* namespace tsm */

#endif
