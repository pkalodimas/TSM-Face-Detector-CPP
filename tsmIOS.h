
#ifndef TSMIOS_CPP_
#define TSMIOS_CPP_

#include <string.h>
#include <iostream>
#include <stdio.h>
#include "headers/tsmLibs/Vector.cpp"
#include "headers/openCV/OpenCVHandler.cpp"
#include "headers/xmlLibs/xmlWriter.cpp"
#include "headers/tsmLibs/Settings.cpp"

using namespace tsm;

tsm::Vector< tsm::Vector<char> >* getArguments(char *argumentsText){

	tsm::Vector< tsm::Vector<char> > *args = new tsm::Vector< tsm::Vector<char> >();

	//Count the number of arguments
	for( int i=0; i<(int)strlen(argumentsText); i++ ){

		int c = strcspn(&argumentsText[i]," \n\0");
		if( c > 0 ) {
			args->length++;
			i += c;
		}
	}
	if( args->length == 0 )  return args;

	//Set the arguments to the arguments vector
	args->init(args->length);

	args->data[0].data = strtok(argumentsText, " ");
	args->data[0].length = strlen(args->data[0].data);
	for( int i=1; i<args->length; i++ ){

		args->data[i].data = strtok(NULL, " \n");
		args->data[i].length = strlen(args->data[0].data);
	}

	return args;
}

tsm::Vector< tsm::Vector<char> > *getArguments(char *argv[], int argc){

	tsm::Vector< tsm::Vector<char> > *args = new tsm::Vector< tsm::Vector<char> >(argc-1);

	//Count the number of arguments
	for( int i=1; i<argc; i++ ){

		args->data[i-1].length = strlen(argv[i]);
		args->data[i-1].data = argv[i];
	}

	return args;
}

template <typename T>
bool setOutput(tsm::Vector< Box<T> > *boxes, Settings<T> *sets, OpenCVHandler<T> *image){
	
	if ( !boxes || !sets ) throw ERROR_OUTPUT_CODE;

	if (sets->OutputType & 1) {

		size_t ext = image->imagePath->find_last_of('.', image->imagePath->length());
		std::string xmlpath = image->imagePath->substr(0, ext) +".xml";
		xmlWriter<T>::writeXMLBoxes(boxes, &xmlpath);
	}
	if (sets->OutputType & 2) {

		image->drawBoxes2Image(boxes);
		size_t ext = image->imagePath->find_last_of('.', image->imagePath->length());
		std::string savepath = image->imagePath->substr(0, ext) + "-detected" + image->imagePath->substr(ext, image->imagePath->length() - ext);
		image->saveImage(&savepath);
	}
	if (sets->OutputType & 4) {

		image->drawBoxes2Image(boxes);
		image->showImage(image->imagePath);
	}

	return true;
}

#endif /* TSMIOS_CPP_ */
