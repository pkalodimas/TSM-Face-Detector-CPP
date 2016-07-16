
#ifndef PYRAMID_CPP_
#define PYRAMID_CPP_

#include "Pyramid.h"

namespace tsm {

// Constructor
template <typename T>
Pyramid<T>::Pyramid() {
	this->pyra = nullptr;
	this->scales = nullptr;
	this->imx = 0;
	this->imy = 0;
	this->interval = 0;
	this->padx = 0;
	this->pady = 0;
	this->levels = 0;
	this->MaxLevel = 0;
	this->MinLevel = 0;
	this->MinFace = 0;
	this->sBin = 1;
}

// Destructor
template <typename T>
Pyramid<T>::~Pyramid() {
	delete this->scales;
	this->scales = nullptr;

	delete this->pyra;
	this->pyra = nullptr;
}

// log2f
template <typename T>
T log2f(T num){
	return log(num)/log(2);
}

// Initialize Pyramid
template <typename T>
void Pyramid<T>::releaseLevel(int level){

	delete this->pyra->data[level];
	this->pyra->data[level] = nullptr;
}


// Initialize Pyramid
template <typename T>
bool Pyramid<T>::init(int interval, int sbin, int sourceDims[], Vector<int> *maxsize){

	if( !sourceDims || !maxsize ) return false;

	this->levels = 1 + (int) floor( log((double) min(sourceDims[0], sourceDims[1])/(5*sbin) ) / log( pow((float)2,(float)1/interval) ) );
	this->interval = interval;
	this->sBin = sbin;
	this->imx = sourceDims[0];
	this->imy = sourceDims[1];
	this->padx = max( maxsize->data[1]-2, 0);
	this->pady = max( maxsize->data[0]-2, 0);

	return true;
}

template <typename T>
bool Pyramid<T>::init(Model<T> *model, Image<uint8_t> *image){

	if( !model || !image ) return false;
	return this->init(model->interval, model->sbin, image->dims, model->maxsize);
}

template <typename T>
bool Pyramid<T>::setCustomPyramidSettings(int minlevel, int maxlevel, int minface){

	minlevel = max(minlevel, 0);
	maxlevel = max(maxlevel, 0);
	minface = max(minface, 0);

	if( maxlevel > 0 && maxlevel <= minlevel ) return false;
	this->MinLevel = minlevel;
	this->MaxLevel = maxlevel;
	this->MinFace = minface;
	this->levels = maxlevel - minlevel;
	return true;
}

// Create Pyramid Scales Vector
template <typename T>
Vector<float>* Pyramid<T>::createPyramidScales(int sourceDims[], int minlevel, int maxlevel, int sbin, int interval, int minface){

	int max_scale = 0;
	if( minface > 0 ) max_scale = (int) ceilf(log(sourceDims[1]/minface) / log(2) * interval);
	else max_scale = 1 + (int) floor( log((double) min(sourceDims[0], sourceDims[1])/(5*sbin) ) / log( pow((float)2,(float)1/interval) ) );

	minlevel = max(minlevel, 0);
	if( maxlevel > 0 ) maxlevel = min(maxlevel, max_scale);
	else maxlevel = max_scale;
	if( maxlevel <= minlevel ) return nullptr;

	Vector<float> *scales = new Vector<float>(maxlevel - minlevel);
	float sc = (float) pow((float)2,(float)1/interval);

	for( int i=minlevel; i<maxlevel; i++ ){

		scales->data[i-minlevel] = (float) pow(2,(float)(i+interval)/interval) * sbin/2;
	}

	return scales;
}

template <typename T>
Vector<float>* Pyramid<T>::createPyramidScales(int *sourceDims){

	this->scales = this->createPyramidScales(sourceDims, this->MinLevel, this->MaxLevel, this->sBin, this->interval, this->MinFace);
	return this->scales;
}


// Create Pyramid
template <typename T>
Vector< HOGImage<T>* >* Pyramid<T>::createPyramid(Image<uint8_t> *sourceImage, Vector<float> *scales, int interval, int sbin, Vector<int> maxsize[]){

	if( !sourceImage || !scales || !maxsize ) return nullptr;

	Vector< HOGImage<T>* > *pyra = new Vector< HOGImage<T>* >(scales->length);
	int pads[3] = { max( maxsize->data[1]-2, 0)+1 , max( maxsize->data[0]-2, 0)+1, 0 };

	Image<T> *im = new Image<T>(sourceImage->dims);
	for( int i=0; i<sourceImage->length; i++ ){ im->data[i] = (T) sourceImage->data[i]; }	// uint8 to double

	for( int i=0; i<min(scales->length, interval); i++ ){

		Image<T> *scaled = im->resize(sbin / scales->data[i]);
		if( !scaled ) throw ERROR_RESIZE_CODE;

		pyra->data[i] = scaled->features(sbin, pads);
		if( !pyra->data[i] ) throw ERROR_FEATURES_CODE;

		for( int j=i+interval; j<scales->length; j+=interval ){

			Image<T> *reduced = (Image<T>*) scaled->reduce();
			if( !reduced ) throw ERROR_REDUCE_CODE;

			pyra->data[j] = reduced->features(sbin, pads);
			if( !pyra->data[j] ) throw ERROR_FEATURES_CODE;

			delete scaled;
			scaled = reduced;
		}

		delete scaled;
	}

	delete im;
	return pyra;
}

template <typename T>
Vector< HOGImage<T>* >* Pyramid<T>::createPyramid(Model<T> *model, Image<uint8_t> *image){

	if( !model || !image ) return nullptr;
	this->init(model, image);
	if( !this->createPyramidScales(image->dims) ) return nullptr;
	this->pyra = this->createPyramid(image, this->scales, this->interval, this->sBin, model->maxsize);
	return pyra;
}

template <typename T>
Vector< HOGImage<T>* >* Pyramid<T>::createPyramid(Model<T> *model, Image<uint8_t> *image, int maxlevel, int minlevel, int minface){

	if( !model || !image ) return nullptr;
	this->init(model, image);
	if( !this->setCustomPyramidSettings(minlevel, maxlevel, minface) ) return nullptr;

	if( !this->createPyramidScales(image->dims) ) return nullptr;
	this->pyra = this->createPyramid(image, this->scales, this->interval, this->sBin, model->maxsize);
	return pyra;
}

} /* namespace tsm */

#endif

