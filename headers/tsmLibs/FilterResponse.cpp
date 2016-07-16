
#ifndef FILTERRESPONSE_CPP_
#define FILTERRESPONSE_CPP_

#include "FilterResponse.h"

namespace tsm {

template <typename T>
FilterResponse<T>::FilterResponse() {
}

template <typename T>
FilterResponse<T>::FilterResponse(int *dims) : Array<T>(dims) {
}

template <typename T>
FilterResponse<T>::~FilterResponse() {
}

} /* namespace tsm */

#endif
