
#ifndef ARRAY_CPP_
#define ARRAY_CPP_

#include "Array.h"

namespace tsm {

// Constructors
template <typename T>
Array<T>::Array() {

	this->length = 0;
	this->data = nullptr;
	this->dims[0] = 0;
	this->dims[1] = 0;
	this->dims[2] = 0;
}

template <typename T>
Array<T>::Array(int x, int y, int z) {

	this->dims[0] = x;
	this->dims[1] = y;
	this->dims[2] = z;
	this->length = x * y * z;
	this->data = new T[this->length]();
}

template <typename T>
Array<T>::Array(int size[]) {

	if (size) {
		this->dims[0] = size[0];
		this->dims[1] = size[1];
		this->dims[2] = size[2];
		this->length = size[0] * size[1] * size[2];
		this->data = new T[this->length]();
	}
	else Array();
}

// Destructor
template <typename T>
Array<T>::~Array() {
}

// Reset
template <typename T>
void Array<T>::reset(){

	this->length = 0;
	this->dims[0] = 0;
	this->dims[1] = 0;
	this->dims[2] = 0;
	delete[] this->data;
	this->data = nullptr;
}

// Initialize
template <typename T>
void Array<T>::init(int size[]){

	this->dims[0] = size[0];
	this->dims[1] = size[1];
	this->dims[2] = size[2];
	this->length = size[0] * size[1] *size[2];
	this->data = new T[this->length]();
}

// Copy Dims
template <typename T>
bool Array<T>::copyDims(int *dims){

	if( !dims ) return false;

	this->dims[0] = dims[0];
	this->dims[1] = dims[1];
	this->dims[2] = dims[2];

	return true;
}

// Copy Arrays
template <typename T>
bool Array<T>::copy(Array<T> *table){

	if( !table ) return false;
	if (table->length != this->length) {
		this->reset();
		this->init(table->dims);
	}
	memcpy(this->data, table->data, table->length * sizeof(T));
	this->length = table->length;
	this->copyDims(table->dims);
	return true;
}

template <typename T>
Array<T>* Array<T>::clone(){

	Array<T> *clone = new Array<T>(this->dims);
	clone->copy(this);
	return clone;
}

// Array Additions
template <typename T>
bool Array<T>::add(T value){
	
	return Vector<T>::add(value);
}

template <typename T>
bool Array<T>::add(Array<T> *A){

	if( !A ) return false;
	if( this->dims[0] != A->dims[0] || this->dims[1] != A->dims[1] || this->dims[2] != A->dims[2] ) return false;

	return Vector<T>::add(A);
}

template <typename T>
bool Array<T>::add(Array<T> *A, Array<T> *B){

	if( !A || !B ) return false;
	if( A->dims[0] != B->dims[0] || A->dims[1] != B->dims[1] || A->dims[2] != B->dims[2] ) return false;

	this->copy(A);
	this->add(B);

	return true;
}

// static
template <typename T>
Array<T>* Array<T>::add(Array<T> *A, Array<T> *B, Array<T> *C){

	if( !A || !B ) return nullptr;
	if( A->dims[0] != B->dims[0] || A->dims[1] != B->dims[1] || A->dims[2] != B->dims[2] ) return nullptr;

	C = Vector<T>::add(A, B, C);
	if( !C ) return nullptr;
	C->copyDims(A->dims);

	return C;
}


template <typename T>
static Array<T>* Array<T>::add(Array<T> *A, T value, Array<T> *C){

	C = Vector<T>::add(A, value, C);
	if( !C ) return nullptr;
	C->copyDims(A->dims);

	return C;
}

} /* namespace tsm */

#endif
