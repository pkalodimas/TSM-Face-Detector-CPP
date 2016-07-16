
#ifndef HOGIMAGE_H_
#define HOGIMAGE_H_

#include "Array.cpp"
#include "Filter.h"
#include "FilterResponse.h"

namespace tsm {

template <typename T>
class HOGImage: public Array<T> {
public:
	HOGImage();
	HOGImage(int *dims);
	virtual ~HOGImage();

	FilterResponse<T>* convolve(Filter<T> *filter);
	static FilterResponse<T>* convolve(HOGImage<T> *feat, Filter<T> *filter);
};

} /* namespace tsm */

#endif /* HOGIMAGE_H_ */
