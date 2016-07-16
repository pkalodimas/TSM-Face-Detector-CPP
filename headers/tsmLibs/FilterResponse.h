
#ifndef FILTERRESPONSE_H_
#define FILTERRESPONSE_H_

#include "Array.cpp"

namespace tsm {

template <typename T>
class FilterResponse: public Array<T> {
public:
	FilterResponse();
	FilterResponse(int *dims);
	virtual ~FilterResponse();
};

} /* namespace tsm */

#endif /* FILTERRESPONSE_H_ */
