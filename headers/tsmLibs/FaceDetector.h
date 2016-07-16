
#ifndef FACEDETECTOR_H_
#define FACEDETECTOR_H_

#include "Model.cpp"
#include "Settings.cpp"
#include "HOGImage.cpp"
#include "Pyramid.cpp"
#include "FilterResponse.cpp"
#include "BoxCache.cpp"
#include "ErrorCodes.h"

namespace tsm {

template <typename T>
class FaceDetector {
public:
	Model<T> *model;
	Settings<T> *settings;
	Image<uint8_t> *image;
	BoxCache<T> *boxCache;
	int errorCode;
public:
	FaceDetector();
	FaceDetector(Model<T> *model, Settings<T> *settings, Image<uint8_t> *image);
	virtual ~FaceDetector();

	BoxCache<T> *detect();
	static Vector< FilterResponse<T>* >* convolution(Vector< Filter<T>* >* filters, HOGImage<T> *image);
	static BoxCache<T>* detectV2(Model<T> *model, Image<uint8_t> *image, Settings<T> *settings);
	static BoxCache<T>* detectV3(Model<T> *model, Image<uint8_t> *image, Settings<T> *settings);
	static BoxCache<T>* backstage(BoxCache<T> *boxcache, Component<T> *component, Settings<T> *settings, Pyramid<T> *pyramid, int level);
	static Array<T>* dtstage(Component<T>* component, Vector< FilterResponse<T>* > *resps, int level);
};

} /* namespace tsm */

#endif /* FACEDETECTOR_H_ */
