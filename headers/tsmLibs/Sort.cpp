
#ifndef SORT_CPP_
#define SORT_CPP_

#include "Vector.h"

namespace tsm {

template <typename T>
Vector<int>* Vector<T>::sort(int start, int stop, Vector<int> *indexes){

	if( !this->data ) return nullptr;

	if( !indexes ) {
		indexes = new Vector<int>(this->length);
		for( int i=0; i<this->length; i++ ){ indexes->data[i] = i; }
	}

    int i,j,k;
	T key;

    if( start < stop)
    {
		k = (start+stop)/2;
		this->swap(start,k);
		indexes->swap(start, k);
		key = this->data[start];
        i = start+1;
        j = stop;

		while(i <= j){

            while((i <= stop) && (this->data[i] <= key))  i++;

            while((j >= start) && (this->data[j] > key))  j--;

			if( i < j){
				this->swap(i,j);
				indexes->swap(i,j);
			}
        }

		this->swap(start,j);
		indexes->swap(start,j);

		indexes = this->sort(start, j-1, indexes);
		indexes = this->sort(j+1, stop, indexes);
    }
    return indexes;
}

template <typename T>
Vector<int>* Vector<T>::sort(){

	return this->sort(0, this->length-1, nullptr);
}

template <typename T>
static Vector<T>* Vector<T>::sort(Vector<T> *table, int start, int stop, Vector<int> *indexes){

	if( !table || !table->data ) return nullptr;

	Vector<T> *stable = new Vector<T>(table->length);
	stable->copy(table);
	Vector<int> *indx = stable->sort();
	*indexes = *indx;

	indx->data = nullptr;
	delete indx;

	return stable;
}

} /* namespace tsm */

#endif
