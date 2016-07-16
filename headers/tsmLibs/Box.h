
#ifndef BOX_H_
#define BOX_H_

#include "Array.cpp"

namespace tsm {

template <typename T>
class Box {
public:
	T s;
	int c;
	int level;
	Array<T> *xy;

public:
	Box();
	virtual ~Box();
};

} /* namespace tsm */

#endif /* BOX_H_ */
