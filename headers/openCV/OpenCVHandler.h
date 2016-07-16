
#ifndef OPENCVHANDLER_H_
#define OPENCVHANDLER_H_

#include <stdint.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../tsmLibs/Box.cpp"
#include "../tsmLibs/Image.h"

//using namespace cv;

namespace tsm {

template <typename T>
class OpenCVHandler {
public:
	std::string *imagePath;
	cv::Mat *CVimage;
public:
	OpenCVHandler();
	OpenCVHandler(std::string *filepath);
	virtual ~OpenCVHandler();

	bool readImage();
	bool saveImage();
	bool saveImage(std::string *filepath);
	bool drawBoxes2Image(Vector< Box<T> > *boxes);
	bool showImage(std::string *windowTitle);
	Image<uint8_t>* cv2ArrayImage();

	static Image<uint8_t>* cv2ArrayImage(cv::Mat *cvimage);
	static cv::Mat* readImage(std::string *filepath);
	static Image<uint8_t>* importImage2Array(std::string *filepath);
	static cv::Mat* drawBoxes2Image(cv::Mat *cvimage, Vector< Box<T> > *boxes);
	static bool showImage(cv::Mat *cvimage, std::string *windowTitle);
	static bool saveImage(cv::Mat *cvimage, std::string *filepath);
};

} /* namespace tsm */

#endif /* OPENCVHANDLER_H_ */
