
#ifndef CONVOLUTION_CPP_
#define CONVOLUTION_CPP_

#include "HOGImage.h"
#include "ErrorCodes.h"

namespace tsm {

template <typename T>
FilterResponse<T> * HOGImage<T>::convolve(HOGImage<T> *feat, Filter<T> *filter) {

	if( !feat || !filter || !feat->data || !filter->data ) throw ERROR_FCONV_CODE;

	int rdims[3] = { (feat->dims[0] - filter->dims[0] + 1), (feat->dims[1] - filter->dims[1] + 1), 1 };
	FilterResponse<T> *response = new FilterResponse<T>(rdims);
	T *feat_ptr, *filter_ptr, *resp_ptr;
	T val;

	for( int f=0; f<feat->dims[2]; f++ ){

		resp_ptr = response->data;
		feat_ptr = feat->data + (f * feat->dims[0] * feat->dims[1]);
		filter_ptr = filter->data + (f * filter->dims[0] * filter->dims[1]);

		for( int y=0; y<response->dims[1]; y++ ){

			for( int x=0; x<response->dims[0]; x++ ){

				val = 0;
				T *feat_tmp = feat_ptr + y*feat->dims[0] + x;
				T *filter_tmp = filter_ptr;

				for( int xp=0; xp<5/*filter->dims[0]*/; xp++ ){

					for (int yp=0; yp<5/*filter->dims[1]*/; yp++) {  val += feat_tmp[yp] * filter_tmp[yp];  }
					feat_tmp += feat->dims[0];
					filter_tmp += filter->dims[0];
				}
				*resp_ptr += val;
				resp_ptr++;
			}
		}
	}
	return response;
}

template <typename T>
FilterResponse<T>* HOGImage<T>::convolve(Filter<T> *filter){

	return this->convolve(this, filter);
}

} /* namespace tsm */

#endif
