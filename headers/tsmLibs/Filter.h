
#ifndef FILTER_H_
#define FILTER_H_

#include "Array.cpp"

namespace tsm {

template <typename T>
class Filter: public Array<T> {
public:
	int i;
public:
	Filter();
	virtual ~Filter();
};

} /* namespace tsm */

#endif /* TSMFILTER_H_ */
