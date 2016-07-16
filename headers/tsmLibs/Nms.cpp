
#ifndef NMS_CPP_
#define NMS_CPP_

#include "BoxCache.h"

namespace tsm {

template <typename T>
Vector< Box<T> >* BoxCache<T>::nms(int overlap, int nms_limit){

	if( !this->boxes || this->boxes->length == 0 ) return nullptr;
	if (nms_limit < 1) nms_limit = this->size;

	int boxcache = this->size;
	int N = this->boxes->length;
	Vector<T> *s;
	Vector<int> *I, *pick;
	T *x1, *x2, *y1, *y2, *area;
	T xx1, xx2, yy1, yy2, w, h, inter, o1, o2;
	int i;
	Vector< Box<T> > tboxes = *this->boxes;

	if( N > nms_limit ){

		s = new Vector<T>(this->boxes->length);
		for( int j=0; j<s->length; j++ ){  s->data[j] = this->boxes->data[j].s; }

		I = s->sort();
		tboxes.init(nms_limit);

		for( int j=this->boxes->length-nms_limit, i=0; j<this->boxes->length; j++, i++ ){

			tboxes.data[i] = this->boxes->data[(int)I->data[j]];
			this->boxes->data[(int)I->data[j]].xy = nullptr;
			I->data[i] = i;
		}

		boxes->reset();
		I->length = nms_limit;
		N = nms_limit;
	}
	else {

		s = new Vector<T>(boxes->length);
		for( int j=0; j<s->length; j++ ){  s->data[j] = boxes->data[j].s;  }
		I = s->sort();
	}
	delete s;

	x1 = new T[N*sizeof(T)]();
	y1 = new T[N*sizeof(T)]();
	x2 = new T[N*sizeof(T)]();
	y2 = new T[N*sizeof(T)]();
	area = new T[N*sizeof(T)]();

	for( int nb=0; nb<N; nb++ ){

		x1[nb] = tboxes.data[nb].xy->min(0, tboxes.data[nb].xy->dims[0]);
		y1[nb] = tboxes.data[nb].xy->min(1, tboxes.data[nb].xy->dims[0]);
		x2[nb] = tboxes.data[nb].xy->max(2, tboxes.data[nb].xy->dims[0]);
		y2[nb] = tboxes.data[nb].xy->max(3, tboxes.data[nb].xy->dims[0]);

		area[nb] = (x2[nb]-x1[nb]+1) * (y2[nb]-y1[nb]+1);
	}

	pick = new Vector<int>(N);
	pick->length = 0;

	while( I->length > 0 ){

		i = I->data[I->length-1];
		pick->data[pick->length] = i;
		pick->length++;
		I->data[I->length-1] = -1;

		for( int j=0; j<I->length-1; j++ ){

			xx1 = max(x1[I->data[j]],x1[i]);
			yy1 = max(y1[I->data[j]],y1[i]);
			xx2 = min(x2[I->data[j]],x2[i]);
			yy2 = min(y2[I->data[j]],y2[i]);

			w = xx2 - xx1 + 1;
			if( w < 0 ){ w = 0; }

			h = yy2 - yy1 + 1;
			if( h < 0 ){ h = 0; }

			inter = w*h;
			o1 = inter / area[j];
			o2 = inter / area[i];

			if( o1 > overlap || o2 > overlap ){  I->data[j] = -1;  }
		}

		for( int j=0; j<I->length; j++ ){

			if( I->data[j] == -1 ){

				for( i=j+1; i<I->length && I->data[i]==-1; i++ ){}
				if( i < I->length ) I->swap(j, i);
				else {
					I->length = j;
					break;
				}
			}
		}
	}

	boxes->init(boxcache);
	boxes->length = pick->length;

	for( int j=0; j<pick->length; j++ ){
		this->boxes->data[j] = tboxes.data[pick->data[j]];
		tboxes.data[pick->data[j]].xy = nullptr;
	}

	delete[] x1;
	delete[] x2;
	delete[] y1;
	delete[] y2;
	delete[] area;
	delete I;
	delete pick;

	return this->boxes;
}

} /* namespace tsm */

#endif /* NMS_CPP_ */
