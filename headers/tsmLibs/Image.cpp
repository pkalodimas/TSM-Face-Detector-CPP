
#ifndef IMAGE_CPP_
#define IMAGE_CPP_

#include "Image.h"
#include "Reduce.cpp"
#include "Resize.cpp"
#include "Features.cpp"

namespace tsm {

// Constructor
template <typename T>
Image<T>::Image() {
}

template <typename T>
Image<T>::Image(int *dims) : Array<T>(dims) {
}

// Destructor
template <typename T>
Image<T>::~Image() {
}

// Clone
template <typename T>
Image<T> *Image<T>::clone(){

	return (Image<T>*) Array<T>::clone();
}

} /* namespace tsm */

#endif

