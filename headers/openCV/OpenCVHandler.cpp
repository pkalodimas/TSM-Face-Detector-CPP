#ifndef OPENCVHANDLER_CPP_
#define OPENCVHANDLER_CPP_

#include "OpenCVHandler.h"

using namespace cv;

namespace tsm {

template <typename T>
OpenCVHandler<T>::OpenCVHandler() {
	this->imagePath = nullptr;
	this->CVimage = nullptr;
}

template <typename T>
OpenCVHandler<T>::OpenCVHandler(std::string *filepath) {
	this->imagePath = filepath;
	this->CVimage = nullptr;
}

template <typename T>
OpenCVHandler<T>::~OpenCVHandler() {
	delete this->imagePath;
	delete this->CVimage;
}

template <typename T>
Image<uint8_t>* OpenCVHandler<T>::cv2ArrayImage(cv::Mat* cvimage){

	if( !cvimage || !cvimage->data ) return nullptr;

	int d[3] = {cvimage->cols, cvimage->rows, 3};
	Image<uint8_t> *image = new Image<uint8_t>(d);

	uint8_t *glevel = image->data + (image->dims[0] * image->dims[1]);
	uint8_t *blevel = glevel + (image->dims[0] * image->dims[1]);

	for( int y=0; y<image->dims[1]; y++ ){
		for( int x=0; x<image->dims[0]; x++ ){

			image->data[x+y*image->dims[0]] = cvimage->data[cvimage->channels()*(cvimage->cols*y + x) + 0];
			glevel[(x+y*image->dims[0])] = cvimage->data[cvimage->channels()*(cvimage->cols*y + x) + 1];
			blevel[(x+y*image->dims[0])] = cvimage->data[cvimage->channels()*(cvimage->cols*y + x) + 2];
		}
	}

	return image;
}

template <typename T>
Image<uint8_t>* OpenCVHandler<T>::cv2ArrayImage(){

	return cv2ArrayImage(this->CVimage);
}

template <typename T>
cv::Mat* OpenCVHandler<T>::readImage(std::string *filepath){

	if( !filepath ) return nullptr;

	cv::Mat *cvimage = new cv::Mat();
	*cvimage = cv::imread(filepath->c_str(), CV_LOAD_IMAGE_COLOR);
	if( !cvimage->data ) {
		delete cvimage;
		return nullptr;
	}

	return cvimage;
}

template <typename T>
bool OpenCVHandler<T>::readImage(){

	this->CVimage = this->readImage(this->imagePath);
	if( !this->CVimage ) return false;
	return true;
}

template <typename T>
Image<uint8_t>* OpenCVHandler<T>::importImage2Array(std::string *filepath){

	cv::Mat *cvimage = OpenCVHandler<T>::readImage(filepath);
	if( !cvimage ) return nullptr;

	Image<T> *image = OpenCVHandler<T>::cv2ArrayImage(cvimage);
	cvimage->release();
	delete cvimage;

	return image;
}

template <typename T>
cv::Mat* OpenCVHandler<T>::drawBoxes2Image(cv::Mat *cvimage, Vector< Box<T> > *boxes){

	if( !boxes || !cvimage || !cvimage->data ) return nullptr;

	for( int i=0; i<boxes->length; i++ ){

		Box<T> *box = &boxes->data[i];

		int box_edge[4];
		box_edge[0] = (int) box->xy->min(0, box->xy->dims[0]);
		box_edge[1] = (int) box->xy->min(1, box->xy->dims[0]);
		box_edge[2] = (int) box->xy->max(2, box->xy->dims[0]);
		box_edge[3] = (int) box->xy->max(3, box->xy->dims[0]);
		int partsize = (int) box->xy->data[2] - (int) box->xy->data[0];

		cv::rectangle(*cvimage, cvPoint((int)box_edge[0]-1, (int)box_edge[1]-1), cvPoint((int)box_edge[2]+1, (int)box_edge[3]+1), CV_RGB(0,0,0), 1, 8, 0);
		cv::putText(*cvimage, std::to_string(i+1), cvPoint((int)(box_edge[0] + box_edge[2] - partsize)/2, (int)(box_edge[1] - partsize/2)), CV_FONT_HERSHEY_TRIPLEX, 1, CV_RGB(0,0,0), 1, 8, false);

		for( int j=0; j<box->xy->dims[1]; j++ ){

			T *box_line = &box->xy->data[j*box->xy->dims[0]];

			cv::rectangle(*cvimage, cvPoint((int)box_line[0], (int)box_line[1]), cvPoint((int)box_line[2], (int)box_line[3]), CV_RGB(0,254,0), 1, 8, 0);
			cv::circle(*cvimage, cvPoint((int) (box_line[0]+box_line[2])/2, (int) (box_line[1]+box_line[3])/2), 2, CV_RGB(254,0,0), -1, 8, 0);
		}
	}

	return cvimage;
}

template <typename T>
bool OpenCVHandler<T>::drawBoxes2Image(Vector< Box<T> > *boxes){

	if( !this->drawBoxes2Image(this->CVimage, boxes) ) return false;
	return true;
}

template <typename T>
bool OpenCVHandler<T>::showImage(cv::Mat *cvimage, std::string *windowTitle){

	if( !cvimage || !cvimage->data || !windowTitle ) return false;

	cv::namedWindow(windowTitle->c_str(), WINDOW_AUTOSIZE);
	cv::imshow(windowTitle->c_str(), *cvimage);
	waitKey(0);

	return true;
}

template <typename T>
bool OpenCVHandler<T>::showImage(std::string *windowTitle){

	return showImage(this->CVimage, windowTitle);
}


template <typename T>
bool OpenCVHandler<T>::saveImage(cv::Mat *cvimage, std::string *filepath){

	if( !cvimage || !cvimage->data || !filepath ) return false;

	if( !imwrite(filepath->c_str(), *cvimage) ) return false;
	return true;
}

template <typename T>
bool OpenCVHandler<T>::saveImage(){

	return this->saveImage(this->CVimage, this->imagePath);
}

template <typename T>
bool OpenCVHandler<T>::saveImage(std::string *filepath){

	return this->saveImage(this->CVimage, filepath);
}

} /* namespace tsm */

#endif /* OPENCVHANDLER_CPP_ */

