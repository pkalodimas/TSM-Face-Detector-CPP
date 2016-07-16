#ifndef VECTOR_H
#define VECTOR_H

namespace tsm {

template <typename T>
class Vector
{
public:
	T *data;
	int length;

public:
	Vector();
	Vector(int size);
	~Vector();

	void init(int size);
	void reset();
	bool copy(Vector<T> *table);
	Vector<T>* clone();

	T min(int start, int step);
	T max(int start, int step);
	bool add(Vector<T> *A);
	bool add(Vector<T> *A, Vector<T> *B);
	bool add(T value);
	bool add(Vector<T> *A, T value);
	bool swap(int a, int b);
	Vector<int>* sort();
	Vector<int>* sort(int start, int stop, Vector<int> *indexes);

	static Vector<T>* add(Vector<T> *A, T value, Vector<T> *C);
	static Vector<T>* add(Vector<T> *A, Vector<T> *B, Vector<T> *C);
	static Vector<T>* sort(Vector<T> *table, int start, int stop, Vector<int> *indexes);
};

} /* namespace tsm */

#endif
