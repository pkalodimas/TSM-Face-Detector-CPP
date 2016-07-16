
#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <cstdint>

#include "Part.cpp"
#include "FilterResponse.cpp"

namespace tsm {

template <typename T>
class Component {
public:
	Vector< Part<T> > *parts;
	int id;
	int pose;
public:
	Component();
	virtual ~Component();

	void reset();
	Array<T>* shiftdt(Vector< FilterResponse<T>* > *responses, int level);
	Vector< Array<T>* >* backtrack(Vector<int> *X, Vector<int> *Y, Vector<float> *scales, int padx, int pady);
};

} /* namespace tsm */

#endif /* COMPONENT_H_ */
