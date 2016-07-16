
#ifndef REDUCE_CPP_
#define REDUCE_CPP_

#include <cmath>
#include "Image.h"

namespace tsm {

template <typename T>
Image<T>* Image<T>::reduce(Image<T> *RGBimage){

	if( !RGBimage || !RGBimage->data )  return nullptr;

	int dims[3] = { RGBimage->dims[1], (int)  floorf(RGBimage->dims[0]/2 + 0.5), RGBimage->dims[2] };
	Image<T> *im = new Image<T>(dims);

#ifdef OMP_ENABLE
#pragma omp parallel for
#endif
	for( int z=0; z<RGBimage->dims[2]; z++ ){

		for( int y=0; y<RGBimage->dims[1]; y++ ){

			T *s = RGBimage->data + z*RGBimage->dims[0]*RGBimage->dims[1] + y*RGBimage->dims[0];
			T *d = im->data + z*im->dims[0]*im->dims[1] + y;

			d[0] = (T) (s[0]*0.6875 + s[1]*0.2500 + s[2]*0.0625);

			for ( int x=1; x<im->dims[1]-2; x++ ) {
				s += 2;
				d += im->dims[0];
				*d = (T) (s[-2]*0.0625 + s[-1]*0.25 + s[0]*0.375 + s[1]*0.25 + s[2]*0.0625);
			}

			s += 2;
			d += im->dims[0];
			if (im->dims[1]*2 <= RGBimage->dims[0])  *d = (T) (s[-2]*0.0625 + s[-1]*0.25 + s[0]*0.375 + s[1]*0.25 + s[2]*0.0625);
			else  *d = (T) (s[1]*0.3125 + s[0]*0.3750 + s[-1]*0.2500 + s[-2]*0.0625);

			s += 2;
			d += im->dims[0];
			*d = (T) (s[0]*0.6875 + s[-1]*0.2500 + s[-2]*0.0625);
		}
	}

	dims[0] = im->dims[1];
	dims[1] = (int) floorf((im->dims[0]/2)+0.5);
	dims[2] = im->dims[2];
	Image<T> *scimage = new Image<T>(dims);

#ifdef OMP_ENABLE
#pragma omp parallel for
#endif
	for( int z=0; z<im->dims[2]; z++ ){

		for( int y=0; y<im->dims[1]; y++ ){

			T *s = im->data + z*im->dims[0]*im->dims[1] + y*im->dims[0];
			T *d = scimage->data + z*scimage->dims[0]*scimage->dims[1] + y;

			d[0] = (T) (s[0]*0.6875 + s[1]*0.2500 + s[2]*0.0625);

			for ( int x=1; x<scimage->dims[1]-2; x++ ) {
				s += 2;
				d += scimage->dims[0];
				*d = (T) (s[-2]*0.0625 + s[-1]*0.25 + s[0]*0.375 + s[1]*0.25 + s[2]*0.0625);
			}

			s += 2;
			d += scimage->dims[0];
			if (scimage->dims[1]*2 <= im->dims[0])  *d = (T) (s[-2]*0.0625 + s[-1]*0.25 + s[0]*0.375 + s[1]*0.25 + s[2]*0.0625);
			else  *d = (T) (s[1]*0.3125 + s[0]*0.3750 + s[-1]*0.2500 + s[-2]*0.0625);

			s += 2;
			d += scimage->dims[0];
			*d = (T) (s[0]*0.6875 + s[-1]*0.2500 + s[-2]*0.0625);
		}
	}

	delete im;
	return scimage;
}

template <typename T>
Image<T>* Image<T>::reduce(){

	return this->reduce(this);
}

} /* namespace tsm */

#endif
