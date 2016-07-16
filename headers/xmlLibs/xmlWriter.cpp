
#ifndef XMLWRITER_CPP_
#define XMLWRITER_CPP_

#include "xmlWriter.h"

using namespace rapidxml;

namespace tsm {

template <typename T>
xmlWriter<T>::xmlWriter() {
	this->xmlFilePath = nullptr;
	this->xmlString = nullptr;
}

template <typename T>
xmlWriter<T>::xmlWriter(std::string *filepath){
	this->xmlString = nullptr;
	this->xmlFilePath = filepath;
}

template <typename T>
xmlWriter<T>::~xmlWriter() {
	delete this->xmlString;
	delete this->xmlFilePath;
}

template <typename T>
std::string *xmlWriter<T>::boxes2xml(std::string *xmlString, Vector< Box<T> > *boxes){

	if( !boxes || boxes->length < 1 ) return nullptr;

	xml_document<> xmlDoc;
	xml_node<> *xmlBoxes = xmlDoc.allocate_node(node_element, "Boxes", 0, 0, 0);

	for( int i=0; i<boxes->length; i++ ){

		//const char *strnum = nullptr;
		xml_node<> *xmlBox = xmlDoc.allocate_node(node_element, "Box", 0, 0, 0);
		//Box.s
		xml_node<> *xmlnodes = xmlDoc.allocate_node(node_element, "s", (num2XML(boxes->data[i].s))->c_str(), 0, 0);
		xmlBox->append_node(xmlnodes);
		//Box.c
		xmlnodes = xmlDoc.allocate_node(node_element, "c", (num2XML(boxes->data[i].c))->c_str(), 0, 0);
		xmlBox->append_node(xmlnodes);
		//Box.level
		xmlnodes = xmlDoc.allocate_node(node_element, "level", (num2XML(boxes->data[i].level))->c_str(), 0, 0);
		xmlBox->append_node(xmlnodes);
		//Box.xy
		xmlnodes = xmlDoc.allocate_node(node_element, "xy", (arrayData2XML(boxes->data[i].xy))->c_str(), 0, 0);
		xmlBox->append_node(xmlnodes);

		xmlBoxes->append_node(xmlBox);
	}

	xmlDoc.append_node(xmlBoxes);
	if( !xmlString ) xmlString = new std::string();
	rapidxml::print(std::back_inserter(*xmlString), xmlDoc, 0);

	return xmlString;
}

template <typename T>
bool xmlWriter<T>::boxes2xml(Vector< Box<T> > *boxes){

	this->xmlString = this->boxes2xml(this->xmlString, boxes);
	if( !this->xmlString ) return false;
	return true;
}

template <typename T>
bool xmlWriter<T>::writeXMLBoxes(Vector< Box<T> > *boxes, std::string *filepath){

	std::string *xmlboxes = xmlWriter<T>::boxes2xml(nullptr, boxes);
	xmlWriter<T>::writeXML(xmlboxes, filepath);
	delete xmlboxes;
	return true;
}

template <typename T>
bool xmlWriter<T>::writeXML(std::string *xmlString, std::string *filepath) {

	if (!xmlString || !filepath) return false;

	FILE *fd = fopen(filepath->c_str(), "w");
	if (!fd) return false;

	int bytes = fwrite(xmlString->c_str(), xmlString->size(), 1, fd);
	if (bytes != xmlString->size()) return false;

	fclose(fd);
	return true;
}

template <typename T>
bool xmlWriter<T>::writeXML(){

	return this->writeXML(this->xmlString, this->xmlFilePath);
}

} /* namespace tsm */

#endif

