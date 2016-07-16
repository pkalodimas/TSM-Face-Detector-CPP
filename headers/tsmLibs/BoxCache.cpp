
#ifndef BOXCACHE_CPP_
#define BOXCACHE_CPP_

#include "BoxCache.h"
#include "Nms.cpp"

namespace tsm {

// Constructors
template <typename T>
BoxCache<T>::BoxCache() {
	this->boxes = new Vector< Box<T> >;
	this->size = 0;
}

template <typename T>
BoxCache<T>::BoxCache(int size) {
	this->boxes = new Vector< Box<T> >(size);
	this->boxes->length = 0;
	this->size = size;
}

// Destructors
template <typename T>
BoxCache<T>::~BoxCache() {
	delete this->boxes;
}

// Reset
template <typename T>
void BoxCache<T>::reset(){

	delete this->boxes;
	this->size = 0;
}

//Initiliaze
template <typename T>
void BoxCache<T>::init(int size){

	this->reset();
	this->boxes = new Vector< Box<T> >(size);
	this->boxes->length = 0;
	this->size = size;
}

// Clipboxes
template <typename T>
void BoxCache<T>::clipboxes(int imx, int imy){

	if( !this->boxes ) return;

	for( int i=0; i<this->boxes->length; i++ ){

		for( T *ptr=this->boxes->data[i].xy->data; ptr<(this->boxes->data[i].xy->data + this->boxes->data[i].xy->length); ptr+=4 ){

			ptr[0] = (T) std::max(ptr[0], (T)1);
			ptr[1] = (T) std::max(ptr[1], (T)1);
			ptr[2] = (T) std::min(ptr[2], (T)imx);
			ptr[3] = (T) std::min(ptr[3], (T)imy);
		}
	}
}

template <typename T>
Vector< Box<T> > *BoxCache<T>::getResults(){

	if( !this->boxes || this->boxes->length < 1 ) return nullptr;

	Box<T> *b = this->boxes->data;
	this->boxes->data = new Box<T>[this->boxes->length]();
	for (int i = 0; i < this->boxes->length; i++) {
		this->boxes->data[i] = b[i];
		b[i].xy = nullptr;
	}
	delete[] b;

	return this->boxes;
}

} /* namespace tsm */

#endif /* BOXCACHE_CPP_ */

