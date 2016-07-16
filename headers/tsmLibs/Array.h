
#ifndef ARRAY_H_
#define ARRAY_H_

#include "Vector.cpp"

namespace tsm {

template <typename T>
class Array: public Vector<T> {
public:
	int dims[3];

public:
	Array();
	Array(int *size);
	Array(int x, int y, int z);
	virtual ~Array();

	bool copyDims(int *dims);
	void init(int size[]);
	void reset();
	bool copy(Array<T> *table);
	Array<T>* clone();

	bool add(T value);
	bool add(Array<T> *A);
	bool add(Array<T> *A, Array<T> *B);
	static Array<T>* add(Array<T> *A, Array<T> *B, Array<T> *C);
	static Array<T>* add(Array<T> *A, T value, Array<T> *C);
};

} /* namespace tsm */

#endif /* HEADERS_MYLIBS_ARRAY_H_ */
