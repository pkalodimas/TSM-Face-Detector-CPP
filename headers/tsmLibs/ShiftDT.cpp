
#ifndef SHIFTDT_CPP_
#define SHIFTDT_CPP_

#include <limits>
#include "Part.h"

namespace tsm {

template <typename T>
Array<T>* Part<T>::shiftdt(Array<T> *transform, Array<T> *score, T *w, int startx, int starty, int step, Array<int> *Ix, Array<int> *Iy) {

	if( !score || !score->data || !Ix || !Iy || !w ) return nullptr;

	if( !transform ) transform = new Array<T>(score->dims);
	if( transform->dims[0] != score->dims[0] || transform->dims[1] != score->dims[1] || transform->dims[2] != score->dims[2] ) transform->init(score->dims);
	Ix->init(score->dims);
	Iy->init(score->dims);

	T *temp = new T[transform->length]();
	int *tempIy = new int[Iy->length]();
	int lenx = score->dims[0];
	int leny = score->dims[1];
	int *v = (leny > lenx )? new int[leny]() : new int[lenx]();		//Same mem for X,Y
	T *z = (leny > lenx )? new T[leny+1]() : new T[lenx+1]();	//Same mem for X,Y

	T a, b;
	T *_score, *_transform;
	int *_ix, *_iy;

	//1D distance transform for X
	a = -w[2];
	b = -w[3];
	_score = score->data;
	_transform = temp;
	_iy = tempIy;

	for( int x=1; x<=lenx; x++ ) {

		v[0] = 0;
		z[0] = (T) std::numeric_limits<T>::lowest();
		z[1] = (T) std::numeric_limits<T>::max();

		for( int q=1, k=0; q<leny; q++, k++ ){

			T s = (T)(  ((_score[q*lenx] - _score[v[k]*lenx]) - b*(q - v[k]) + a*(q*q - v[k]*v[k])) / (2*a*(q-v[k]))  );
			while (s <= z[k]) {
				k--;
				s = (T)(  ((_score[q*lenx] - _score[v[k]*lenx]) - b*(q - v[k]) + a*(q*q - v[k]*v[k])) / (2*a*(q-v[k]))  );
			}
			v[k+1] = q;
			z[k+1] = s;
			z[k+2] = (T) std::numeric_limits<T>::max();
		}

		for (int i=0, k=0, q=starty-1; i<leny; i++, q+=step) {

			while (z[k+1] < q) { k++; }
			*_transform = (T) (a*(q-v[k])*(q-v[k]) + b*(q-v[k]) + _score[v[k]*lenx]);
			*_iy = v[k];

			_transform += lenx;
			_iy += lenx;
		}

		_score++;
		_transform = temp + x;
		_iy = tempIy + x;
	}

	//1D distance transform for Y
	a = -w[0];
	b = -w[1];
	_score = temp;
	_transform = transform->data;
	_ix = Ix->data;

	for( int y=0; y<leny; y++ ) {

		v[0] = 0;
		z[0] = (T) std::numeric_limits<T>::lowest();
		z[1] = (T) std::numeric_limits<T>::max();

		for( int q=1, k=0; q<lenx; q++, k++ ){

			T s = (T)(  ((_score[q] - _score[v[k]]) - b*(q - v[k]) + a*(q*q - v[k]*v[k])) / (2*a*(q-v[k]))  );
			while (s <= z[k]) {
				k--;
				s = (T)(  ((_score[q] - _score[v[k]]) - b*(q - v[k]) + a*(q*q - v[k]*v[k])) / (2*a*(q-v[k]))  );
			}
			v[k+1] = q;
			z[k+1] = s;
			z[k+2] = (T) std::numeric_limits<T>::max();;
		}

		for (int i=0, k=0, q=startx-1; i<lenx; i++, q+=step) {

			while (z[k+1] < q) { k++; }
			*_transform = (T) (a*(q-v[k])*(q-v[k]) + b*(q-v[k]) + _score[v[k]]);
			*_ix = v[k];

			_transform++;
			_ix++;
		}

		_score += lenx;
	}

	delete[] v;
	delete[] z;

	//Fix Ix, Iy values
	_ix = Ix->data;
	_iy = Iy->data;
	v = tempIy;
	for (int y = 0; y<leny; y++, v+=lenx) {

		for (int x = 0; x<lenx; x++, _iy++, _ix++) {

			*_iy = v[*_ix] + 1;
			(*_ix)++;
		}
	}

	delete[] tempIy;
	delete[] temp;

	return transform;
}

template <typename T>
Array<T>* Part<T>::shiftdt(Array<T> *score, T *w, int startx, int starty, int step, Array<int> *Ix, Array<int> *Iy) {

	return Part<T>::shiftdt(nullptr, score, w, startx, starty, step, Ix, Iy);
}


template <typename T>
Array<T> *Part<T>::shiftdt(){

	if( !this->Ix ) this->Ix = new Array<int>();
	if( !this->Iy ) this->Iy = new Array<int>();
	shiftdt(this->score, this->score, this->w.data, this->startx, this->starty, this->step, this->Ix, this->Iy);
	return this->score;
}

} /* namespace tsm */

#endif

