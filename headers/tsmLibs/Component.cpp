
#ifndef COMPONENT_CPP_
#define COMPONENT_CPP_

#include "Component.h"
#include "Backtrack.cpp"
#include "ErrorCodes.h"

namespace tsm {

// Constructor
template <typename T>
Component<T>::Component() {
	this->parts = nullptr;
	this->id = 0;
	this->pose = 0;
}

// Destructor
template <typename T>
Component<T>::~Component() {
	delete this->parts;
}

// Clear
template <typename T>
void Component<T>::reset(){

	for( int i=0; i<this->parts->length; i++ ){
		this->parts->data[i].reset();
	}
}

// Detect
template <typename T>
Array<T>* Component<T>::shiftdt(Vector< FilterResponse<T>* > *responses, int level){

	if( !responses || level < 0) throw ERROR_DT_STAGE_CODE;

	for( int k=this->parts->length-1; k>0; k-- ){

		Part<T> *child = &this->parts->data[k];
		Part<T> *parent = &this->parts->data[child->parent-1];
		child->level = level+1;

		if( !child->score )  child->score = responses->data[child->filterid-1]->clone();

		child->shiftdt();
		if( !child->score ) throw ERROR_DT_CODE;

		if( !parent->score ) {

			child->score->add(responses->data[parent->filterid-1]);
			parent->score = child->score;
			child->score = nullptr;
		}
		else {
			parent->score->add(child->score);
			delete child->score;
			child->score = nullptr;
		}
	}

	this->parts->data[0].level = level + 1;
	this->parts->data[0].score->add(this->parts->data[0].w.data[0]);

	return this->parts->data[0].score;
}

} /* namespace tsm */

#endif

