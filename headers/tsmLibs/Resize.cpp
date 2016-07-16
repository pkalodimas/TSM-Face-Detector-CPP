
#ifndef RESIZE_CPP_
#define RESIZE_CPP_

#include <cmath>
#include <assert.h>
#include "Image.h"

namespace tsm {

template <typename T>
struct alphainfo {
	int si;
	int di;
	T alpha;
};

template <typename T, typename F>
void alphacopy(T *src, T *dst, alphainfo<F> *ofs, int n, int l) {

	if( !src || !dst || !ofs ) return;

	alphainfo<F> *end = ofs + n;

	for( ofs; ofs!=end ; ofs++ ){

		dst[ofs->di] += (T) (ofs->alpha * src[ofs->si]);
	}
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------RESIZE ----------------------------------------
//---------------------------------------------------------------------------------------------

template <typename T>
Image<T>* Image<T>::resize(Image<T> *RGBimage, float scale){

	if( !RGBimage || !RGBimage->data ) return nullptr;
	if( scale >= 1 ) return RGBimage->clone();

	int dims[3] = {RGBimage->dims[1], (int) floorf(RGBimage->dims[0]*scale + 0.5), RGBimage->dims[2] };
	Image<T> *im = new Image<T>(dims);
	float sc = (float)im->dims[1] / (float)RGBimage->dims[0];
	float invscale = (float)1 / (float)sc;
	int len = (int) ceil(im->dims[1] * invscale) + 2*im->dims[1];
	int k=0;
	alphainfo<T> *ofs = new alphainfo<T>[len]();

	for( int dy=0; dy<im->dims[1]; dy++ ){

		float fsy1 = dy * invscale;
		float fsy2 = fsy1 + invscale;
		int sy1 = (int) ceil(fsy1);
		int sy2 = (int) floor(fsy2);

		if (sy1 - fsy1 > 0.001) {

			assert(k < len);
			assert(sy1 - 1 >= 0);
			ofs[k].di = dy * im->dims[0];
			ofs[k].si = sy1 - 1 ;
			ofs[k++].alpha = (sy1 - fsy1) * sc;
		}

		for (int sy=sy1; sy<sy2; sy++) {

			assert(k < len);
			assert(sy < RGBimage->dims[0]);
			ofs[k].di = dy * im->dims[0];
			ofs[k].si = sy;
			ofs[k++].alpha = sc;
		}

		if (fsy2 - sy2 > 0.001) {

			assert(k < len);
			assert(sy2 < RGBimage->dims[0]);
			ofs[k].di = dy * im->dims[0];
			ofs[k].si = sy2;
			ofs[k++].alpha = (fsy2 - sy2) * sc;
		}
	}

#ifdef OMP_ENABLE
#pragma omp parallel for
#endif
	for ( int c=0; c<RGBimage->dims[2]; c++ ) {
		for ( int x=0; x<RGBimage->dims[1]; x++ ) {

			T *s = RGBimage->data + c*RGBimage->dims[0]*RGBimage->dims[1] + x*RGBimage->dims[0];
			T *d = im->data + c*im->dims[0]*im->dims[1] + x;
			alphacopy(s, d, ofs, k, im->length);
		}
	}

	delete[] ofs;

	dims[0] = im->dims[1];
	dims[1] = (int) floorf(im->dims[0]*scale + 0.5);
	dims[2] = im->dims[2];
	Image<T> *scimage = new Image<T>(dims);
	sc = (float)scimage->dims[1] / (float)im->dims[0];
	invscale = (float)1 / (float)sc;
	len = (int) ceil(scimage->dims[1]*invscale) + 2*scimage->dims[1];
	ofs = new alphainfo<T>[len]();
	k = 0;

	for( int dy=0; dy<scimage->dims[1]; dy++ ){

		float fsy1 = dy * invscale;
		float fsy2 = fsy1 + invscale;
		int sy1 = (int) ceil(fsy1);
		int sy2 = (int) floor(fsy2);

		if (sy1 - fsy1 > 0.001) {

			assert(k < len);
			assert(sy1 - 1 >= 0);
			ofs[k].di = dy * scimage->dims[0];
			ofs[k].si = sy1 - 1 ;
			ofs[k++].alpha = (sy1 - fsy1) * sc;
		}

		for (int sy=sy1; sy<sy2; sy++) {

			assert(k < len);
			assert(sy < im->dims[0]);
			ofs[k].di = dy * scimage->dims[0];
			ofs[k].si = sy;
			ofs[k++].alpha = sc;
		}

		if (fsy2 - sy2 > 0.001) {

			assert(k < len);
			assert(sy2 < im->dims[0]);
			ofs[k].di = dy * scimage->dims[0];
			ofs[k].si = sy2;
			ofs[k++].alpha = (fsy2 - sy2) * sc;
		}
	}

#ifdef OMP_ENABLE
#pragma omp parallel for
#endif
	for ( int c=0; c<im->dims[2]; c++ ) {
		for ( int x=0; x<im->dims[1]; x++ ) {

			T *s = im->data + c*im->dims[0]*im->dims[1] + x*im->dims[0];
			T *d = scimage->data + c*scimage->dims[0]*scimage->dims[1] + x;
			alphacopy(s, d, ofs, k, scimage->length);
		}
	}

	delete[] ofs;
	delete im;

	return scimage;
}

template <typename T>
Image<T>* Image<T>::resize(float scale){

	return this->resize(this, scale);
}

} /* namespace tsm */

#endif
