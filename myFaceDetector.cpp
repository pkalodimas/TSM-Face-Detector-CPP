/*
 * main.cpp
 *
 *  Created on: 13 Ã·¿ 2016
 *      Author: null0llun
 */

#define OMP_ENABLE
#define TIME_PROFILE
#define PRINT_ERRORS

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>

#include "tsmIOS.h"
#include "headers/tsmLibs/Image.h"
#include "headers/tsmLibs/Settings.cpp"
#include "headers/tsmLibs/Model.cpp"
#include "headers/tsmLibs/FaceDetector.cpp"
#include "headers/xmlLibs/xmlReader.cpp"
#include "headers/xmlLibs/xmlWriter.cpp"
#include "headers/openCV/OpenCVHandler.cpp"
#include "headers/tsmLibs/ErrorCodes.h"

#ifdef OMP_ENABLE
	#include <omp.h>
#endif
#ifdef TIME_PROFILE
	#include <omp.h>
#endif

using namespace tsm;

void sig_handler(int signum)
{
	#ifdef PRINT_ERRORS
    printf("Error %d : Unknown OS Error (OS signal)!\n", SIGSEGV);
	#endif
	exit( ERROR_OS_CODE );
}

int main( int argc, const char* argv[] )
{
	signal(SIGSEGV, sig_handler);	// Segmentation fault (core dump) OS Signal handler
	signal(SIGABRT, sig_handler);	// Aborted (core dump)

try{
	#ifdef OMP_ENABLE
		omp_set_nested(true);
	#endif;

	tsm::Vector< tsm::Vector<char> > *args = getArguments((char**) argv, argc);

	while( args->length < 2 || args->length > 3 ){

		char arguments[200];
		printf("Please set your arguments [image model settings]: ");
		fgets(arguments, 199, stdin);
		args = getArguments(arguments);
	}

	std::string *image_path = new std::string(args->data[0].data);
	std::string *model_path = new std::string(args->data[1].data);
	std::string *settings_path = ( args->length > 2 ) ? new std::string(args->data[2].data) : nullptr;

	#ifdef TIME_PROFILE
		double readDataTime = omp_get_wtime();
	#endif

	OpenCVHandler<float> imageHandler;
	imageHandler.imagePath = image_path;
	if( !imageHandler.readImage() ) throw ERROR_READ_IMAGE_CODE;

	// Read Image
	Image<uint8_t> *image = (Image<uint8_t>*) imageHandler.cv2ArrayImage();
	if( !image ) throw ERROR_READ_IMAGE_CODE;

	// Read settings
	Settings<float> *settings = xmlReader<float>::readXMLSettings(settings_path);
	if( !settings ) throw ERROR_READ_SETTINGS_CODE;

	// Read Model
	Model<float> *model = xmlReader<float>::readXMLModel(model_path);
	if( !model ) throw ERROR_READ_MODEL_CODE;

	#ifdef TIME_PROFILE
		readDataTime = omp_get_wtime() - readDataTime;
	#endif

	#ifdef TIME_PROFILE
		double detectTime = omp_get_wtime();
	#endif

	// Face Detection call
	FaceDetector<float> *fdetector = new FaceDetector<float>(model, settings, image);
	if ( !fdetector->detect() ) throw fdetector->errorCode;
	tsm::Vector< Box<float> > *results = fdetector->boxCache->getResults();

	#ifdef TIME_PROFILE
		detectTime = omp_get_wtime() - detectTime;
	#endif

	#ifdef TIME_PROFILE
		cout << "TSM Face Detection System Time Profile\n";
		cout << "1. Read Data      : " << readDataTime << "sec\n";
		cout << "2. Face Detection : " << detectTime << "sec\n";
	#endif

	// Return Results
	setOutput(results, settings, &imageHandler);

	delete model;
	delete image;
	delete results;
	delete settings;

}
catch(int code){
	#ifdef PRINT_ERRORS
	cout << "TSM System Error Code " << code <<"!\n";
	#endif
	return code;
}
catch(...){
	cout << "TSM System Unkown Error Code!\n";
	return ERROR_OS_CODE;
}
	return 0;
}



