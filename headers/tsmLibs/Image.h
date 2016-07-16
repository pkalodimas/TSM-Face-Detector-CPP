
#ifndef IMAGE_H_
#define IMAGE_H_

#include "Array.cpp"
#include "HOGImage.h"

namespace tsm {

template <typename T>
class Image : public Array<T> {
public:
	Image();
	Image(int *dims);
	virtual ~Image();

	Image<T>* clone();
	Image<T>* reduce();
	Image<T>* resize(float scale);
	HOGImage<T>* features(int cells, int *pads);

	static Image<T>* reduce(Image<T>* RGBimage);
	static Image<T>* resize(Image<T>* RGBimage, float scale);
	static HOGImage<T>* features(Image<T> *image, int cells, int *pads);
	static HOGImage<T>* portrait(HOGImage<T> *table, int pads[], T padval, int start, int stop);
};

} /* namespace tsm */

#endif /* IMAGE_H_ */
