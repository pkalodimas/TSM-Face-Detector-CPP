
#ifndef MODEL_H_
#define MODEL_H_

#include "Def.cpp"
#include "Filter.cpp"
#include "Component.cpp"

namespace tsm {

template <typename T>
class Model {
public:
//	float obj;
//	float delta;
	T thresh;
	int sbin;
	int interval;
//	int len;
	Vector<int> *maxsize;
	Vector< Component<T>* > *components;
	Vector< Def<T> > *defs;
	Vector< Filter<T>* > *filters;

public:
	Model();
	virtual ~Model();
	Model<T> *modelComponents(int padx, int pady);
};

} /* namespace tsm */

#endif /* MODEL_H_ */
