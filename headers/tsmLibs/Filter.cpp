
#ifndef FILTER_CPP_
#define FILTER_CPP_

#include "Filter.h"

namespace tsm {

template <typename T>
Filter<T>::Filter() {
	this->i = 0;
}

template <typename T>
Filter<T>::~Filter() {
}

} /* namespace tsm */

#endif
