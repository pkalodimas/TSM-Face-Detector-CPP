
#ifndef DEF_CPP_
#define DEF_CPP_

#include "Def.h"

namespace tsm {

template <typename T>
Def<T>::Def() {
	this->w = nullptr;
	this->anchor = nullptr;
	this->i = 0;
}

template <typename T>
Def<T>::~Def() {
	delete this->w;
	delete this->anchor;
}

} /* namespace tsm */

#endif
