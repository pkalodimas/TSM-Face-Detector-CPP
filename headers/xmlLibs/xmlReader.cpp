
#ifndef XMLREADER_CPP_
#define XMLREADER_CPP_

#include "xmlReader.h"

namespace tsm {

template <typename T>
xmlReader<T>::xmlReader() {
	this->xmlFilePath = nullptr;
}

template <typename T>
xmlReader<T>::xmlReader(std::string *filepath){
	this->xmlFilePath = filepath;
}

template <typename T>
xmlReader<T>::~xmlReader() {
	delete this->xmlFilePath;
}

template <typename T>
Model<T>* xmlReader<T>::xml2Model(xml_document<> *doc){

	Model<T> *model = new Model<T>();
	xml_node<> *xmlModel = doc->first_node("model");

	//model->obj = (T) atof(xmlModel->first_node("obj")->value());
	model->thresh = (T) atof(xmlModel->first_node("thresh")->value());
	//model->delta = (T) atof(xmlModel->first_node("delta")->value());

	model->sbin = (int) atoi(xmlModel->first_node("sbin")->value());
	model->interval = (int) atoi(xmlModel->first_node("interval")->value());
//	model->len = (int) atoi(xmlModel->first_node("len")->value());

	//cout << "1";
	model->maxsize = xmlReader<int>::xml2Vector(xmlModel->first_node("maxsize"));
	//cout << "1";
	model->defs = xmlReader<T>::xml2Defs(xmlModel->first_node("defs"));
	//cout << "1";
	model->filters = xmlReader<T>::xml2Filters(xmlModel->first_node("filters"));
	//cout << "1";
	model->components = xmlReader<T>::xml2Components(xmlModel->first_node("components"));
	//cout << "1";

	return model;
}

template <typename T>
Model<T>* xmlReader<T>::readXMLModel(std::string *filepath){

	rapidxml::file<> xmlFile(filepath->c_str());
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	Model<T> *model = xmlReader<T>::xml2Model(&doc);
	return model;
}

template <typename T>
Model<T>* xmlReader<T>::readXMLModel(){

	return this->xml2Model(this->xmlFilePath);
}

template <typename T>
Settings<T>* xmlReader<T>::xml2Settings(xml_document<> *doc){

	if( !doc ) return nullptr;

	Settings<T> *settings = new Settings<T>();

	xml_node<> *xmlSettings = doc->first_node("settings");

	settings->Threshold = (T) atof(xmlSettings->first_node("threshold")->value());
	settings->BoxCacheSize = (int) atoi(xmlSettings->first_node("boxCacheSize")->value());
	settings->NmsLimit = (int) atoi(xmlSettings->first_node("nmsLimit")->value());
	settings->NmsOverlap = (float) atof(xmlSettings->first_node("nmsOverlap")->value());
	settings->Interval = (int) atoi(xmlSettings->first_node("interval")->value());
	settings->MinLevel = (int) atoi(xmlSettings->first_node("minlevel")->value());
	settings->MaxLevel = (int) atoi(xmlSettings->first_node("maxlevel")->value());
	settings->OutputType = (int) atoi(xmlSettings->first_node("outputType")->value());
	settings->MinFace = (int) atoi(xmlSettings->first_node("faceHeight")->value());
	settings->FindBuffer = (int)atoi(xmlSettings->first_node("findBuffer")->value());
	settings->Version = (int)atoi(xmlSettings->first_node("version")->value());

	return settings;
}

template <typename T>
Settings<T>* xmlReader<T>::readXMLSettings(std::string *filepath){

	rapidxml::file<> xmlFile(filepath->c_str());
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	Settings<T> *settings = xmlReader<T>::xml2Settings(&doc);
	return settings;
}

template <typename T>
Settings<T>* xmlReader<T>::readXMLSettings(){

	return this->xml2Settings(this->xmlFilePath);
}

} /* namespace tsm */

#endif

