
#ifndef PART_CPP_
#define PART_CPP_

#include "Part.h"
#include "ShiftDT.cpp"
#include "Find.cpp"

namespace tsm {

template <typename T>
Part<T>::Part() {
	this->defid = 0;
	this->filterid = 0;
	this->parent = 0;
	this->sizx = 0;
	this->sizy = 0;
	this->filterI = 0;
	this->defI = 0;
	this->scale = 0;
	this->startx = 0;
	this->starty = 0;
	this->step = 0;
	this->level = 0;
	this->score = nullptr;
	this->Ix = nullptr;
	this->Iy = nullptr;
}

template <typename T>
Part<T>::~Part() {
	delete this->score;
	delete this->Ix;
	delete this->Iy;
}

template <typename T>
int Part<T>::find(T threshold, int fbuffer, Vector<int> *X, Vector<int> *Y){

	return this->find(this->score, threshold, fbuffer, X, Y);
}

template <typename T>
int Part<T>::nms_find(T threshold, int fbuffer, Vector<int> *X, Vector<int> *Y){

	return this->nms_find(this->score, threshold, fbuffer, X, Y);
}

template <typename T>
void Part<T>::reset(){

	this->level = 0;
	delete this->Ix;
	this->Ix = nullptr;
	delete this->Iy;
	this->Iy = nullptr;
	delete this->score;
	this->score = nullptr;
}

} /* namespace tsm */

#endif
