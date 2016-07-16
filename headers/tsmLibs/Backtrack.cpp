
#ifndef BACKTRACK_CPP_
#define BACKTRACK_CPP_

#include "Component.h"

namespace tsm {

template <typename T>
Vector<T> *sub2ind(Vector<T> *indexes, T dimX, Vector<T> *x, Vector<T> *y){

	if( !x || !y || !x->data || !y->data ) return nullptr;

	if( !indexes ) indexes = new Vector<T>(x->length);
	if( indexes->length < x->length ) indexes->init(x->length);

	for( int i=0; i<indexes->length; i++ ){

		indexes->data[i] = ((y->data[i]-1)*dimX + (x->data[i]-1));
	}

	return indexes;
};

template <typename T>
Vector<T> *sub2ind(T dimX, Vector<T> *x, Vector<T> *y){

	return sub2ind(nullptr, dimX, x, y);
}

//------------------------------------- BACKTRACK ------------------------------------
template <typename T>
Vector< Array<T>* >* Component<T>::backtrack(Vector<int> *X, Vector<int> *Y, Vector<float> *scales, int padx, int pady){

	if( !X || !Y || !scales || !this->parts ) return nullptr;

	int numparts = this->parts->length;
	Array<T> *ptr = new Array<T>(X->length, 2, numparts);
	Vector< Array<T>* > *box = new Vector< Array<T>* >(X->length);
	Vector<int> *x = new Vector<int>(X->length);
	Vector<int> *y = new Vector<int>(Y->length);
	Vector<int> *inds = new Vector<int>(X->length);
	Part<T> *p;
	int k, l;
	float scale;
	T *x_ptr;

	k = 0;
	l = k*4;
	p = &(this->parts->data[k]);
	scale = (float) scales->data[p->level-1];

	for( int i=0; i<X->length; i++ ){
		x->data[i] = (T) X->data[i] + 1;
		y->data[i] = (T) Y->data[i] + 1;
	}

	for( int i=0; i<box->length; i++ ){
		box->data[i] = new Array<T>(4, numparts, 1);
	}

	x_ptr = ptr->data + k*ptr->dims[0]*ptr->dims[1];
	for( int i=0, j=0; i<x->length; i++, j+=2 ){
		x_ptr[j] = x->data[i];
		x_ptr[j+1] = y->data[i];
	}

	for( int i=0; i<x->length; i++ ){
		box->data[i]->data[l] = (x->data[i]-1-padx)*scale + 1;
		box->data[i]->data[l+1] = (y->data[i]-1-pady)*scale + 1;
		box->data[i]->data[l+2] = box->data[i]->data[l] + (p->sizx)*scale - 1;
		box->data[i]->data[l+3] = box->data[i]->data[l+1] + (p->sizy)*scale - 1;
	}

	for( k=1, l=k*4; k<numparts; k++, l+=4 ){		//numparts

		p = &(this->parts->data[k]);
		int par = p->parent - 1;

		x_ptr = ptr->data + par*ptr->dims[0]*ptr->dims[1];
		for( int i=0, j=0; i<x->length; i++, j+=2 ){
			x->data[i] = x_ptr[j];
			y->data[i] = x_ptr[j+1];
		}

		inds = sub2ind(inds, p->Ix->dims[0], x, y);

		x_ptr = ptr->data + k*ptr->dims[0]*ptr->dims[1];
		for( int i=0, j=0; i<inds->length; i++, j+=2 ){
			x_ptr[j] = p->Ix->data[(int)inds->data[i]];
			x_ptr[j+1] = p->Iy->data[(int)inds->data[i]];
		};

		scale = (float) scales->data[p->level-1];

		x_ptr = ptr->data + k*ptr->dims[0]*ptr->dims[1];
		for( int i=0, j=0; i<ptr->dims[0]; i++, j+=2 ){
			box->data[i]->data[l] = (x_ptr[j] - 1 - padx) * scale + 1;
			box->data[i]->data[l+1] = (x_ptr[j+1] - 1 - pady) * scale + 1;
			box->data[i]->data[l+2] = box->data[i]->data[l] + p->sizx * scale - 1;
			box->data[i]->data[l+3] = box->data[i]->data[l+1] + p->sizy * scale - 1;
		}
	}

	delete inds;
	delete x;
	delete y;
	delete ptr;

	return box;
};

} /* namespace tsm */

#endif
