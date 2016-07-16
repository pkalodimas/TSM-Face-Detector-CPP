
#ifndef FIND_CPP_
#define FIND_CPP_

#include "Part.h"

namespace tsm {

//--------------------------------------------- FIND V1.0 ------------------------------------

template <typename T>
int Part<T>::find(Array<T> *table, T threshold, int fbuffer, Vector<int> *X, Vector<int> *Y){

	if( !table || !table->data || !X || !Y ) return -1;
	if( fbuffer < 1 ) fbuffer = table->length;

	T *ptr;
	int cnt = 0;

	X->reset();
	Y->reset();

	ptr = table->data;
	for( int i=0; i<table->dims[1]; i++ ){

		for( int j=0; j<table->dims[0]; j++, ptr++ ){

			if( *ptr >= threshold ) {

				if( cnt == X->length ){
					cnt += fbuffer;
					X->data = (int*) realloc(X->data, cnt*sizeof(int));
					Y->data = (int*) realloc(Y->data, cnt*sizeof(int));
				}
				X->data[X->length] = j;
				Y->data[X->length] = i;
				X->length++;
			}
		}
	}
	Y->length = X->length;

	return X->length;
}


//--------------------------------------------- FIND V2.0 ------------------------------------

template <typename T>
Array<T> *Part<T>::clearArea(Array<T> *area, int x, int y, T thresh, T rval){

	if( !area ) return area;
	area->data[y*area->dims[0]+x] = rval;

	if( x < 1 || y < 1 || x > (area->dims[0]-2) || y > (area->dims[1]-2) ) return area;

	if( area->data[((y-1)*area->dims[0])+x-1] >= thresh ) clearArea(area, x-1, y-1, thresh, rval);
	if( area->data[((y-1)*area->dims[0])+x] >= thresh ) clearArea(area, x, y-1, thresh, rval);
	if( area->data[((y-1)*area->dims[0])+x+1] >= thresh ) clearArea(area, x+1, y-1, thresh, rval);

	if( area->data[(y*area->dims[0])+x-1] >= thresh ) clearArea(area, x-1, y, thresh, rval);
	if( area->data[(y*area->dims[0])+x+1] >= thresh ) clearArea(area, x+1, y, thresh, rval);

	if( area->data[((y+1)*area->dims[0])+x-1] >= thresh ) clearArea(area, x-1, y+1, thresh, rval);
	if( area->data[((y+1)*area->dims[0])+x] >= thresh ) clearArea(area, x, y+1, thresh, rval);
	if( area->data[((y+1)*area->dims[0])+x+1] >= thresh ) clearArea(area, x+1, y+1, thresh, rval);

	return area;
}

template <typename T>
int Part<T>::nms_find(Array<T> *table, T threshold, int fbuffer, Vector<int> *X, Vector<int> *Y){

	if( !table || !table->data || !X || !Y ) return -1;
	if( fbuffer < 1 ) fbuffer = table->length;

	T *ptr;
	int cnt = 0;
	int len = 0;
	T rval = (T) (threshold > 0)? 0 : 2*threshold;

	Vector<T> *s = nullptr;

	X->reset();
	Y->reset();

	do {
		ptr = table->data;
		len = 0;

		for( int i=0; i<table->dims[1]; i++ ){

			for( int j=0; j<table->dims[0]; j++, ptr++ ){

				if( *ptr >= threshold ) {

					if( cnt == X->length ){
						cnt += fbuffer;
						X->data = (int*) realloc(X->data, cnt*sizeof(int));
						Y->data = (int*) realloc(Y->data, cnt*sizeof(int));
					}
					X->data[X->length] = j;
					Y->data[X->length] = i;
					X->length++;
					len++;
				}
			}
		}
		Y->length = X->length;

		if( len == 0) break;
		if( !s ) {
			s = new Vector<T>(len);
			s->length = 0;
		}

		int start = X->length - len;
		int maxi = start;
		int x = X->data[start];
		int y = Y->data[start];
		T max = table->data[Y->data[start]*table->dims[0] + X->data[start]];

		for( int i=start+1; i<X->length; i++ ){
			if( max < table->data[Y->data[i]*table->dims[0] + X->data[i]] ) {
				max = table->data[Y->data[i]*table->dims[0] + X->data[i]];
				x = X->data[i];
				y = Y->data[i];
				maxi = i;
			}
		}

		X->data[maxi] = X->data[start];
		Y->data[maxi] = Y->data[start];
		X->data[start] = x;
		Y->data[start] = y;
		s->data[s->length] = max;

		X->length = start + 1;
		Y->length = start + 1;
		s->length++;

		clearArea(table, x, y, threshold, rval);

	}while( len > 1 );


	if( X->length > 0 ){

		int *y_temp = Y->data;
		int *x_temp = X->data;
		X->init(X->length);
		Y->init(Y->length);
		memcpy(X->data, x_temp, X->length*sizeof(T));
		memcpy(Y->data, y_temp, Y->length*sizeof(T));

		for( int i=0; i<X->length; i++ ){

			table->data[Y->data[i]*table->dims[0]+X->data[i]] = s->data[i];
		}

		delete[] x_temp;
		delete[] y_temp;
		delete s;
	}

	return X->length;
}

} /* namespace tsm */

#endif
