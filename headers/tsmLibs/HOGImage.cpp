
#ifndef HOGIMAGE_CPP_
#define HOGIMAGE_CPP_

#include "HOGImage.h"
#include "Convolution.cpp"

namespace tsm {

// Constructor
template <typename T>
HOGImage<T>::HOGImage() {
}

template <typename T>
HOGImage<T>::HOGImage(int *dims) : Array<T>(dims) {
}

// Destructor
template <typename T>
HOGImage<T>::~HOGImage() {
}

} /* namespace tsm */

#endif

