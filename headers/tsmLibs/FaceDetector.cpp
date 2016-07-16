
#ifndef FACEDETECTOR_CPP_
#define FACEDETECTOR_CPP_

#include "FaceDetector.h"

namespace tsm {

template <typename T>
FaceDetector<T>::FaceDetector() {
	this->model = nullptr;
	this->settings = nullptr;
	this->image = nullptr;
	this->boxCache = nullptr;
	this->errorCode = 0;
}

template <typename T>
FaceDetector<T>::FaceDetector(Model<T> *model, Settings<T> *settings, Image<uint8_t> *image) {
	this->model = model;
	this->settings = settings;
	this->image = image;
	this->boxCache = nullptr;
	this->errorCode = 0;
}

template <typename T>
FaceDetector<T>::~FaceDetector() {
	delete this->model;
	delete this->settings;
	delete this->image;
	delete this->boxCache;
}

template <typename T>
Vector< FilterResponse<T>* >* FaceDetector<T>::convolution(Vector< Filter<T>* >* filters, HOGImage<T> *image){

	if( !filters || !image ) return nullptr;
	Vector< FilterResponse<T>* > *resps = new Vector< FilterResponse<T>* >(filters->length);

#ifdef OMP_ENABLE
	#pragma omp parallel for
#endif
	for( int i=0; i<filters->length; i++){ resps->data[i] = image->convolve(filters->data[i]); }

	return resps;
}

template <typename T>
Array<T>* FaceDetector<T>::dtstage(Component<T>* component, Vector< FilterResponse<T>* > *resps, int level){

	return component->shiftdt(resps, level);
}

template <typename T>
BoxCache<T>* FaceDetector<T>::backstage(BoxCache<T> *boxcache, Component<T> *component, Settings<T> *settings, Pyramid<T> *pyramid, int level){

	if( !pyramid || !settings || !component )  throw ERROR_BACKTRACK_STAGE_CODE;

	if( !boxcache ) boxcache = new BoxCache<T>(settings->BoxCacheSize);
	if( boxcache->size < 1 ) boxcache->init(settings->BoxCacheSize);

	// Find
	Vector<int> X, Y;
//	int e = component->parts->data[0].find(settings->Threshold, settings->FindBuffer, &X, &Y);	// version 1
	int e = component->parts->data[0].nms_find(settings->Threshold, settings->FindBuffer, &X, &Y);	// version 2
	if( e < 0 ) throw ERROR_FIND_CODE;

	// Backtrack
	if( X.length > 0 ) {

		Vector< Array<T>* > *XY = component->backtrack(&X, &Y, pyramid->scales, pyramid->padx, pyramid->pady);
		if( !XY ) throw ERROR_BACKTRACK_CODE;

#ifdef OMP_ENABLE
	#pragma omp critical
	{
#endif
		for( int i=0; i<X.length; i++ ){

			// NMS
			if( boxcache->boxes->length == boxcache->size ){
				if( !boxcache->nms(settings->NmsOverlap, settings->NmsLimit) ) throw ERROR_NMS_CODE;
			}

			int cnt = boxcache->boxes->length;
			boxcache->boxes->data[cnt].c = component->id;
			boxcache->boxes->data[cnt].s = component->parts->data[0].score->data[Y.data[i]*component->parts->data[0].score->dims[0] + X.data[i]];
			boxcache->boxes->data[cnt].level = level + 1;
			boxcache->boxes->data[cnt].xy = XY->data[i];

			boxcache->boxes->length++;
		}
#ifdef OMP_ENABLE
	} // OMP Critical
#endif

		delete XY;
	}

	return boxcache;
}

// Detect using version 2 of TSM system
template <typename T>
BoxCache<T>* FaceDetector<T>::detectV2(Model<T> *model, Image<uint8_t> *image, Settings<T> *settings) {

	if (!model || !image || !settings) throw ERROR_FACE_DETECTION_CODE;

	model->thresh = settings->Threshold;
	model->interval = settings->Interval;

	// Create Features Pyramid
	Pyramid<T> *pyramid = new Pyramid<T>();
	pyramid->init(model->interval, model->sbin, image->dims, model->maxsize);
	if (!pyramid->setCustomPyramidSettings(settings->MinLevel, settings->MaxLevel, settings->MinFace)) throw ERROR_FPYRAMID_CODE;
	if (!pyramid->createPyramid(model, image)) throw ERROR_FPYRAMID_CODE;

	// Model Components
	model->modelComponents(pyramid->padx, pyramid->pady);

	// Detect stage
	BoxCache<T> *boxcache = new BoxCache<T>(settings->BoxCacheSize);

	for (int level = pyramid->pyra->length-1; level>=0; level--) {

		Vector< FilterResponse<T>* > *resp = FaceDetector<T>::convolution(model->filters, (HOGImage<T>*) pyramid->pyra->data[level]);
		pyramid->releaseLevel(level);
		if (!resp) throw ERROR_CONVOLUTION_CODE;

#ifdef OMP_ENABLE
#pragma omp parallel for
#endif
		for (int c = 0; c<model->components->length; c++) {

			dtstage(model->components->data[c], resp, level);
			backstage(boxcache, model->components->data[c], settings, pyramid, level);
			model->components->data[c]->reset();
		}

		for (int i = 0; i < resp->length; i++) { delete resp->data[i]; }
		delete resp;
	}

	boxcache->clipboxes(image->dims[0], image->dims[1]);
	if (!boxcache->nms(settings->NmsOverlap, settings->NmsLimit)) throw ERROR_NMS_CODE;

	return boxcache;
}

// Detect using the version 3 of the TSM system
template <typename T>
BoxCache<T>* FaceDetector<T>::detectV3(Model<T> *model, Image<uint8_t> *image, Settings<T> *settings) {

	if (!model || !image || !settings) throw ERROR_FACE_DETECTION_CODE;

	model->thresh = settings->Threshold;
	model->interval = settings->Interval;

	// Prepare Features Pyramid
	Pyramid<T> *pyramid = new Pyramid<T>();
	pyramid->init(model->interval, model->sbin, image->dims, model->maxsize);
	if (!pyramid->setCustomPyramidSettings(settings->MinLevel, settings->MaxLevel, settings->MinFace)) throw ERROR_FPYRAMID_CODE;
	if (!pyramid->createPyramidScales(image->dims)) throw ERROR_FPYRAMID_CODE;

	int pads[3] = { pyramid->padx + 1, pyramid->pady + 1, 0 };
	pyramid->pyra = (Vector< HOGImage<T>* >*) new Vector< Image<T>* >(pyramid->scales->length);
//	Vector< Image<T>* > *pyra = new Vector< Image<T>* >(pyramid->scales->length);
	Image<T> *im = new Image<T>(image->dims);
	for (int i = 0; i<image->length; i++) { im->data[i] = (T) image->data[i]; }	// uint8 to double

	// Model Components
	model->modelComponents(pyramid->padx, pyramid->pady);

	// Detect stage
	BoxCache<T> *boxcache = new BoxCache<T>(settings->BoxCacheSize);


	for (int level = 0; level < pyramid->scales->length; level++) {

//		if ( !pyra->data[level] ) pyra->data[level] = im->resize(model->sbin / pyramid->scales->data[level]);
//		if ( level + model->interval < pyramid->scales->length ) pyra->data[level + model->interval] = pyra->data[level]->reduce();
		if (!pyramid->pyra->data[level]) pyramid->pyra->data[level] = (HOGImage<T>*) im->resize(model->sbin / pyramid->scales->data[level]);
		if (level + model->interval < pyramid->scales->length) pyramid->pyra->data[level + model->interval] = (HOGImage<T>*) ((Image<T>*) pyramid->pyra->data[level])->reduce();

		HOGImage<T> *feats = ((Image<T>*) pyramid->pyra->data[level])->features(model->sbin, pads);
		pyramid->releaseLevel(level);

//		HOGImage<T> *feats = pyra->data[level]->features(model->sbin, pads);
//		delete pyra->data[level];
//		pyra->data[level] = nullptr;

		Vector< FilterResponse<T>* > *resp = FaceDetector<T>::convolution(model->filters, feats);
		delete feats;
		if (!resp) throw ERROR_CONVOLUTION_CODE;

#ifdef OMP_ENABLE
#pragma omp parallel for
#endif
		for (int c = 0; c<model->components->length; c++) {

			dtstage(model->components->data[c], resp, level);
			backstage(boxcache, model->components->data[c], settings, pyramid, level);
			model->components->data[c]->reset();
		}

		for (int i = 0; i < resp->length; i++) { delete resp->data[i]; }
		delete resp;
	}

	boxcache->clipboxes(image->dims[0], image->dims[1]);
	if (!boxcache->nms(settings->NmsOverlap, settings->NmsLimit)) throw ERROR_NMS_CODE;

	return boxcache;
}

template <typename T>
BoxCache<T>* FaceDetector<T>::detect() {

	try {
		if( this->settings->Version == 2 ) this->boxCache = detectV2(this->model, this->image, this->settings);
		else this->boxCache = detectV3(this->model, this->image, this->settings);
		if (!this->boxCache) throw ERROR_FACE_DETECTION_CODE;
	}
	catch (int c) {
		this->errorCode = c;
		return nullptr;
	}
	return this->boxCache;
}



} /* namespace tsm */

#endif
