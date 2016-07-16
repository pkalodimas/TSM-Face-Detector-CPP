
#ifndef BOX_CPP_
#define BOX_CPP_

#include "Box.h"

namespace tsm {

// Constructor
template <typename T>
Box<T>::Box() {
	this->s = 0;
	this->level = 0;
	this->c = 0;
	this->xy = nullptr;
}

// Destructor
template <typename T>
Box<T>::~Box() {
	delete this->xy;
}

} /* namespace tsm */

#endif

