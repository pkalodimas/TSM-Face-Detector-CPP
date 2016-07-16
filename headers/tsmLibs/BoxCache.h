
#ifndef BOXCACHE_H_
#define BOXCACHE_H_

#include <algorithm>
#include "Box.cpp"

namespace tsm {

template <typename T>
class BoxCache {
public:
	Vector< Box<T> > *boxes;
	int size;

public:
	BoxCache();
	BoxCache(int size);
	virtual ~BoxCache();
	void reset();
	void init(int size);
	Vector< Box<T> > *getResults();

	void clipboxes(int imx, int imy);
	Vector< Box<T> >* nms(int overlap, int nms_limit);
};

} /* namespace tsm */

#endif /* BOXCACHE_H_ */
