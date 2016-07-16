
#ifndef PART_H_
#define PART_H_

#include "Array.cpp"

namespace tsm {

template <typename T>
class Part {
public:
	int defid;
	int filterid;
	int parent;
	int sizx;
	int sizy;
	int filterI;
	int defI;
	int scale;
	int startx;
	int starty;
	int step;
	int level;
	Vector<T> w;
	Array<T> *score;
	Array<int> *Ix;
	Array<int> *Iy;

public:
	Part();
	virtual ~Part();
	void reset();

	Array<T>* shiftdt();
	int find(T threshold, int fbuffer, Vector<int> *X, Vector<int> *Y);
	int nms_find(T threshold, int fbuffer, Vector<int> *X, Vector<int> *Y);

	static Array<T>* shiftdt(Array<T> *transform, Array<T> *score, T *w, int startx, int starty, int step, Array<int> *Ix, Array<int> *Iy);
	static Array<T>* shiftdt(Array<T> *score, T *w, int startx, int starty, int step, Array<int> *Ix, Array<int> *Iy);
	static int find(Array<T> *table, T threshold, int fbuffer, Vector<int> *X, Vector<int> *Y);
	static int nms_find(Array<T> *table, T threshold, int fbuffer, Vector<int> *X, Vector<int> *Y);
	static Array<T>* clearArea(Array<T> *area, int x, int y, T thresh, T rval);
};

} /* namespace tsm */

#endif /* PART_H_ */
