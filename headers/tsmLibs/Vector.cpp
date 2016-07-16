
#ifndef VECTOR_CPP_
#define VECTOR_CPP_

#include "Vector.h"
#include "Sort.cpp"

namespace tsm {

// Constructors
template <typename T>
Vector<T>::Vector(){

	this->data = nullptr;
	this->length = 0;
}

template <typename T>
Vector<T>::Vector(int size){

	this->length = size;
	this->data = new T[size]();
}

// Destructor
template <typename T>
Vector<T>::~Vector(){

	delete[] this->data;
	this->data = nullptr;
}

//Reset
template <typename T>
void Vector<T>::reset(){

	delete[] this->data;
	this->data = nullptr;
	this->length = 0;
}

// Init
template <typename T>
void Vector<T>::init(int size){

	this->length = size;
	this->data = new T[size]();
}

// Copy Vectors
template <typename T>
bool Vector<T>::copy(Vector<T> *table){

	if( !table ) return false;
	if( this->length != table->length ){
		this->reset();
		this->init(table->length);
	}

	memcpy(this->data, table->data, table->length*sizeof(T));
	return true;
}

// Vector Clone
template <typename T>
Vector<T> *Vector<T>::clone(){

	Vector<T> *clone = new Vector<T>(this->length);
	if( !clone->copy(this) ) {
		delete clone;
		return nullptr;
	}

	return clone;
}

// Vector Min & Max
template <typename T>
T Vector<T>::min(int start, int step){

	if( start >= this->length || start < 0 ) return 0;
	if( step < 1 ) return 0;

	T min = this->data[start];

	for( int i=start+step; i<this->length; i+=step ){ 
		if( min > this->data[i] ) min = this->data[i];
	}

	return min;
}

template <typename T>
T Vector<T>::max(int start, int step){

	if( start >= this->length || start < 0 ) return 0;
	if( step < 1 ) return 0;

	T max = this->data[start];

	for( int i=start+step; i<this->length; i+=step ){ 
		if( max < this->data[i] ) max = this->data[i];
	}

	return max;
}

// Vector Additions
template <typename T>
bool Vector<T>::add(Vector<T> *A){

	if( !A ) return false;
	if( this->length != A->length ) return false;

	for( int i=0; i<this->length; i++ ){ this->data[i] = this->data[i] + A->data[i]; }

	return true;
}

template <typename T>
bool Vector<T>::add(Vector<T> *A, Vector<T> *B){

	if( !A || !B ) return false;
	if( A->length != B->length ) return false;
	if( this->length != A->length ){

		this->reset();
		this->init(A->length);
	}
	if( !this->copy(A) ) return false;
	if( !this->add(B) ) return false;

	return true;
}

// static
template <typename T>
Vector<T>* Vector<T>::add(Vector<T> *A, Vector<T> *B, Vector<T> *C){

	if( !A || !B ) return nullptr;
	if( A->length != B->length ) return nullptr;

	if( !C ) C = new Vector<T>(A->length);
	if( !C->add(A,B) ) return nullptr;

	return C;
}

template <typename T>
bool Vector<T>::add(T value){

	for( int i=0; i<this->length; i++ ){ this->data[i] = this->data[i] + value; }
	return true;
}

template <typename T>
bool Vector<T>::add(Vector<T> *A, T value){

	if( !A ) return false;
	if( this->length != A->length ){
		this->reset();
		this->init(A->length);
	}
	if( !this->copy(A) ) return false;
	if( !this->add(value) ) return false;

	return true;
}

// static
template <typename T>
Vector<T>* Vector<T>::add(Vector<T> *A, T value, Vector<T> *C){

	if( !A ) return nullptr;
	if( !C ) C = new Vector<T>(A->length);
	if( C->length != A->length ){
		C->reset();
		C->init(A->length);
	}
	if( !C->add(A, value) ) return nullptr;

	return C;
}

template <typename T>
bool Vector<T>::swap(int a, int b){

	if( a < 0 || b < 0 ) return false;
	if( a >= this->length || b >= this->length ) return false;

	T temp = this->data[a];
	this->data[a] = this->data[b];
	this->data[b] = temp;
	return true;
}

} /* namespace tsm */

#endif
