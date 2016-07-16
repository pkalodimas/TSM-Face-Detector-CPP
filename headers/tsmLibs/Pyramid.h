
#ifndef PYRAMID_H_
#define PYRAMID_H_

#include <cmath>
#include <algorithm>

#include "Image.cpp"
#include "Model.cpp"

using namespace std;

namespace tsm {

template <typename T>
class Pyramid {
public:
	Vector< HOGImage<T>* > *pyra;
	Vector<float> *scales;
	int MinLevel;
	int MaxLevel;
	int MinFace;
	int levels;
	int interval;
	int imx;
	int imy;
	int padx;
	int pady;
	int sBin;
public:
	Pyramid();
	virtual ~Pyramid();

	void releaseLevel(int level);
	bool init(int interval, int sbin, int sourceDims[], Vector<int> *maxsize);
	bool init(Model<T> *model, Image<uint8_t> *image);
	bool setCustomPyramidSettings(int minlevel, int maxlevel, int minface);
	Vector<float>* createPyramidScales(int *sourceDims);
	Vector< HOGImage<T>* >* createPyramid(Model<T> *model, Image<uint8_t> *image);
	Vector< HOGImage<T>* >* createPyramid(Model<T> *model, Image<uint8_t> *image, int maxlevel, int minlevel, int minface);

	static Vector<float>* createPyramidScales(int sourceDims[], int minlevel, int maxlevel, int sbin, int interval, int minface);
	static Vector< HOGImage<T>* >* createPyramid(Image<uint8_t> *sourceImage, Vector<float>* scales, int interval, int sbin, Vector<int> maxsize[]);
};

} /* namespace tsm */

#endif /* HEADERS_TSMLIBS_PYRAMID_H_ */
