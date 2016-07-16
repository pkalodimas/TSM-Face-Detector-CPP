
#ifndef DEF_H_
#define DEF_H_

#include "Vector.cpp"

namespace tsm {

template <typename T>
class Def {
public:
	int i;
	Vector<T> *w;
	Vector<int> *anchor;
public:
	Def();
	virtual ~Def();
};

} /* namespace tsm */

#endif /* DEF_H_ */
